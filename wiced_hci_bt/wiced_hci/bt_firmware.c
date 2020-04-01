/**
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
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bt_hci_interface.h"
#include "wiced_uart.h"
#include "cy_result_mw.h"


/******************************************************
 *                      Macros
 ******************************************************/


/* Verify if bt_bus function returns success.
 * Otherwise, returns the error code immediately.
 * Assert in DEBUG build.
 */
#define VERIFY_RETVAL( function_call ) \
do \
{ \
    cy_rslt_t verify_result = (function_call); \
    if ( verify_result != CY_RSLT_SUCCESS ) \
    { \
        printf("[%s %d] VERIFY FAILED\n",__func__,__LINE__); \
        return verify_result; \
    } \
} while ( 0 )

/* Verify if HCI response is expected.
 * Otherwise, returns HCI unexpected response immediately.
 * Assert in DEBUG build.
 */
#define VERIFY_RESPONSE( a, b, size ) \
{ \
    if ( memcmp( (a), (b), (size) ) != 0 ) \
    { \
        return WICED_BT_UNKNOWN_PACKET; \
    } \
}

/******************************************************
 *                    Constants
 ******************************************************/
#define DEFAULT_READ_TIMEOUT (100)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               extern Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/
cy_rslt_t bt_issue_reset ( void );

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
cy_rslt_t bt_issue_reset ( void )
{
    uint32_t length = 4;
    uint8_t hci_data[ 8 ] = { 0x00 };
    uint8_t hci_reset_cmd[] = { 0x1, 0x03, 0x0c, 0x00}; /* reset code as per spec */
    uint8_t hardware_error[ 4 ] = { 0x04, 0x10, 0x01, 0x00 };
    uint8_t hci_reset_expected_event[] = {0x04, 0x0E, 0x04, 0x01, 0x03, 0x0C, 0x00};

    cy_hci_uart_write(hci_reset_cmd, length);

    length = 7; /* length of expected response */
    /* if any hardware parsing error event just ignore it and read next bytes */
    cy_hci_uart_read(hci_data, &length, 110);

    if ( !memcmp( hardware_error, hci_data, 4 ) )
    {
        printf(( "hardware parsing error received \n" ));
        length = sizeof(hardware_error);
        VERIFY_RETVAL( cy_hci_uart_read(hci_data, &length,100) );
    }

    if ( memcmp( hci_data, hci_reset_expected_event, 7 )!=0 )
    {
        printf(( "HCI_CMD_RESET command reponse is wrong \n" ));
        return CY_RSLT_MW_ERROR;
    }
    return CY_RSLT_SUCCESS;
}


cy_rslt_t bt_firmware_download( const uint8_t* firmware_image, uint32_t size, const char* version )
{
    uint8_t* data = (uint8_t*) firmware_image;
    uint32_t remaining_length = size;
    uint8_t hci_event[100];

    if(bt_issue_reset()!= CY_RSLT_SUCCESS)
        return CY_RSLT_MW_ERROR;

    /* Send hci_download_minidriver command */
    uint8_t minidrv[] = {0x1, 0x2e, 0xfc, 00};
    uint8_t hci_data[100];
    uint32_t length = 7;
    cy_hci_uart_write(minidrv, 4);
    cy_hci_uart_read(hci_data, &length,100);
    /* The firmware image (.hcd format) contains a collection of hci_write_ram command + a block of the image,
     * followed by a hci_write_ram image at the end. Parse and send each individual command and wait for the response.
     * This is to ensure the integrity of the firmware image sent to the bluetooth chip.
     */
    while ( remaining_length )
    {
        uint32_t data_length = data[ 2 ] + 3; /* content of data length + 2 bytes of opcode and 1 byte of data length */
        hci_command_opcode_t command_opcode = *(hci_command_opcode_t*) data;
        uint8_t temp_data[ 512 ];
        uint32_t bytes_read;
        memset( &hci_event, 0, sizeof( hci_event ) );
        memset( temp_data, 0, sizeof( temp_data ) );

        temp_data[ 0 ] = HCI_COMMAND_PACKET;
        memcpy( &temp_data[ 1 ], data, data_length );

       // printf ("remaiing length = %d \n", remaining_length);

        /* Send hci_write_ram command. The length of the data immediately follows the command opcode */
        cy_hci_uart_write(temp_data, data_length+1);
        bytes_read = 7;
        cy_hci_uart_read(hci_event, &bytes_read,220);

        switch ( command_opcode )
        {
            case HCI_CMD_OPCODE_WRITE_RAM:
                /* Update remaining length and data pointer */
                data += data_length;
                remaining_length -= data_length;
                break;

            case HCI_CMD_OPCODE_LAUNCH_RAM:
                /* All responses have been read. Now let's flush residual data if any and reset remaining length */
                bytes_read = 1;
                remaining_length = 0;
                break;

            default:
                return 0;
        }
    }

    /* Wait for bluetooth chip to pull its RTS (host's CTS) low. From observation using CRO, it takes the bluetooth chip > 170ms to pull its RTS low after CTS low */
    return 0;

}
