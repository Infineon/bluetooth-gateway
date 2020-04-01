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

/** @bluetooth_mesh.c
 *
 * This handles the mesh api functions when USE_WICED_HCI is defined
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wiced_hci.h"
#include "wiced_hci_bt_mesh.h"
#include "wiced_hci_bt_common_internal.h"
#include "cyabs_rtos.h"

/******************************************************
 *                    Constants
 ******************************************************/
#define CONNECTION_STATUS_LENGTH 3

#define DEFAULT_CONNECTION_ID     0x03

/******************************************************
  *                   Structures
  ******************************************************/

 typedef struct wiced_hci_bt_mesh_context {
         wiced_bt_mesh_provision_end_cb_t     prov_end_cb;
         wiced_bt_mesh_core_gatt_send_cb_t    proxy_data_cb;
         wiced_bt_mesh_write_nvram_data_cb_t  write_nvram_data_cb;
         wiced_bt_mesh_status_cb_t            mesh_status_cb;
}wiced_hci_bt_mesh_context_t;

/******************************************************
  *               Variable Definitions
  ******************************************************/

wiced_hci_bt_mesh_context_t wh_bt_mesh_context;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void wiced_hci_mesh_cb(uint16_t event, uint8_t* payload, uint32_t len);

/******************************************************
  *               Function Definitions
  ******************************************************/

static void wiced_hci_mesh_cb(uint16_t event, uint8_t* payload, uint32_t len)
{
    uint8_t*                     p = payload;

    switch(event)
    {
        case HCI_CONTROL_MESH_EVENT_PROVISIONING_STATUS:
        {
            uint32_t conn_id = 0x01;
            uint8_t result = *p;
            WICED_INFO(("HCI_CONTROL_MESH_EVENT_PROVISIONING_STATUS : %02X \n",result));
            (*wh_bt_mesh_context.prov_end_cb)(conn_id, result);
        }
        break;
        case HCI_CONTROL_MESH_EVENT_PROXY_DATA:
        {
            WICED_INFO(("HCI_CONTROL_MESH_EVENT_PROXY_DATA\n "));
            uint32_t packet_len;
            uint8_t *packet;
            packet_len = len;
            packet = (uint8_t*) malloc(len);
            memcpy(packet, p, len);
            (*wh_bt_mesh_context.proxy_data_cb)(packet, packet_len);
        }
        break;
        case HCI_CONTROL_EVENT_DEVICE_STARTED :
        {

            WICED_INFO(("HCI_CONTROL_EVENT_DEVICE_STARTED "));
            cy_rtos_delay_milliseconds(1000);
        }
        break;
        case HCI_CONTROL_MESH_EVENT_NVRAM_DATA:
        {
            uint16_t nvram_id;
            uint8_t *packet;
            STREAM_TO_UINT16(nvram_id, p);
            packet = (uint8_t*) malloc(len-2);
            memcpy(packet, p, len-2);
            (*wh_bt_mesh_context.write_nvram_data_cb)(nvram_id, packet, len - 2);
        }
        break;

        case HCI_CONTROL_MESH_EVENT_MESH_STATUS :
        {
            uint8_t result = *p;
            WICED_INFO(("\n Received HCI_CONTROL_MESH_EVENT_MESH_STATUS = %02X\n ", result));
            (*wh_bt_mesh_context.mesh_status_cb)(result);
             break;
        }
        default:
            WICED_INFO(("%s EVENT not handled\n", __FUNCTION__));
            break;
    }
}

cy_rslt_t wiced_bt_mesh_proxy_connect(uint8_t connection_state)
{
    uint8_t     data[CONNECTION_STATUS_LENGTH];
    cy_rslt_t   result = CY_RSLT_SUCCESS;

    WICED_INFO(("proxy connect state = %d \n", connection_state));
     if (connection_state)
     {
         /*TBD : provide flexibility to pass connection id from the MCU application */
         data[0]  = DEFAULT_CONNECTION_ID;
         data[1]  = 0x00;
     }
     else
     {
         /* for disconnection connection id should be zero */
         data[0]  = 0x00;
         data[1]  = 0x00;
     }
    data[2] = 0;
    wiced_hci_send( HCI_CONTROL_MESH_COMMAND_SEND_CONN_STATUS, data, CONNECTION_STATUS_LENGTH );
    return result;
}


cy_rslt_t wiced_bt_mesh_send_proxy_packet(uint8_t* p_data, uint8_t data_len)
{
    uint8_t*        data  = NULL;
    uint16_t        length  = data_len;
    cy_rslt_t   result = CY_RSLT_SUCCESS;

    WICED_INFO(("[%s]\n",__func__));
    /* allocate memory for the data */
    data = (uint8_t*)calloc(length, sizeof(uint8_t));
    memcpy(data,p_data,length);
    wiced_hci_send( HCI_CONTROL_MESH_COMMAND_SEND_PROXY_DATA , data, length );
    free(data);
    return result;
}


cy_rslt_t wiced_bt_mesh_init( wiced_bt_mesh_provision_end_cb_t prov_end_cb,
                                   wiced_bt_mesh_core_gatt_send_cb_t proxy_data_cb,
                                   wiced_bt_mesh_write_nvram_data_cb_t write_nvram_data_cb,
                                   wiced_bt_mesh_status_cb_t mesh_status_cb )
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    WICED_INFO(("[%s]\n",__func__));
    /* Copy the callback information */
    wh_bt_mesh_context.prov_end_cb         = prov_end_cb;
    wh_bt_mesh_context.proxy_data_cb       = proxy_data_cb;
    wh_bt_mesh_context.write_nvram_data_cb = write_nvram_data_cb;
    wh_bt_mesh_context.mesh_status_cb      = mesh_status_cb;
    /* set the event callback */
    wiced_hci_set_event_callback( MESH, wiced_hci_mesh_cb );
    wiced_hci_send( HCI_CONTROL_MESH_COMMAND_APP_START , NULL, 1 );
    return result;
}

cy_rslt_t wiced_bt_mesh_reboot()
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    WICED_INFO(("[%s]\n",__func__));
    wiced_hci_send( HCI_CONTROL_MESH_COMMAND_STACK_INIT , NULL, 1 );
    return result;
}

cy_rslt_t wiced_bt_mesh_push_nvram_data(uint8_t *data_in , uint8_t data_len , uint16_t idx)
{
    uint8_t*                data  = NULL;
    uint8_t*                p     = NULL;
    uint16_t                length  = 0;
    length = data_len + 2;
    /* allocate memory for the data */
    data = (uint8_t*)calloc(length, sizeof(uint8_t));
    p = data;
    p[0] = (uint8_t)idx;
    p[1] = (uint8_t)(idx >> 8);
    p = p+2;
    memcpy( p, data_in, data_len );

    wiced_hci_send(HCI_CONTROL_COMMAND_PUSH_NVRAM_DATA, data, length);
    free(data);
    return CY_RSLT_SUCCESS;

}
