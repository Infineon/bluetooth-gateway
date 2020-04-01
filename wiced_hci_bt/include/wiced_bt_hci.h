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

#pragma once

#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint8_t wiced_bt_management_evt_t;

/** Bluetooth Management events */
enum wiced_bt_management_evt_e {
    /* Bluetooth status events */
    BTM_ENABLED_EVT,                                /**< Bluetooth controller and host stack enabled. Event data: wiced_bt_dev_enabled_t */
    BTM_DISABLED_EVT,                               /**< Bluetooth controller and host stack disabled. Event data: NULL */
    BTM_POWER_MANAGEMENT_STATUS_EVT,                /**< Power management status change. Event data: wiced_bt_power_mgmt_notification_t */
    /* Security events */
    BTM_PIN_REQUEST_EVT,                            /**< PIN request (used only with legacy devices). Event data: #wiced_bt_dev_name_and_class_t */
    BTM_USER_CONFIRMATION_REQUEST_EVT,              /**< received USER_CONFIRMATION_REQUEST event (respond using #wiced_bt_dev_confirm_req_reply). Event data: #wiced_bt_dev_user_cfm_req_t */
    BTM_PASSKEY_NOTIFICATION_EVT,                   /**< received USER_PASSKEY_NOTIFY event. Event data: #wiced_bt_dev_user_key_notif_t */
    BTM_PASSKEY_REQUEST_EVT,                        /**< received USER_PASSKEY_REQUEST event (respond using #wiced_bt_dev_pass_key_req_reply). Event data: #wiced_bt_dev_user_key_req_t */
    BTM_KEYPRESS_NOTIFICATION_EVT,                  /**< received KEYPRESS_NOTIFY event. Event data: #wiced_bt_dev_user_keypress_t */
    BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT, /**< Requesting IO capabilities for BR/EDR pairing. Event data: #wiced_bt_dev_bredr_io_caps_req_t */
    BTM_PAIRING_IO_CAPABILITIES_BR_EDR_RESPONSE_EVT,/**< Received IO capabilities response for BR/EDR pairing. Event data: #wiced_bt_dev_bredr_io_caps_rsp_t */
    BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT,    /**< Requesting IO capabilities for BLE pairing. Slave can check peer io capabilities in event data before updating with local io capabilities. Event data: #wiced_bt_dev_ble_io_caps_req_t */
    BTM_PAIRING_COMPLETE_EVT,                       /**< received SIMPLE_PAIRING_COMPLETE event. Event data: #wiced_bt_dev_pairing_cplt_t */
    BTM_ENCRYPTION_STATUS_EVT,                      /**< Encryption status change. Event data: #wiced_bt_dev_encryption_status_t */
    BTM_SECURITY_REQUEST_EVT,                       /**< Security request (respond using #wiced_bt_ble_security_grant). Event data: #wiced_bt_dev_security_request_t */
    BTM_SECURITY_FAILED_EVT,                        /**< Security procedure/authentication failed. Event data: #wiced_bt_dev_security_failed_t */
    BTM_SECURITY_ABORTED_EVT,                       /**< Security procedure aborted locally, or unexpected link drop. Event data: #wiced_bt_dev_name_and_class_t */

    BTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT,           /**< Result of reading local OOB data (wiced_bt_dev_read_local_oob_data). Event data: #wiced_bt_dev_local_oob_t */
    BTM_REMOTE_OOB_DATA_REQUEST_EVT,                /**< OOB data from remote device (respond using #wiced_bt_dev_remote_oob_data_reply). Event data: #wiced_bt_dev_remote_oob_t */

    BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT,         /**< Updated remote device link keys (store device_link_keys to  NV memory). This is the place to
verify that the correct link key has been generated. Event data: #wiced_bt_device_link_keys_t */
    BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT,        /**< Request for stored remote device link keys (restore device_link_keys from NV memory). If successful, return WICED_BT_SUCCESS. Event data: #wiced_bt_device_link_keys_t */

    BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT,             /**< Update local identity key (stored local_identity_keys NV memory). Event data: #wiced_bt_local_identity_keys_t */
    BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT,            /**< Request local identity key (get local_identity_keys from NV memory). If successful, return WICED_BT_SUCCESS. Event data: #wiced_bt_local_identity_keys_t */

    BTM_BLE_SCAN_STATE_CHANGED_EVT,                 /**< BLE scan state change. Event data: #wiced_bt_ble_scan_type_t */
    BTM_BLE_ADVERT_STATE_CHANGED_EVT,               /**< BLE advertisement state change. Event data: #wiced_bt_ble_advert_mode_t */

    /* BLE Secure Connection events */
    BTM_SMP_REMOTE_OOB_DATA_REQUEST_EVT,            /**< SMP remote oob data request. Reply using wiced_bt_smp_oob_data_reply. Event data: wiced_bt_smp_remote_oob_req_t  */
    BTM_SMP_SC_REMOTE_OOB_DATA_REQUEST_EVT,         /**< LE secure connection remote oob data request. Reply using wiced_bt_smp_sc_oob_reply. Event data: #wiced_bt_smp_sc_remote_oob_req_t */
    BTM_SMP_SC_LOCAL_OOB_DATA_NOTIFICATION_EVT,     /**< LE secure connection local OOB data (wiced_bt_smp_create_local_sc_oob_data). Event data: #wiced_bt_smp_sc_local_oob_t*/

    BTM_SCO_CONNECTED_EVT,                          /**< SCO connected event. Event data: #wiced_bt_sco_connected_t */
    BTM_SCO_DISCONNECTED_EVT,                       /**< SCO disconnected event. Event data: #wiced_bt_sco_disconnected_t */
    BTM_SCO_CONNECTION_REQUEST_EVT,                 /**< SCO connection request event. Event data: #wiced_bt_sco_connection_request_t */
    BTM_SCO_CONNECTION_CHANGE_EVT,                  /**< SCO connection change event. Event data: #wiced_bt_sco_connection_change_t */
    BTM_BLE_CONNECTION_PARAM_UPDATE,                /**< BLE connection parameter update. Event data: #wiced_bt_ble_connection_param_update_t */
    BTM_BLE_PHY_UPDATE_EVT,                          /**< BLE Physical link update. Event data: wiced_bt_ble_phy_update_t */
    BTM_LPM_STATE_LOW_POWER,                        /**< BT device wake has been deasserted. Used for Host Stack Use Case. */
};


/** Device enabled (used by BTM_ENABLED_EVT) */
typedef struct {
    cy_rslt_t          status;                     /**< Status */
} wiced_bt_dev_enabled_t;

/** Structure definitions for Bluetooth Management (wiced_bt_management_cback_t) event notifications */
typedef union
{
    /* Bluetooth status event data types*/
    wiced_bt_dev_enabled_t                  enabled;                            /**< Data for BTM_ENABLED_EVT */
} wiced_bt_management_evt_data_t;

/**
 * Bluetooth Management callback
 *
 * Callback for Bluetooth Management event notifications.
 * Registered using wiced_bt_stack_init()
 *
 * @param event             : Event ID
 * @param p_event_data      : Event data
 *
 * @return Status of event handling
 */
typedef cy_rslt_t (wiced_bt_management_cback_t) (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);

#ifdef __cplusplus
} ;
#endif
