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
 * @file wiced_hci_bt_ble.c
 *
 * This handles the BLE api functions when USE_WICED_HCI is defined.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wiced_hci.h"
#include "cy_result_mw.h"
#include "wiced_defs.h"
#include "wiced_hci_bt_common_internal.h"

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Structures
 ******************************************************/


typedef struct _wiced_hci_bt_ble_context
{
        wiced_hci_cb                  ble_context_cb;
} wiced_hci_bt_ble_context_t;
/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               External Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/
wiced_hci_bt_ble_context_t     wh_bt_ble_context;

/******************************************************
 *               Function Definitions
 ******************************************************/

cy_rslt_t wiced_bt_start_advertisements(wiced_bt_ble_advert_mode_t advert_mode, wiced_bt_ble_address_type_t directed_advertisement_bdaddr_type, wiced_bt_device_address_ptr_t directed_advertisement_bdaddr_ptr)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint8_t        data[2] ;
    uint16_t       length = 1;

    switch(advert_mode)
    {
        case BTM_BLE_ADVERT_OFF:
            data[0] = 1; /* Dont send advertisements */
            length = 1;
            break;

        /** TODO:
         * Require support for the below.
         * As of now, everything will be sent to the lower layer as an advertisement enable.
         */

        case BTM_BLE_ADVERT_DIRECTED_HIGH:
        case BTM_BLE_ADVERT_DIRECTED_LOW:
        case BTM_BLE_ADVERT_UNDIRECTED_HIGH:
        case BTM_BLE_ADVERT_UNDIRECTED_LOW:
        case BTM_BLE_ADVERT_NONCONN_HIGH:
        case BTM_BLE_ADVERT_NONCONN_LOW:
        case BTM_BLE_ADVERT_DISCOVERABLE_HIGH:
        case BTM_BLE_ADVERT_DISCOVERABLE_LOW:
            data[0] = 1;
            break;

        default:
            break;
    }

    wiced_hci_send(HCI_CONTROL_LE_COMMAND_ADVERTISE,
                   &data[0],
                   length);

    return result;
}

cy_rslt_t wiced_bt_ble_set_raw_advertisement_data(UINT8 num_elem,
                                                       wiced_bt_ble_advert_elem_t *p_data)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint8_t data[100] ;
    uint16_t length       = 0;
    uint16_t count        = 0;
    uint16_t point        = 1;

    for ( count = 0; count  < num_elem; count++ )
    {
        length += sizeof(wiced_bt_ble_advert_type_t); /* advert_type */
        length += sizeof(uint16_t);/* length field */
        length += p_data[count].len * sizeof(uint8_t) + sizeof(uint8_t); //adding null char byte
    }

    data[0] = num_elem;

    for ( count = 0; count < num_elem; count++ )
    {
        data[point++] = p_data[count].advert_type;

        data[point++] = (uint8_t)((p_data[count].len & 0xff00)>> 8);
        data[point++] = (p_data[count].len & 0x00ff);

        memcpy(&data[point], p_data[count].p_data, (p_data[count].len)*sizeof(uint8_t) + 1 );
        point += (p_data[count].len)*sizeof(uint8_t) + 1;
    }

    wiced_hci_send(HCI_CONTROL_LE_COMMAND_SET_RAW_ADVERTISE_DATA,
                   data,
                   length);

    WICED_INFO(("[%s %d] done\n",__func__,__LINE__));
    return result;
}

void wiced_bt_ble_security_grant(wiced_bt_device_address_t bd_addr, uint8_t res)
{
    uint8_t data[sizeof(wiced_bt_device_address_t) + sizeof(uint8_t)];
    uint16_t length = sizeof(wiced_bt_device_address_t) + sizeof(uint8_t);

    memcpy(data, bd_addr, sizeof(wiced_bt_device_address_t));
    memcpy(&data[sizeof(wiced_bt_device_address_t)],&res,sizeof(uint8_t));
    wiced_hci_send(HCI_CONTROL_LE_COMMAND_SECURITY_GRANT, data, length);

}
