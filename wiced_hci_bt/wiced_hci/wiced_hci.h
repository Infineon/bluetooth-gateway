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

#include "cy_result_mw.h"
/** @file
 *
 * HCI Control Protocol Definitions
 *
 * This file provides definitions for HCI Control Interface between an MCU
 * and hci_control application running on cypress BT controllers.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_DEBUG_TRACES
#define WICED_DEBUG( X )        printf X
#else
#define WICED_DEBUG( X )
#endif

#define WICED_INFO( X )         printf X

#define WICED_ERROR( X )        printf X



/* Packets exchanged over the UART between MCU and hci_control application contain 5 byte header
 * -------------------------------------------------------------------------------------------------------
 * |  Packet Type      | Command Code          |    Group Code       |        Packet Length              |
 * -------------------------------------------------------------------------------------------------------
 * |HCI_WICED_PKT(0x19)|HCI_CONTROL_COMMAND_...|HCI_CONTROL_GROUP_...|length(low byte)| length(high byte)|
 * -------------------------------------------------------------------------------------------------------
 */

/******************************************************
 *                   Enumerations
 ******************************************************/
typedef enum control_group_enum {
    DEVICE = 0,
    LE,
    GATT,
    MESH = 0x16,
    MAX_CONTROL_GROUP
} control_group_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/
#define HCI_EVENT_PKT                                       4
#define HCI_ACL_DATA_PKT                                    2
#define HCI_WICED_PKT                                       25

#define CY_TRUE     1
#define CY_FALSE    0
#define BD_ADDR_LEN     6

/*
 * Group codes
 */
#define HCI_CONTROL_GROUP_DEVICE                              0x00
#define HCI_CONTROL_GROUP_LE                                  0x01
#define HCI_CONTROL_GROUP_GATT                                0x02
#define HCI_CONTROL_GROUP_MESH                                0x16
#define HCI_CONTROL_GROUP_MISC                                0xFF

#define HCI_CONTROL_GROUP(x) ((((x) >> 8)) & 0xff)


/*
 * General purpose commands
 */
#define HCI_CONTROL_COMMAND_RESET                           ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x01 )    /* Restart controller */
#define HCI_CONTROL_COMMAND_TRACE_ENABLE                    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x02 )    /* Enable or disable WICED traces */
#define HCI_CONTROL_COMMAND_SET_LOCAL_BDA                   ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x03 )    /* Set local device addrsss */
#define HCI_CONTROL_COMMAND_SET_BAUD_RATE                   ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x04 )    /* Change UART baud rate */
#define HCI_CONTROL_COMMAND_PUSH_NVRAM_DATA                 ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x05 )    /* Download previously saved NVRAM chunk */
#define HCI_CONTROL_COMMAND_DELETE_NVRAM_DATA               ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x06 )    /* Delete NVRAM chunk currently stored in RAM */
#define HCI_CONTROL_COMMAND_INQUIRY                         ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x07 )    /* Start/stop inquiry */
#define HCI_CONTROL_COMMAND_SET_VISIBILITY                  ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x08 )    /* Set BR/EDR connectability and discoverability of the device */
#define HCI_CONTROL_COMMAND_SET_PAIRING_MODE                ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x09 )    /* Set Pairing Mode for the device 0 = Not pairable 1 = Pairable */
#define HCI_CONTROL_COMMAND_UNBOND                          ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0A )    /* Delete bond with specified BDADDR */
#define HCI_CONTROL_COMMAND_USER_CONFIRMATION               ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0B )    /* User Confirmation during pairing, TRUE/FALSE passed as parameter */
#define HCI_CONTROL_COMMAND_ENABLE_COEX                     ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0C )    /* Enable coex functionality */
#define HCI_CONTROL_COMMAND_DISABLE_COEX                    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0D )    /* Disable coex functionality */
#define HCI_CONTROL_COMMAND_SET_BATTERY_LEVEL               ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0E )    /* Sets battery level in the GATT database */
#define HCI_CONTROL_COMMAND_READ_LOCAL_BDA                  ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0F )    /* Get local device addrsss */
#define HCI_CONTROL_COMMAND_BOND                            ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x10 )    /* Initiate Bonding with a peer device */
#define HCI_CONTROL_COMMAND_READ_BUFF_STATS                 ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x11 )    /* Read Buffer statistics */
#define HCI_CONTROL_COMMAND_SET_LOCAL_NAME                  ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x12 )    /* Set the local name     */

/*
 * LE Commands
 * Define commands sent to the GAP/GATT implementation on 20706
 */
