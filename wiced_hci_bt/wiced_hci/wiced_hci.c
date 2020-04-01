/*
 * Copyright 2020, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 *
 * HOST OFFLOADING: wiced_hci implementation
 *
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "bt_firmware.h"
#include "wiced_hci.h"
#include "wiced_uart.h"
#include "cy_result_mw.h"
#include "cyabs_rtos.h"

/******************************************************
 *                    Constants
 ******************************************************/

#define WICED_HCI_QUEUE_MAX_ENTRIES                20
#define WICED_HCI_CMD_THREAD_STACK_SIZE            (4096)
#define WICED_HCI_NUM_UART_THREADS                 3
#define WICED_HCI_HEADER_LENGTH                    5

#define WICED_NO_WAIT       0
#define WICED_WAIT_FOREVER  ((uint32_t) 0xFFFFFFFF)
#define WICED_NEVER_TIMEOUT ((uint32_t) 0xFFFFFFFF)

#define HCI_CMD_THREAD_STACK_SIZE                  (4*1024)
#define HCI_READ_THREAD_STACK_SIZE                 (4*1024)

/******************************************************
 *                   Structures
 ******************************************************/

/**
 * Queue element for app framework queue.
 */
typedef struct
{
    void        (*function)(void);
    bool        wait_for_event_complete;
} thread_queue_element_t;

/**
 * Context data for the big HCI.
 *
 */
typedef struct
{
    wiced_hci_cb                evt_cb[MAX_CONTROL_GROUP];
} wiced_hci_context_t;

static cy_thread_t hci_read_thread;

//MBED_ALIGN(8) uint8_t hci_cmd_thread_cb[HCI_CMD_THREAD_STACK_SIZE] = {0};
__attribute__((aligned(8))) uint8_t hci_read_thread_stack [HCI_READ_THREAD_STACK_SIZE] =  { 0 };
__attribute__((aligned(8))) uint8_t hci_cmd_thread_stack  [HCI_CMD_THREAD_STACK_SIZE]  = { 0 };

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void wiced_hci_read_thread(uint32_t args);
static void wiced_hci_write_command(uint16_t command, uint8_t* payload, uint16_t length);

/* moving this to global, as the Free RTOS stack size is only 4096 bytes
 * the wiced_hci_read_thread()  thread is declaring a stack buffer of 2048
 * and in turn calls parse_wiced_pkt() which is also declaring local variable of
 * buffer size 2048 bytes on stack causing stack overflow in FreeRTOS-LwIP
 */
static uint8_t data_parsepkt[2048];
static uint8_t data_hciread[2048];

/******************************************************
 *               External Function Declarations
 ******************************************************/
extern const char cy_patch_version[];
extern const uint8_t cy_patchram_buf[];
extern const int cy_patch_ram_length;

/******************************************************
 *               Variable Definitions
 ******************************************************/
wiced_hci_context_t   wiced_hci_context;
/******************************************************
 *               Function Definitions
 ******************************************************/

/*****************************************************
** Function         optimizations to copy BD address to and from a stream
*******************************************************/
#define MY_BDADDR_TO_STREAM(p, a)   {register int ijk; for (ijk = 0; ijk < BD_ADDR_LEN;  ijk++) *(p)++ = (uint8_t) a[BD_ADDR_LEN - 1 - ijk];}
uint8_t *copyBdaToStream(uint8_t *pStream, uint8_t *pBDA)
{
    MY_BDADDR_TO_STREAM(pStream, pBDA);
    return (pStream);
}

#define MY_STREAM_TO_BDADDR(a, p) { register int ijk; register uint8_t *pbda = (uint8_t *)a + BD_ADDR_LEN - 1; for (ijk = 0; ijk < BD_ADDR_LEN; ijk++) *pbda-- = *p++; }
uint8_t *copyStreamToBda(uint8_t *pBDA, uint8_t *pStream)
{
    MY_STREAM_TO_BDADDR(pBDA, pStream);
    return (pStream);
}

