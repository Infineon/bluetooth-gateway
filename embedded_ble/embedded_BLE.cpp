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

#include <stdio.h>
#include "embedded_BLE_mesh.h"
#include "embedded_BLE.h"
#include "embedded_GAP.h"

#include "wiced_hci_bt_dm.h"
#include "cy_result.h"

using namespace cypress::embedded;

BLE* BLE::Eble = NULL;

cy_rslt_t embedded_bluetooth_event_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    printf("%s (event: %x)\n",__func__, event );

    BLE& ble = BLE::Instance();

    ble_init_callback_t callback = ble.get_initCallback();

    switch( event )
    {
    /* Bluetooth stack enabled */
    case BTM_ENABLED_EVT:
        ble.setInitialized();
        if (callback)
            callback();
        break;

    case BTM_DISABLED_EVT:
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        break;

    case BTM_SECURITY_REQUEST_EVT:
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
        break;

    case BTM_PASSKEY_REQUEST_EVT:
    case BTM_SMP_REMOTE_OOB_DATA_REQUEST_EVT:
    case BTM_USER_CONFIRMATION_REQUEST_EVT:
    case BTM_SMP_SC_LOCAL_OOB_DATA_NOTIFICATION_EVT:
    case BTM_SMP_SC_REMOTE_OOB_DATA_REQUEST_EVT:
        break;

    default:
        printf("Unhandled Bluetooth Stack Callback event :%d\n", event);
        break;
    }

    return result;
}

ble_error_t BLE::init(void (*callback)(void))
{
    printf("Initializing Embedded BLE\n");
    init_callback = callback;

    wiced_bt_stack_init(embedded_bluetooth_event_callback);
    return BLE_ERROR_NONE;
}

ble_error_t BLE::shutdown(void)
{
    printf("Shutting down Embedded BLE\n");
    initialized = 0;
    init_callback = 0;
    return BLE_ERROR_NONE;
}

ble_error_t BLE::reset(void)
{
    printf("Resetting Embedded BLE\n");
    return BLE_ERROR_NONE;
}

Mesh& BLE::mesh(void)
{
    if (!this->initialized)
    {
        printf("[Warning] BLE Instance has not been initialized\n");
    }

    return Mesh::getMeshInstance((BLE &)*Eble);
}

Gap& BLE::gap(void)
{
    if (!this->initialized)
    {
        printf("[Warning] BLE Instance has not been initialized\n");
    }

    return Gap::getGapInstance();
}