#define HCI_CONTROL_LE_COMMAND_SCAN                         ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x01 )    /* start scan */
#define HCI_CONTROL_LE_COMMAND_ADVERTISE                    ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x02 )    /* start advertisements */
#define HCI_CONTROL_LE_COMMAND_CONNECT                      ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x03 )    /* connect to peer */
#define HCI_CONTROL_LE_COMMAND_CANCEL_CONNECT               ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x04 )    /* cancel connect */
#define HCI_CONTROL_LE_COMMAND_DISCONNECT                   ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x05 )    /* disconnect */
#define HCI_CONTROL_LE_RE_PAIR                              ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x06 )    /* delete keys and then re-pair */
#define HCI_CONTROL_LE_COMMAND_GET_IDENTITY_ADDRESS         ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x07 )    /* get identity address */
#define HCI_CONTROL_LE_COMMAND_SET_CHANNEL_CLASSIFICATION   ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x08 )    /* set channel classification for the available 40 channels */
#define HCI_CONTROL_LE_COMMAND_SET_CONN_PARAMS              ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x09 )    /* set connection parameters */
#define HCI_CONTROL_LE_COMMAND_SET_RAW_ADVERTISE_DATA       ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x0a )    /* set raw advertisement data */
#define HCI_CONTROL_LE_COMMAND_SECURITY_GRANT               ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x0b )    /* grant or deny access */

/*
 * GATT Commands
 * Define commands to perform various GATT procedures
 */
#define HCI_CONTROL_GATT_COMMAND_DISCOVER_SERVICES          ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x01 )    /* discover services */
#define HCI_CONTROL_GATT_COMMAND_DISCOVER_CHARACTERISTICS   ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x02 )    /* discover characteristics */
#define HCI_CONTROL_GATT_COMMAND_DISCOVER_DESCRIPTORS       ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x03 )    /* discover descriptors */
#define HCI_CONTROL_GATT_COMMAND_READ_REQUEST               ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x04 )    /* send read request */
#define HCI_CONTROL_GATT_COMMAND_READ_RESPONSE              ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x05 )    /* send read response */
#define HCI_CONTROL_GATT_COMMAND_WRITE_COMMAND              ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x06 )    /* send write command */
#define HCI_CONTROL_GATT_COMMAND_WRITE_REQUEST              ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x07 )    /* send write request */
#define HCI_CONTROL_GATT_COMMAND_WRITE_RESPONSE             ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x08 )    /* send write response */
#define HCI_CONTROL_GATT_COMMAND_NOTIFY                     ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x09 )    /* send notification */
#define HCI_CONTROL_GATT_COMMAND_INDICATE                   ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0a )    /* send indication */
#define HCI_CONTROL_GATT_COMMAND_INDICATE_CONFIRM           ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0b )    /* send indication confirmation */
#define HCI_CONTROL_GATT_COMMAND_REGISTER                   ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0c )    /* register GATT callback */
#define HCI_CONTROL_GATT_COMMAND_DB_INIT                    ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0d )    /* initialize GATT database */

/*
 * Mesh Commands
 */

// Commands added for proxy
#define HCI_CONTROL_MESH_COMMAND_SEND_CONN_STATUS                           ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xf1 )    /* Application sends connect proxy */
#define HCI_CONTROL_MESH_COMMAND_PUSH_NVRAM_DATA                            ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xf3 )    /* Application pushes the nvram data to the mesh core */
#define HCI_CONTROL_MESH_COMMAND_STACK_INIT                                 ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xf4 )    /* */
#define HCI_CONTROL_MESH_COMMAND_CONNECTION_STATE                           ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xf5 )    /*  Application use this interface to send connection state , before it sends proxy data */
#define HCI_CONTROL_MESH_COMMAND_APP_START                                  ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xef )  /* Starts Mesh Application - calls mesh_application_init() and deletes all NVRAM chuncks */
#define HCI_CONTROL_MESH_COMMAND_SEND_PROXY_DATA                            ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xf2 )  /* Mesh Application can send proxy data from MCU */

/*
 * Define general events that controller can send
 */