static void parse_wiced_pkt(void)
{
    uint32_t length = 2;
    uint8_t control_gp = 0;
    uint16_t control_cmd = 0;

    memset(data_parsepkt, 0, sizeof(data_parsepkt));

    if( (cy_hci_uart_read(data_parsepkt, &length, WICED_NEVER_TIMEOUT)) != CY_RSLT_SUCCESS)
    {
        WICED_ERROR(("[%s]\n",__func__));
        return;
    }

    if (length == 0)
        return;

    control_cmd = data_parsepkt[0] + (data_parsepkt[1] << 8);
    control_gp = HCI_CONTROL_GROUP(data_parsepkt[0] + (data_parsepkt[1] << 8) );

    length= 2;
    if( (cy_hci_uart_read(data_parsepkt, &length,WICED_NEVER_TIMEOUT)) != CY_RSLT_SUCCESS)
    {
        WICED_ERROR(("[%s]\n",__func__));
        return;
    }

    if (length == 0)
        return;

    length = data_parsepkt[0] + (data_parsepkt[1] << 8);

    if ( length > 0 )
    {
        if( (cy_hci_uart_read(data_parsepkt, &length, WICED_NEVER_TIMEOUT)) != CY_RSLT_SUCCESS)
        {
            WICED_ERROR(("[%s]\n",__func__));
            return;
        }
        if (length == 0)
            return;
    }
    switch(control_gp)
    {
        case HCI_CONTROL_GROUP_DEVICE:
        case HCI_CONTROL_GROUP_LE:
        case HCI_CONTROL_GROUP_GATT:
        case HCI_CONTROL_GROUP_MESH:
            if(wiced_hci_context.evt_cb[control_gp])
            {
                wiced_hci_context.evt_cb[control_gp]( control_cmd, data_parsepkt, length );
            }
            break;
        case HCI_CONTROL_GROUP_MISC:
        default:
            WICED_INFO(("[%s %d]COMMAND NOT SUPPORTED\n",__func__,__LINE__));
            break;

    }
}

static void wiced_hci_read_thread(uint32_t args)
{
    uint32_t  length = 1;
    cy_rslt_t result = CY_RSLT_SUCCESS;
#if !defined(WICED_HCI_FW_DOWNLOAD_BYPASS)
    WICED_INFO(("[HCI] Downloading Firmware...\n"));
    result = bt_firmware_download( cy_patchram_buf, cy_patch_ram_length, cy_patch_version );
    if ( result != CY_RSLT_SUCCESS )
    {
        WICED_ERROR(("[HCI] Error downloading HCI firmware\n"));
        return;
    }
    WICED_INFO(("[HCI] Firmware Download Complete.\n"));
#else
    UNUSED_VARIABLE( result );
    UNUSED_VARIABLE( bt_uart_config );
#endif
    cy_rtos_delay_milliseconds(1000);
    while( CY_TRUE )
    {
        cy_hci_uart_read(data_hciread, &length, WICED_NEVER_TIMEOUT);
        switch(data_hciread[0])
        {
            case HCI_WICED_PKT:
                parse_wiced_pkt();
                /*
                 * call the appropriate evt_cb in the wiced_hci_context
                 */
                break;

            default:
                break;
        }
    }
}

static void wiced_hci_write_command(uint16_t command, uint8_t* payload, uint16_t length)
{
    uint8_t    data[1040]; // TODO: make this dynamic
    uint32_t   header = 0;

    data[header++] = HCI_WICED_PKT;
    data[header++] = command & 0xff;
    data[header++] = (command >> 8) & 0xff;
    data[header++] = length & 0xff;
    data[header++] = (length >> 8) & 0xff;

    if(length != 0)
        memcpy(&data[header], payload, length);
    cy_hci_uart_write(data,length+WICED_HCI_HEADER_LENGTH);
}

void wiced_hci_send(uint32_t opcode, uint8_t* data, uint16_t length)
{
    wiced_hci_write_command(opcode, data, length);
}

cy_rslt_t wiced_hci_up()
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* initialize the uart */
    result = cy_hci_uart_init();
    if ( result != CY_RSLT_SUCCESS )
    {
        WICED_ERROR(("[HCI] UART initialization failed(result: %lu)\n", result));
        return result;
    }

    /* create a read thread for the hci */
    result = cy_rtos_create_thread(&hci_read_thread, wiced_hci_read_thread, "hci_read_thread",
                            hci_read_thread_stack, sizeof(hci_read_thread_stack), CY_RTOS_PRIORITY_NORMAL, (cy_thread_arg_t)NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        WICED_ERROR(("[HCI] Fatal Error - Could not create Read Thread\n"));
        return result;
    }

    return result;
}



cy_rslt_t wiced_hci_down(void)
{
    cy_rslt_t result= CY_RSLT_SUCCESS;

    /* Kill the thread */
    result = cy_rtos_terminate_thread(&hci_read_thread);
    if (result != CY_RSLT_SUCCESS)
    {
        WICED_ERROR(("[HCI] Fatal Error - Could not terminate Read Thread\n"));
        return result;
    }

    /* de-initialize the UART */
    result = cy_hci_uart_deinit();
    if ( result != CY_RSLT_SUCCESS )
    {
        WICED_ERROR(("[HCI] UART de-initialization failed (result: %lu)\n",result));
        return result;
    }

    /* set the control block to 0 */
    memset(data_parsepkt, 0, sizeof(data_parsepkt));
    memset(data_hciread, 0, sizeof(data_hciread));
    memset(&wiced_hci_context, 0 ,sizeof(wiced_hci_context));
    return result;
}

cy_rslt_t wiced_hci_set_event_callback(control_group_t group, wiced_hci_cb evt_cb)
{
    if ( group >= MAX_CONTROL_GROUP)
    {
        return CY_RSLT_MW_ERROR;
    }

    wiced_hci_context.evt_cb[group] = evt_cb;

    return CY_RSLT_SUCCESS;
}

