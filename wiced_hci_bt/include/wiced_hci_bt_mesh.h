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

#ifdef __cplusplus
extern "C"
{
#endif

#include "cy_result.h"
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


/******************************************************
 *               External Function Declarations
 ******************************************************/
/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
/**
 * \brief Definition of the callback function to send proxy packet.
 * \details Application implements that function if it want to use
 * GATT notification or even external function (for example MeshController).
 * Called by core to send packet to the proxy client.
 *
 * @param[in]   packet          :Packet to send
 * @param[in]   packet_len      :Length of the packet to send
 *
 * @return      None
 */
typedef void(*wiced_bt_mesh_core_gatt_send_cb_t)(const uint8_t *packet, uint32_t packet_len);

/**
 * @anchor BT_MESH_PROVISION_RESULT
 * @name Provisioning Result codes
 * \details The following is the list of Provisioning Result codes that a provisioning layer can return in wiced_bt_mesh_provision_end_cb_t().
 * @{
 */
#define WICED_BT_MESH_PROVISION_RESULT_SUCCESS   0   /**< Provisioning succeeded */
#define WICED_BT_MESH_PROVISION_RESULT_TIMEOUT   1   /**< Provisioning failed due to timeout */
#define WICED_BT_MESH_PROVISION_RESULT_FAILED    2   /**< Provisioning  failed */

/**
 * \brief Definition of the callback function on provisioning end.
 * \details Provisioner or/and provisioning application implements that function to be called on successfull or failed end of provisioning.
 *
 * @param[in]   conn_id     :Connection ID of the provisioning connection
 * @param[in]   result      ::Provisioning Result code (see @ref BT_MESH_PROVISION_RESULT "Provisioning Result codes")
 *
 * @return   None
 */
typedef void (*wiced_bt_mesh_provision_end_cb_t)(uint32_t  conn_id, uint8_t   result);

typedef void (*wiced_bt_mesh_write_nvram_data_cb_t) (int id, uint8_t *payload, uint32_t payload_len);

/**
 * \brief Definition of the callback function of mesh status
 * \details Application implements the function to know the current status of the mesh.
 *
  * @param[in]   status     :: Mesh status
 *
 * @return   None
 */
typedef void (*wiced_bt_mesh_status_cb_t)(uint8_t status);

/**
 * Function         wiced_bt_mesh_init
 *
 *                  Initialize the Mesh stack; register
 *                  callback for Bluetooth event notification.
 *
 * @param[in] prov_end_cb           : Callback for receiving provisioning status
 * @param[in] proxy_data_cb         : Callback for receiving proxy data
 * @param[in] write_nvram_data_cb   : Callback for receiving nvram data
 * @param[in] mesh_status_cb        : Callback for recieving the mesh status
 *
 * @return cy_rslt_t                : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
cy_rslt_t wiced_bt_mesh_init(wiced_bt_mesh_provision_end_cb_t prov_end_cb,wiced_bt_mesh_core_gatt_send_cb_t proxy_data_cb, wiced_bt_mesh_write_nvram_data_cb_t write_nvram_data_cb, wiced_bt_mesh_status_cb_t mesh_status_cb);

/**
 * Function         wiced_bt_mesh_send_proxy_packet
 *
 *                  send proxy packet to the proxy interface
 *
 * @param[in] p_data                : proxy packet
 * @param[in] data_lem              : proxy data length
 *
 * @return cy_rslt_t                : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
cy_rslt_t wiced_bt_mesh_send_proxy_packet(uint8_t* p_data, uint8_t data_len);


/**
 * Function         wiced_bt_mesh_proxy_connect
 *
 *                  request proxy connection to the mesh stack.
 *
 * @param[in] connection_state      : 1 - for connect , and 0 - for disconnect
 *
 * @return cy_rslt_t                : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
cy_rslt_t wiced_bt_mesh_proxy_connect(uint8_t connection_state);

/**
 * Function         wiced_bt_mesh_push_nvram_data
 *
 *                  push the stored mesh nvram data
 *
 * @param[in] data                  : nvram data
 * @param[in] data_len              : data length
 * @param[in] idx                   : index
 *
 * @return cy_rslt_t                : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
cy_rslt_t wiced_bt_mesh_push_nvram_data(uint8_t *data , uint8_t data_len , uint16_t idx);


#ifdef __cplusplus
} /*extern "C" */
#endif