#define HCI_CONTROL_EVENT_COMMAND_STATUS                    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x01 )    /* Command status event for the requested operation */
#define HCI_CONTROL_EVENT_WICED_TRACE                       ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x02 )    /* WICED trace packet */
#define HCI_CONTROL_EVENT_HCI_TRACE                         ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x03 )    /* Bluetooth protocol trace */
#define HCI_CONTROL_EVENT_NVRAM_DATA                        ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x04 )    /* Request to MCU to save NVRAM chunk */
#define HCI_CONTROL_EVENT_DEVICE_STARTED                    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x05 )    /* Device completed power up initialization */
#define HCI_CONTROL_EVENT_INQUIRY_RESULT                    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x06 )    /* Inquiry result */
#define HCI_CONTROL_EVENT_INQUIRY_COMPLETE                  ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x07 )    /* Inquiry completed event */
#define HCI_CONTROL_EVENT_PAIRING_COMPLETE                  ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x08 )    /* Pairing Completed */
#define HCI_CONTROL_EVENT_ENCRYPTION_CHANGED                ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x09 )    /* Encryption changed event */
#define HCI_CONTROL_EVENT_CONNECTED_DEVICE_NAME             ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0A )    /* Device name event */
#define HCI_CONTROL_EVENT_USER_CONFIRMATION                 ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0B )    /* User Confirmation during pairing */
#define HCI_CONTROL_EVENT_DEVICE_ERROR                      ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0C )    /* Device Error event */
#define HCI_CONTROL_EVENT_READ_LOCAL_BDA                    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0D )    /* Local BDA Read event */
#define HCI_CONTROL_EVENT_MAX_NUM_OF_PAIRED_DEVICES_REACHED ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0E )    /* Key Buffer Pool Full */
#define HCI_CONTROL_EVENT_READ_BUFFER_STATS                 ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x0F )    /* Read Buffer statistics event */
#define HCI_CONTROL_EVENT_UPDATE_LINK_KEY                   ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x10 )    /* Update link key info in the DCT */
#define HCI_CONTROL_EVENT_REQUEST_ID_KEYS                   ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x11 )    /* Request the ID keys */
#define HCI_CONTROL_EVENT_READ_RSSI                         ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x12 )    /* BCM20706 returns the RSSI of the desired link */
#define HCI_CONTROL_EVENT_DEVICE_INIT                       ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x13 )    /* sends a Device Started event at the end of application initialization. */
#define HCI_CONTROL_EVENT_SECURITY_REQ                      ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x14 )    /* The app will respond to this event with a "wiced_bt_ble_security_grant". */
#define HCI_CONTROL_EVENT_SECURITY_FAILED                   ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x15 )    /* Security procedure/authentication failed. */
#define HCI_CONTROL_EVENT_IO_CAPABILITIES_BR_EDR_REQUEST    ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x16 )    /* IO capablities request */
#define HCI_CONTROL_EVENT_KEYPRESS_NOTIFICATION             ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x17 )    /* KeyPress notification */
#define HCI_CONTROL_EVENT_CONNECTION_STATUS                 ( ( HCI_CONTROL_GROUP_DEVICE << 8 ) | 0x18 )    /* Connection Status */
/*
 * Define LE events from the BLE GATT/GAP
 */
#define HCI_CONTROL_LE_EVENT_COMMAND_STATUS                 ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x01 )    /* Command status event for the requested operation */
#define HCI_CONTROL_LE_EVENT_SCAN_STATUS                    ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x02 )    /* LE scanning state change notification */
#define HCI_CONTROL_LE_EVENT_ADVERTISEMENT_REPORT           ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x03 )    /* Advertisement report */
#define HCI_CONTROL_LE_EVENT_ADVERTISEMENT_STATE            ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x04 )    /* LE Advertisement state change notification */
#define HCI_CONTROL_LE_EVENT_CONNECTED                      ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x05 )    /* LE Connection established */
#define HCI_CONTROL_LE_EVENT_DISCONNECTED                   ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x06 )    /* Le Connection Terminated */
#define HCI_CONTROL_LE_EVENT_IDENTITY_ADDRESS               ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x07 )    /* Identity address */
#define HCI_CONTROL_LE_EVENT_PEER_MTU                       ( ( HCI_CONTROL_GROUP_LE << 8 ) | 0x08 )    /* Peer MTU */
/*
 * Define GATT events
 */
