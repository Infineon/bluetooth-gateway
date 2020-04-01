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
/*
 * @file
 *
 * Embedded Bluetooth Mesh Applicaiton service
 *
 * - Starting the Application's Bluetooth Mesh service
 * - Stopping/Resetting the Bluetooth Mesh application
 * - Transfer Bluetooth Mesh application data. For example - From Cloud ----> BT Mesh Network and vice-versa.
 */

#pragma once

#include <stdint.h>
#include "embedded_BLE.h"

#include "wiced_hci_bt_mesh.h"
/**
 * \defgroup embedded_ble_mesh Embedded BLE Mesh library
 * @ingroup embedded_ble
 * \defgroup mesh_enums Embedded BLE Mesh library enumerations
 * @ingroup embedded_ble_mesh
 */
namespace cypress
{
namespace embedded
{

/**
 * @addtogroup embedded_ble_mesh
 *
 * @{
 */

/** Defines Bluetooth Mesh class - targetting only Cypress' embedded Bluetooth stack */
class Mesh
{
public:
    /**
     * @addtogroup mesh_enums
     * @{
     *
     */

    /** Defines Bluetooth Mesh Provisioning status values */
    enum BluetoothMeshProvisioningStatus
    {
        BLUETOOTH_MESH_DEVICE_UNPROVISIONED,    /**< Device is not provisioned to any Bluetooth Mesh network */
        BLUETOOTH_MESH_DEVICE_PROVISIONED,      /**< Device is provisioned */
    };

    /** Defines Bluetooth Mesh Connection status values */
    enum BluetoothMeshConnectionStatus
    {
        BLUETOOTH_MESH_NETWORK_CONNECTED,       /**< Device is connected to a Mesh network */
        BLUETOOTH_MESH_NETWORK_DISCONNECTED,    /**< Device is not connected to Mesh network */
    };

    /** Defines Bluetooth Mesh Event values */
    enum BluetoothMeshEvent
    {
        BLUETOOTH_MESH_DEVICE_PROVISIONING_STATUS,  /**< Provisioning status of Bluetooth device */
        BLUETOOTH_MESH_DEVICE_STATUS,               /**< Generic Mesh status of Bluetooth device */
        BLUETOOTH_MESH_NETWORK_RECEIVED_DATA,       /**< Data received from Mesh network to be sent to cloud */
        BLUETOOTH_MESH_NETWORK_STATUS,              /**< Mesh Network status change */
        BLUETOOTH_MESH_NVRAM_DATA,                  /**< Update NVRAM data */
    };
    /** @} */

    /** Defines Embedded BLE Mesh EVent callback payload */
    typedef union MeshEventCallbackData_
    {
        /** provisioning status callback data */
        struct
        {
            uint32_t conn_id;       /**< Mesh Connection ID */
            uint8_t  status;        /**< Mesh Provisioning status */
        } provisioning;

        /** Generic Device/Mesh status callback data */
        struct
        {
            uint8_t device;         /**< Mesh status of Bluetooth device */
        } device;

        /** Data received from Mesh network to be sent to cloud */
        struct
        {
            uint32_t length;        /**< Length of payload received from Mesh network */
            uint8_t* packet;        /**< Mesh payload */
        } network;

        /** NVRAM payload */
        struct
        {
            uint16_t id;            /**< NVRAM Payload index */
            uint32_t length;        /**< NVRAM payload length */
            uint8_t* data;          /**< NVRAM Payload */
        } nvram;

        /** Mesh network status callback data */
        struct
        {
            uint8_t status;         /**< Mesh Network status */
        } mesh;
    }MeshEventCallbackData;

    /** Defines Embedded BLE Mesh EVent callback Function */

    typedef void (*MeshEventCallback_t)(BluetoothMeshEvent event, MeshEventCallbackData* payload);

public:
    /**
     * Get Mesh Instance
     */
    static Mesh& getMeshInstance(BLE& ble)
    {
        if (gmesh == NULL)
        {
            gmesh = new Mesh;
        }

        return (Mesh&)*gmesh;
    }

    /**
     * Starts the Mesh Application on Bluetooth Controller and Wiced HCI serveice on Host
     */
    ble_error_t initialize(void);

    /**
     * Stops Mesh Application on Bluetooth Controller and Wiced HCI service on Host.
     */
    ble_error_t shutdown(void);

    /**
     * Connect to nearby Mesh Network(connects to the mesh network depending on Network-key the device was Provisioned with)
     */
    ble_error_t connectMesh();

    /**
     * Disconnect from Mesh Network
     */
    ble_error_t disconnectMesh();

    /**
     * Push saved NVRAM data to Bluetooth Controller.
     * For example - saved pairing keys, mesh-provisioning data, saved name, address etc.
     */
    ble_error_t pushNVData(uint8_t *data_in , uint8_t data_len , uint16_t idx);

    /**
     * Downstream the packet received from Cloud to Mesh Network.
     */
    int sendData(uint8_t* p_data, uint8_t data_len);

    /**
     * Getter for Device's Provisioning state
     */
    BluetoothMeshProvisioningStatus getProvisionedState(void)
    {
        return isProvisioned;
    }

    /**
     * Getter for Connection state of the Device - Connected to Mesh network or not.
     */
    BluetoothMeshConnectionStatus getConnectionState(void)
    {
        return isConnected;
    }

    /**
     * Register Mesh Event Callback - TODO: Try using Mbed-callback Class for this.
     */
    ble_error_t registerMeshEventcallback(MeshEventCallback_t cb)
    {
        if (!cb)
        {
            return BLE_ERROR_INVALID_PARAM;
        }
        mesh_callback = cb;

        return BLE_ERROR_NONE;
    }

    /**
     * Returns Mesh Event callback registered with the Mesh Object.
     */
    inline MeshEventCallback_t getmeshCallback(void)
    {
        return mesh_callback;
    }

private:

    BluetoothMeshConnectionStatus isConnected;
    BluetoothMeshProvisioningStatus isProvisioned;
    MeshEventCallback_t mesh_callback;

    // Private so that it can  not be called
    Mesh():isConnected(BLUETOOTH_MESH_NETWORK_DISCONNECTED),isProvisioned(BLUETOOTH_MESH_DEVICE_UNPROVISIONED),mesh_callback(NULL)
    {};
    Mesh(Mesh const&): isConnected(BLUETOOTH_MESH_NETWORK_DISCONNECTED),isProvisioned(BLUETOOTH_MESH_DEVICE_UNPROVISIONED),mesh_callback(NULL){};            // copy constructor is private
    Mesh& operator=(Mesh const&);   // assignment operator is private
    static Mesh* gmesh;
};
/** @} */
}

}
