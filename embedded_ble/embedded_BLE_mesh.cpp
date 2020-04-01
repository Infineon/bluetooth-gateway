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

#include "wiced_hci_bt_mesh.h"

using namespace cypress::embedded;

#define MESH_GATEWAY_INFO( X )         printf X

Mesh* Mesh::gmesh = NULL;


void mesh_status_cb(uint8_t status)
{

    Mesh& mesh = Mesh::getMeshInstance(BLE::Instance());
    Mesh::MeshEventCallback_t callback = mesh.getmeshCallback();

    Mesh::MeshEventCallbackData cb_data;
    cb_data.mesh.status = status;


    if (callback)
    {
        callback(Mesh::BLUETOOTH_MESH_NETWORK_STATUS, &cb_data);
    }
}


// Callback function which notifies provisioning status.
void mesh_provisioning_status_cb(uint32_t conn_id, uint8_t result)
{
    Mesh& mesh = Mesh::getMeshInstance(BLE::Instance());
    Mesh::MeshEventCallback_t callback = mesh.getmeshCallback();

    Mesh::MeshEventCallbackData cb_data;
    cb_data.provisioning.conn_id = conn_id;
    cb_data.provisioning.status = result;

    if (callback)
    {
        callback(Mesh::BLUETOOTH_MESH_DEVICE_PROVISIONING_STATUS, &cb_data);
    }

    MESH_GATEWAY_INFO(("%s Provisioning status callback. status:%02x \n" ,__func__, result));
}

// Callback function which recieves proxy packet from the mesh core , this data should be published to cloud
void mesh_cloud_data_cb(const uint8_t *packet, uint32_t packet_len)
{
    Mesh& mesh = Mesh::getMeshInstance(BLE::Instance());
    Mesh::MeshEventCallback_t callback = mesh.getmeshCallback();

    Mesh::MeshEventCallbackData cb_data;
    cb_data.network.packet = (uint8_t *)packet;
    cb_data.network.length = packet_len;

    if (callback)
    {
        callback(Mesh::BLUETOOTH_MESH_NETWORK_RECEIVED_DATA, &cb_data);
    }

    MESH_GATEWAY_INFO(("%s Proxy Data from Mesh for Cloud. Received length = %lu \n", __func__, packet_len));
}

void mesh_nvram_data_cb(int id, uint8_t *packet, uint32_t packet_len )
{
    Mesh& mesh = Mesh::getMeshInstance(BLE::Instance());
    Mesh::MeshEventCallback_t callback = mesh.getmeshCallback();

    Mesh::MeshEventCallbackData cb_data;
    cb_data.nvram.id = id;
    cb_data.nvram.data = packet;
    cb_data.nvram.length = packet_len;

    if (callback)
    {
        callback(Mesh::BLUETOOTH_MESH_NVRAM_DATA, &cb_data);
    }

    MESH_GATEWAY_INFO(("%s NVRAM data received length = %lu \n", __func__, packet_len));
}

ble_error_t Mesh::initialize(void)
{
    MESH_GATEWAY_INFO(("Initializing Embedded BLE Mesh Service\n"));
    wiced_bt_mesh_init( mesh_provisioning_status_cb,
                        mesh_cloud_data_cb,
                        mesh_nvram_data_cb,
                        mesh_status_cb);

    return BLE_ERROR_NONE;
}

ble_error_t Mesh::shutdown(void)
{
    MESH_GATEWAY_INFO(("Shutting down Embedded BLE Mesh Service\n"));
    return BLE_ERROR_NONE;
}

ble_error_t Mesh::connectMesh(void)
{
    MESH_GATEWAY_INFO(("Connecting to Mesh network...\n"));
    uint8_t mesh_connect_cmd = 0x1;

    wiced_bt_mesh_proxy_connect(mesh_connect_cmd);

    return BLE_ERROR_NONE;
}

ble_error_t Mesh::disconnectMesh(void)
{
    MESH_GATEWAY_INFO(("Disconnecting from Mesh Network...\n"));
    uint8_t mesh_disconnect_cmd = 0x0;

    wiced_bt_mesh_proxy_connect(mesh_disconnect_cmd);

    return BLE_ERROR_NONE;
}

ble_error_t Mesh::pushNVData(uint8_t *data_in , uint8_t data_len , uint16_t idx)
{
    wiced_bt_mesh_push_nvram_data(data_in, data_len, idx);
    return BLE_ERROR_NONE;
}

int Mesh::sendData(uint8_t* p_data, uint8_t data_len)
{
    MESH_GATEWAY_INFO(("Sending Data to Mesh Network\n"));

    wiced_bt_mesh_send_proxy_packet(p_data, data_len);

    return BLE_ERROR_NONE;
}