#define HCI_CONTROL_GATT_EVENT_COMMAND_STATUS               ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x01 )    /* Command status event for the requested operation */
#define HCI_CONTROL_GATT_EVENT_DISCOVERY_COMPLETE           ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x02 )    /* Discovery requested by host completed */
#define HCI_CONTROL_GATT_EVENT_SERVICE_DISCOVERED           ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x03 )    /* Service discovered */
#define HCI_CONTROL_GATT_EVENT_CHARACTERISTIC_DISCOVERED    ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x04 )    /* Characteristic discovered */
#define HCI_CONTROL_GATT_EVENT_DESCRIPTOR_DISCOVERED        ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x05 )    /* Characteristic descriptor discovered */
#define HCI_CONTROL_GATT_EVENT_READ_REQUEST                 ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x06 )    /* Peer sent Read Request */
#define HCI_CONTROL_GATT_EVENT_READ_RESPONSE                ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x07 )    /* Read response */
#define HCI_CONTROL_GATT_EVENT_WRITE_REQUEST                ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x08 )    /* Peer sent Write Request */
#define HCI_CONTROL_GATT_EVENT_WRITE_RESPONSE               ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x09 )    /* Write operation completed */
#define HCI_CONTROL_GATT_EVENT_INDICATION                   ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0a )    /* indication from peer */
#define HCI_CONTROL_GATT_EVENT_NOTIFICATION                 ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0b )    /* notification from peer */
#define HCI_CONTROL_GATT_EVENT_READ_ERROR                   ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0c )    /* GATT Read operation error */
#define HCI_CONTROL_GATT_EVENT_WRITE_ERROR                  ( ( HCI_CONTROL_GROUP_GATT << 8 ) | 0x0d )    /* GATT Write operation error */


/*
 * Mesh events
 */
// Events added for Gateway Mesh Proxy
#define HCI_CONTROL_MESH_EVENT_CORE_PROVISION_END                           ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xb0 )  /*  */
#define HCI_CONTROL_MESH_EVENT_NVRAM_DATA                                   ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xfe )  /*  */
#define HCI_CONTROL_MESH_EVENT_MESH_STATUS                                  ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xb2 )  /*  */
#define HCI_CONTROL_MESH_EVENT_PROVISIONING_STATUS                          ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xfd )  /*  */
#define HCI_CONTROL_MESH_EVENT_PROXY_CONNECTION_STATUS                      ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xb4 )  /*  */
#define HCI_CONTROL_MESH_EVENT_PROXY_DATA                                   ( ( HCI_CONTROL_GROUP_MESH << 8 ) | 0xb9 )  /* Event to send proxy data to MCU application */


#define HCI_CONTROL_MISC_EVENT_PING_REPLY                   ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x01 )    /* Ping reply */
#define HCI_CONTROL_MISC_EVENT_VERSION                      ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x02 )    /* SDK Version */

/*
 * Define Scan state that is reported with the HCI_CONTROL_LE_EVENT_SCAN_STATUS
 */
#define HCI_CONTROL_SCAN_EVENT_NO_SCAN                      0
#define HCI_CONTROL_SCAN_EVENT_HIGH_SCAN                    1
#define HCI_CONTROL_SCAN_EVENT_LOW_SCAN                     2
#define HCI_CONTROL_SCAN_EVENT_HIGH_CONN                    3
#define HCI_CONTROL_SCAN_EVENT_LOW_CONN                     4

/*
 * Define status code returned in HCI_CONTROL_EVENT_COMMAND_STATUS
 */
#define HCI_CONTROL_STATUS_SUCCESS                          0
#define HCI_CONTROL_STATUS_IN_PROGRESS                      1
#define HCI_CONTROL_STATUS_ALREADY_CONNECTED                2
#define HCI_CONTROL_STATUS_NOT_CONNECTED                    3
#define HCI_CONTROL_STATUS_BAD_HANDLE                       4
#define HCI_CONTROL_STATUS_WRONG_STATE                      5
#define HCI_CONTROL_STATUS_INVALID_ARGS                     6
#define HCI_CONTROL_STATUS_FAILED                           7
#define HCI_CONTROL_STATUS_UNKNOWN_GROUP                    8
#define HCI_CONTROL_STATUS_UNKNOWN_COMMAND                  9
#define HCI_CONTROL_STATUS_CLIENT_NOT_REGISTERED            10
#define HCI_CONTROL_STATUS_OUT_OF_MEMORY                    11

typedef void (*wiced_hci_cb)(uint16_t command, uint8_t* payload, uint32_t len);

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
cy_rslt_t wiced_hci_up();
cy_rslt_t wiced_hci_down(void);
cy_rslt_t wiced_hci_set_event_callback(control_group_t group, wiced_hci_cb evt_cb);

/**
 * Send data over the wiced_hci interface.
 *
 * @param opcode The operation code as above for commands.
 * @param data   The data to be send as per opcode
 * @param length The length of the data being sent.
 * @return WICED_SUCCESS if the operation succeeded
 *         WICED_ERROR   if the operation failed.
 */
void wiced_hci_send(uint32_t opcode, uint8_t* data, uint16_t length);
cy_rslt_t wiced_hci_configure(wiced_hci_cb rx_cb);


#ifdef __cplusplus
} /* extern C */
#endif
