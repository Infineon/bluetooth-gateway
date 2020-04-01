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

/**
 * @file wiced_hci_bt_dm.c
 *
 * This handles the Device management functions when USE_WICED_HCI is defined.
 *
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cy_result_mw.h"
#include "wiced_hci.h"
#include "wiced_hci_bt_common_internal.h"
#include "wiced_defs.h"

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void wiced_hci_dm_cb(uint16_t command, uint8_t* payload, uint32_t len);

/******************************************************
 *               External Function Declarations
 ******************************************************/

extern cy_rslt_t wiced_hci_up();

/******************************************************
 *               Variable Definitions
 ******************************************************/

wiced_hci_bt_dm_context_t              wh_bt_dm_context;
wiced_hci_bt_gatt_context_t            wh_bt_gatt_context;

/******************************************************
 *               Function Definitions
 ******************************************************/

static void wiced_hci_dm_cb(uint16_t command, uint8_t* payload, uint32_t len)
{
    wiced_bt_management_evt_t evt = 0;
    wiced_bt_management_evt_data_t data;

    WICED_DEBUG((" [%s] [%x] [%d]\n\n", __func__, command, (int)len));

    switch(command)
    {

        case HCI_CONTROL_EVENT_READ_LOCAL_BDA:
            STREAM_TO_BDADDR( wh_bt_dm_context.bd_addr, payload );
            evt = 0xff;
            break;

        case HCI_CONTROL_EVENT_DEVICE_STARTED:
            WICED_INFO(("[%s] HCI_CONTROL_EVENT_DEVICE_STARTED\n",__func__));
            if ( payload )
            {
                STREAM_TO_UINT8(data.enabled.status, payload);
                evt = BTM_ENABLED_EVT;
            }
            break;

        case HCI_CONTROL_LE_EVENT_CONNECTED:
        {
            wiced_bt_gatt_event_data_t    event_data;
            wiced_bt_device_address_t  temp_bdadr;
            WICED_INFO(("[%d] LE Device Connected event\n",(int)len));

            if(payload)
            {
                STREAM_TO_UINT8( event_data.connection_status.addr_type , payload );
                STREAM_TO_BDADDR(temp_bdadr, payload);
                event_data.connection_status.bd_addr = temp_bdadr;
                STREAM_TO_UINT16( event_data.connection_status.conn_id, payload );
                STREAM_TO_UINT8( event_data.connection_status.link_role, payload );
                event_data.connection_status.transport = BT_TRANSPORT_LE;
                event_data.connection_status.connected = TRUE;

                wh_bt_gatt_context.gatt_mgmt_cb( GATT_CONNECTION_STATUS_EVT, &event_data );

                evt = 0xff;
            }
            break;
        }

        case HCI_CONTROL_LE_EVENT_DISCONNECTED:
        {
            wiced_bt_gatt_event_data_t    event_data;
            WICED_INFO(("HCI_CONTROL_LE_EVENT_DISCONNECTED:[%s]\n",__func__));

            evt = 0xff;
            memset(&event_data, 0, sizeof(wiced_bt_gatt_event_data_t));
            if(payload)
            {
                STREAM_TO_UINT16( event_data.connection_status.conn_id, payload );
                STREAM_TO_UINT8( event_data.connection_status.reason, payload );
                event_data.connection_status.connected = FALSE;

                wh_bt_gatt_context.gatt_mgmt_cb( GATT_CONNECTION_STATUS_EVT, &event_data );
            }
            break;
        }

        case HCI_CONTROL_EVENT_COMMAND_STATUS:
        {
#ifdef ENABLE_BT_PROTOCOL_TRACES
            uint8_t control_event_status;
            STREAM_TO_UINT8(control_event_status, payload);
            WICED_DEBUG(("HCI_CONTROL_EVENT_COMMAND_STATUS: %d\n", control_event_status));
#endif
            evt = 0xff;
            break;
        }
        case HCI_CONTROL_EVENT_WICED_TRACE:
        {
#ifdef ENABLE_BT_PROTOCOL_TRACES
            char str[200];
            memcpy(str,payload,len);
            str[len]='\0';
            WICED_DEBUG((" Trace message:\n------------------------\n%s------------------------\n",str));
#endif
            evt = 0xff;
            break;
        }

        default:
            evt = 0xff;
            break;

    }
    if(evt != 0xff)
        wh_bt_dm_context.dm_mgmt_cb( evt, &data );
}

cy_rslt_t wiced_bt_stack_init(wiced_bt_management_cback_t *p_bt_management_cback)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    wh_bt_dm_context.dm_mgmt_cb = p_bt_management_cback;

    /* Setting the event callbacks as the first step to ensure that any data from the BT chip is routed to the right callback */
    wiced_hci_set_event_callback(DEVICE, wiced_hci_dm_cb);
    wiced_hci_set_event_callback(LE, wiced_hci_dm_cb);

    /* start wiced_hci only here. Once started, it stays up. */
    if ( (result = wiced_hci_up() ) != CY_RSLT_SUCCESS )
    {
        WICED_ERROR(( "[%s] Failed to initialize wiced hci\n",__FUNCTION__));
        return result;
    }
    WICED_INFO(("[%s] wiced_hci up\n",__FUNCTION__));

    return result;
}

cy_rslt_t wiced_bt_stack_deinit(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    WICED_INFO(("[%s]\n",__func__));

    /* Call the wiced_hci_down */
    if ( (result = wiced_hci_down() ) != CY_RSLT_SUCCESS )
    {
        WICED_ERROR(( "[%s] Failed to initialize wiced hci\n",__FUNCTION__));
        return result;
    }

    /* Send BTM_DISABLED_EVENT */
    wh_bt_dm_context.dm_mgmt_cb( BTM_DISABLED_EVT, NULL );

    memset(&wh_bt_dm_context, 0 ,sizeof(wh_bt_dm_context));

    return result;
}

void wiced_bt_set_local_bdaddr( wiced_bt_device_address_t  bda )
{
    uint8_t*        data  = NULL;
    uint8_t*        p     = NULL;
    uint16_t      length  = 0;

    length += ( sizeof(wiced_bt_device_address_t));
    /* allocate memory for the data */
    data = (uint8_t*)calloc(length, sizeof(uint8_t));
    p = data;
    BDADDR_TO_STREAM(p, bda);
    wiced_hci_send(HCI_CONTROL_COMMAND_SET_LOCAL_BDA, data, length);
    free(data);
}

void wiced_bt_dev_read_local_addr (wiced_bt_device_address_t bd_addr)
{
    wiced_bt_device_address_t bda = {0,0,0,0,0,0};
    wiced_hci_send( HCI_CONTROL_COMMAND_READ_LOCAL_BDA, bda, sizeof(bda) );
}
