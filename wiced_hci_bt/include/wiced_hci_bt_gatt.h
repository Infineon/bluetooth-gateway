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
#include "cy_result.h"
#include "wiced_bt_hci.h"
#include "wiced_defs.h"

#define HCI_ERR_CONNECTION_TOUT                         0x08
#define HCI_ERR_PEER_USER                               0x13
#define HCI_ERR_CONN_CAUSE_LOCAL_HOST                   0x16
#define HCI_ERR_CONN_FAILED_ESTABLISHMENT               0x3E
#define HCI_ERR_LMP_RESPONSE_TIMEOUT                    0x22
#define L2CAP_CONN_CANCEL                               256
#define MAX_UUID_SIZE                                   16  /**< Maximum UUID size - 16 bytes, and structure to hold any type of UUID. */

/** UUID Type */
typedef struct
{
#define LEN_UUID_16     2
#define LEN_UUID_32     4
#define LEN_UUID_128    16

    uint16_t        len;     /**< UUID length */

    union
    {
        uint16_t    uuid16; /**< 16-bit UUID */
        uint32_t    uuid32; /**< 32-bit UUID */
        uint8_t     uuid128[MAX_UUID_SIZE]; /**< 128-bit UUID */
    } uu;

} wiced_bt_uuid_t;



/** GATT Status Codes*/
enum wiced_bt_gatt_status_e
{
    WICED_BT_GATT_SUCCESS                    = 0x00,         /**< Success */
    WICED_BT_GATT_INVALID_HANDLE             = 0x01,         /**< Invalid Handle */
    WICED_BT_GATT_READ_NOT_PERMIT            = 0x02,         /**< Read Not Permitted */
    WICED_BT_GATT_WRITE_NOT_PERMIT           = 0x03,         /**< Write Not permitted */
    WICED_BT_GATT_INVALID_PDU                = 0x04,         /**< Invalid PDU */
    WICED_BT_GATT_INSUF_AUTHENTICATION       = 0x05,         /**< Insufficient Authentication */
    WICED_BT_GATT_REQ_NOT_SUPPORTED          = 0x06,         /**< Request Not Supported */
    WICED_BT_GATT_INVALID_OFFSET             = 0x07,         /**< Invalid Offset */
    WICED_BT_GATT_INSUF_AUTHORIZATION        = 0x08,         /**< Insufficient Authorization */
    WICED_BT_GATT_PREPARE_Q_FULL             = 0x09,         /**< Prepare Queue Full */
    WICED_BT_GATT_NOT_FOUND                  = 0x0a,         /**< Not Found */
    WICED_BT_GATT_NOT_LONG                   = 0x0b,         /**< Not Long Size */
    WICED_BT_GATT_INSUF_KEY_SIZE             = 0x0c,         /**< Insufficient Key Size */
    WICED_BT_GATT_INVALID_ATTR_LEN           = 0x0d,         /**< Invalid Attribute Length */
    WICED_BT_GATT_ERR_UNLIKELY               = 0x0e,         /**< Error Unlikely */
    WICED_BT_GATT_INSUF_ENCRYPTION           = 0x0f,         /**< Insufficient Encryption */
    WICED_BT_GATT_UNSUPPORT_GRP_TYPE         = 0x10,         /**< Unsupported Group Type */
    WICED_BT_GATT_INSUF_RESOURCE             = 0x11,         /**< Insufficient Resource */

    WICED_BT_GATT_ILLEGAL_PARAMETER          = 0x87,         /**< Illegal Parameter */
    WICED_BT_GATT_NO_RESOURCES               = 0x80,         /**< No Resources */
    WICED_BT_GATT_INTERNAL_ERROR             = 0x81,         /**< Internal Error */
    WICED_BT_GATT_WRONG_STATE                = 0x82,         /**< Wrong State */
    WICED_BT_GATT_DB_FULL                    = 0x83,         /**< DB Full */
    WICED_BT_GATT_BUSY                       = 0x84,         /**< Busy */
    WICED_BT_GATT_ERROR                      = 0x85,         /**< Error */
    WICED_BT_GATT_CMD_STARTED                = 0x86,         /**< Command Started */
    WICED_BT_GATT_PENDING                    = 0x88,         /**< Pending */
    WICED_BT_GATT_AUTH_FAIL                  = 0x89,         /**< Authentication Fail */
    WICED_BT_GATT_MORE                       = 0x8a,         /**< More */
    WICED_BT_GATT_INVALID_CFG                = 0x8b,         /**< Invalid Configuration */
    WICED_BT_GATT_SERVICE_STARTED            = 0x8c,         /**< Service Started */
    WICED_BT_GATT_ENCRYPED_MITM              = WICED_BT_GATT_SUCCESS, /**< Encrypted MITM */
    WICED_BT_GATT_ENCRYPED_NO_MITM           = 0x8d,         /**< Encrypted No MITM */
    WICED_BT_GATT_NOT_ENCRYPTED              = 0x8e,         /**< Not Encrypted */
    WICED_BT_GATT_CONGESTED                  = 0x8f,         /**< Congested */

                                                    /* 0xE0 ~ 0xFC reserved for future use */
    WICED_BT_GATT_CCC_CFG_ERR                = 0xFD,         /**< Improper Client Char Configuration */
    WICED_BT_GATT_PRC_IN_PROGRESS            = 0xFE,         /**< Procedure Already in Progress */
    WICED_BT_GATT_OUT_OF_RANGE               = 0xFF,         /**< Value Out of Range */
};
typedef uint8_t wiced_bt_gatt_status_t;     /**< GATT status (see #wiced_bt_gatt_status_e) */


/** GATT Operation Codes */
#define  GATT_RSP_ERROR                  0x01       /**< Error Response */
#define  GATT_REQ_MTU                    0x02       /**< Exchange MTU Request */
#define  GATT_RSP_MTU                    0x03       /**< Exchange MTU Response */
#define  GATT_REQ_FIND_INFO              0x04       /**< Find Information Request */
#define  GATT_RSP_FIND_INFO              0x05       /**< Find Information Response */
#define  GATT_REQ_FIND_TYPE_VALUE        0x06       /**< Find By Type Value Request */
#define  GATT_RSP_FIND_TYPE_VALUE        0x07       /**< Find By Type Value Response */
#define  GATT_REQ_READ_BY_TYPE           0x08       /**< Read By Type Request */
#define  GATT_RSP_READ_BY_TYPE           0x09       /**< Read By Type Response */
#define  GATT_REQ_READ                   0x0A       /**< Read Request */
#define  GATT_RSP_READ                   0x0B       /**< Read Response */
#define  GATT_REQ_READ_BLOB              0x0C       /**< Read Blob Request */
#define  GATT_RSP_READ_BLOB              0x0D       /**< Read Blob Response */
#define  GATT_REQ_READ_MULTI             0x0E       /**< Read Multiple Request */
#define  GATT_RSP_READ_MULTI             0x0F       /**< Read Multiple Response */
#define  GATT_REQ_READ_BY_GRP_TYPE       0x10       /**< Read By Group Type Request */
#define  GATT_RSP_READ_BY_GRP_TYPE       0x11       /**< Read By Group Type Response */
#define  GATT_REQ_WRITE                  0x12       /**< Write Request */
#define  GATT_RSP_WRITE                  0x13       /**< Write Request */
#define  GATT_CMD_WRITE                  0x52       /**< Write Command */
#define  GATT_REQ_PREPARE_WRITE          0x16       /**< Prepare Write Request */
#define  GATT_RSP_PREPARE_WRITE          0x17       /**< Prepare Write Response */
#define  GATT_REQ_EXEC_WRITE             0x18       /**< Execute Write Request */
#define  GATT_RSP_EXEC_WRITE             0x19       /**< Execute Write Response */
#define  GATT_HANDLE_VALUE_NOTIF         0x1B       /**< Handle Value Notification */
#define  GATT_HANDLE_VALUE_IND           0x1D       /**< Handle Value Indication */
#define  GATT_HANDLE_VALUE_CONF          0x1E       /**< Handle Value Confirmation */
#define  GATT_OP_CODE_MAX                (GATT_HANDLE_VALUE_CONF + 1) /**<  Maximum opcode value */
#define  GATT_SIGN_CMD_WRITE             0xD2       /**< changed in V4.0 1101-0010 (signed write)  see write cmd above*/


/**  GATT Characteristic Properties Mask */
enum wiced_bt_gatt_char_properties_e {
    GATT_CHAR_PROPERTIES_BIT_BROADCAST      = (1 << 0),     /**< bit 0: Broadcast */
    GATT_CHAR_PROPERTIES_BIT_READ           = (1 << 1),     /**< bit 1: Read */
    GATT_CHAR_PROPERTIES_BIT_WRITE_NR       = (1 << 2),     /**< bit 2: Write (No Response) */
    GATT_CHAR_PROPERTIES_BIT_WRITE          = (1 << 3),     /**< bit 3: Write */
    GATT_CHAR_PROPERTIES_BIT_NOTIFY         = (1 << 4),     /**< bit 4: Notify */
    GATT_CHAR_PROPERTIES_BIT_INDICATE       = (1 << 5),     /**< bit 5: Indicate */
    GATT_CHAR_PROPERTIES_BIT_AUTH           = (1 << 6),     /**< bit 6: Authenticate */
    GATT_CHAR_PROPERTIES_BIT_EXT_PROP       = (1 << 7)      /**< bit 7: Extended Properties */
};
typedef uint8_t wiced_bt_gatt_char_properties_t;            /**< GATT characteristic properties mask (see #wiced_bt_gatt_char_properties_e) */

/**  GATT Disconnection reason */
enum wiced_bt_gatt_disconn_reason_e {
    GATT_CONN_UNKNOWN                       = 0,                                    /**< Unknown reason */
    GATT_CONN_L2C_FAILURE                   = 1,                                    /**< General L2cap failure  */
    GATT_CONN_TIMEOUT                       = HCI_ERR_CONNECTION_TOUT,              /**< Connection timeout  */
    GATT_CONN_TERMINATE_PEER_USER           = HCI_ERR_PEER_USER,                    /**< Connection terminated by peer user  */
    GATT_CONN_TERMINATE_LOCAL_HOST          = HCI_ERR_CONN_CAUSE_LOCAL_HOST,        /**< Connection terminated by local host  */
    GATT_CONN_FAIL_ESTABLISH                = HCI_ERR_CONN_FAILED_ESTABLISHMENT,    /**< Connection fail to establish  */
    GATT_CONN_LMP_TIMEOUT                   = HCI_ERR_LMP_RESPONSE_TIMEOUT,         /**< Connection fail due to LMP response tout */
    GATT_CONN_CANCEL                        = L2CAP_CONN_CANCEL                     /**< L2CAP connection cancelled  */
};
typedef uint16_t wiced_bt_gatt_disconn_reason_t;    /**< GATT disconnection reason (see #wiced_bt_gatt_disconn_reason_e) */

/* default GATT MTU size over LE link
*/
#define GATT_DEF_BLE_MTU_SIZE       23

/* invalid connection ID
*/
#define GATT_INVALID_CONN_ID        0xFFFF


/** characteristic descriptor: client configuration value */
enum wiced_bt_gatt_client_char_config_e
{
    GATT_CLIENT_CONFIG_NONE          = 0x0000,      /**< Does not allow both notifications and indications */
    GATT_CLIENT_CONFIG_NOTIFICATION  = 0x0001,      /**< Allows notifications  */
    GATT_CLIENT_CONFIG_INDICATION    = 0x0002       /**< Allows indications  */
};
typedef uint16_t wiced_bt_gatt_client_char_config_t;     /**< GATT client config (see #wiced_bt_gatt_client_char_config_e) */

/** Authentication requirement */
enum wiced_bt_gatt_auth_req_e {
    GATT_AUTH_REQ_NONE                  = 0,    /**< No Authentication Required */
    GATT_AUTH_REQ_NO_MITM               = 1,    /**< Unauthenticated encryption (No MITM) */
    GATT_AUTH_REQ_MITM                  = 2,    /**< Authenticated encryption (MITM) */
    GATT_AUTH_REQ_SIGNED_NO_MITM        = 3,    /**< Signed Data (No MITM) */
    GATT_AUTH_REQ_SIGNED_MITM           = 4     /**< Signed Data (MITM) */
};
typedef uint8_t wiced_bt_gatt_auth_req_t;   /**< GATT authentication requirement (see #wiced_bt_gatt_auth_req_e)*/

/** Attribute value, used for GATT write operations, and read response callbacks */
typedef struct
{
    uint16_t                    handle;                     /**< Attribute handle */
    uint16_t                    offset;                     /**< Attribute value offset, ignored if not needed for a command */
    uint16_t                    len;                        /**< Length of attribute value */
    wiced_bt_gatt_auth_req_t    auth_req;                   /**< Authentication requirement (see @link wiced_bt_gatt_auth_req_e wiced_bt_gatt_auth_req_t @endlink) */
    uint8_t                     value[1];                   /**< The attribute value (actual length is specified by 'len') */
} wiced_bt_gatt_value_t;

/* Defines the macro to get the GATT Response Structure Size. Substracting 1 to take care of size of the placeholder array which holds the start of the attribute value */
#define GATT_RESPONSE_SIZE( attr_val_len ) ( sizeof( wiced_bt_gatt_value_t ) - 1 + attr_val_len )

/** GATT Write Execute request flags */
enum wiced_bt_gatt_exec_flag_e {
    GATT_PREP_WRITE_CANCEL      = 0x00,         /**< GATT_PREP_WRITE_CANCEL */
    GATT_PREP_WRITE_EXEC        = 0x01          /**< GATT_PREP_WRITE_EXEC */
};
typedef uint8_t   wiced_bt_gatt_exec_flag_t;    /**< GATT execute flag (see #wiced_bt_gatt_exec_flag_e) */

/** Attribute read request */
typedef struct
{
    uint16_t        handle;     /**< Handle of attribute to read */
    uint16_t        offset;     /**< Offset to read */
    BOOLEAN         is_long;    /**< TRUE if long read */
    uint16_t        *p_val_len; /**< input and output parameter for value length */
    uint8_t         *p_val;     /**< Value pointer */
} wiced_bt_gatt_read_t;

/** Attribute write request */
typedef struct
{
    uint16_t      handle;     /**< Handle of attribute to write */
    BOOLEAN       is_prep;    /**< TRUE if this is a prepare write request */
    uint16_t      offset;     /**< Offset to write */
    uint16_t      val_len;    /**< Value length */
    uint8_t       *p_val;     /**< Value pointer */
} wiced_bt_gatt_write_t;

/** Attribute information for GATT attribute requests */
typedef union
{
    wiced_bt_gatt_read_t            read_req;                       /**< Parameters for GATTS_REQ_TYPE_READ */
    wiced_bt_gatt_write_t           write_req;                      /**< Parameters for GATTS_REQ_TYPE_WRITE */
    uint16_t                        handle;                         /**< Parameters for GATTS_REQ_TYPE_CONF */
    uint16_t                        mtu;                            /**< Parameters for GATTS_REQ_TYPE_MTU */
    wiced_bt_gatt_exec_flag_t       exec_write;                     /**< Parameters for GATTS_REQ_TYPE_WRITE_EXEC */
} wiced_bt_gatt_request_data_t;

/** GATT Attribute Request Type */
enum wiced_bt_gatt_request_type_e
{
    GATTS_REQ_TYPE_READ = 1,        /**< Attribute read notification (attribute value internally read from GATT database) */
    GATTS_REQ_TYPE_WRITE,           /**< Attribute write notification (attribute value internally written to GATT database) */
    GATTS_REQ_TYPE_PREP_WRITE,      /**< Attribute Prepare Write Notification (Suspending write request before triggering actual execute write ) */
    GATTS_REQ_TYPE_WRITE_EXEC,      /**< Execute write request */
    GATTS_REQ_TYPE_MTU,             /**< MTU exchange information */
    GATTS_REQ_TYPE_CONF,            /**< Value confirmation */
};
typedef uint8_t wiced_bt_gatt_request_type_t;   /**< GATT Attribute Request Type (see #wiced_bt_gatt_request_type_e) */

/** Discovery types */
enum wiced_bt_gatt_discovery_type_e
{
    GATT_DISCOVER_SERVICES_ALL = 1,             /**< discover all services */
    GATT_DISCOVER_SERVICES_BY_UUID,             /**< discover service by UUID */
    GATT_DISCOVER_INCLUDED_SERVICES,            /**< discover an included service within a service */
    GATT_DISCOVER_CHARACTERISTICS,              /**< discover characteristics of a service with/without type requirement */
    GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS,   /**< discover characteristic descriptors of a character */
    GATT_DISCOVER_MAX                           /* maximum discovery types */
};
typedef uint8_t wiced_bt_gatt_discovery_type_t;    /**< GATT Discovery type (see #wiced_bt_gatt_discovery_type_e) */

/** Parameters used in a GATT Discovery */
typedef struct
{
    wiced_bt_uuid_t uuid;        /**< Service or Characteristic UUID */
    uint16_t        s_handle;    /**< Start handle for range to search */
    uint16_t        e_handle;    /**< End handle for range to search */
}wiced_bt_gatt_discovery_param_t;

/** GATT Read Types */
enum wiced_bt_gatt_read_type_e
{
    GATT_READ_BY_TYPE = 1,      /**< Read by Type (service or characteristic UUIDs) */
    GATT_READ_BY_HANDLE,        /**< Read by Handle */
    GATT_READ_MULTIPLE,         /**< Read Multiple (array of handles) */
    GATT_READ_CHAR_VALUE,       /**< Read Characteristic Value */
    GATT_READ_PARTIAL,          /**< Read Partial */
    GATT_READ_MAX
};
typedef uint8_t wiced_bt_gatt_read_type_t;      /**< GATT read type (see #wiced_bt_gatt_read_type_e) */

/** Parameters for GATT_READ_BY_TYPE and GATT_READ_CHAR_VALUE */
typedef struct
{
    wiced_bt_gatt_auth_req_t    auth_req;       /**< Authentication requirement (see @link wiced_bt_gatt_auth_req_e wiced_bt_gatt_auth_req_t @endlink) */
    uint16_t                    s_handle;       /**< Starting handle */
    uint16_t                    e_handle;       /**< Ending handle */
    wiced_bt_uuid_t             uuid;           /**< uuid */
} wiced_bt_gatt_read_by_type_t;

#define GATT_MAX_READ_MULTI_HANDLES      10     /**< Max attributes allowed in one GATT_READ_MULTIPLE request */
/** Parameters for GATT_READ_MULTIPLE */
typedef struct
{
    wiced_bt_gatt_auth_req_t    auth_req;                               /**< authentication requirement (see @link wiced_bt_gatt_auth_req_e wiced_bt_gatt_auth_req_t @endlink) */
    uint16_t                    num_handles;                            /**< number of handles to read */
    uint16_t                    handles[GATT_MAX_READ_MULTI_HANDLES];   /**< handles list to be read */
} wiced_bt_gatt_read_multi_t;

/** Parameters for GATT_READ_BY_HANDLE */
typedef struct
{
    wiced_bt_gatt_auth_req_t    auth_req;    /**< authentication requirement (see @link wiced_bt_gatt_auth_req_e wiced_bt_gatt_auth_req_t @endlink) */
    uint16_t                    handle;      /**< handle */
} wiced_bt_gatt_read_by_handle_t;

/** Parameters for GATT_READ_PARTIAL */
typedef struct
{
    wiced_bt_gatt_auth_req_t    auth_req;    /**< authentication requirement (see @link wiced_bt_gatt_auth_req_e wiced_bt_gatt_auth_req_t @endlink) */
    uint16_t                    handle;      /**< handle */
    uint16_t                    offset;      /**< offset */
} wiced_bt_gatt_read_partial_t;

/** Read request parameters - used when calling #wiced_bt_gatt_send_read */
typedef union
{
    wiced_bt_gatt_read_by_type_t   service;           /**< Parameters for GATT_READ_BY_TYPE */
    wiced_bt_gatt_read_by_type_t   char_type;         /**< Parameters for GATT_READ_CHAR_VALUE */
    wiced_bt_gatt_read_multi_t     read_multiple;     /**< Parameters for GATT_READ_MULTIPLE */
    wiced_bt_gatt_read_by_handle_t by_handle;         /**< Parameters for GATT_READ_BY_HANDLE */
    wiced_bt_gatt_read_partial_t   partial;           /**< Parameters for GATT_READ_PARTIAL  */
} wiced_bt_gatt_read_param_t;

/**  Write request types - used when calling #wiced_bt_gatt_send_write */
enum wiced_bt_gatt_write_type_e
{
    GATT_WRITE_NO_RSP = 1,  /**< Write without response */
    GATT_WRITE,             /**< Write with response */
    GATT_WRITE_PREPARE      /**< Prepare to write (call #wiced_bt_gatt_send_execute_write to execute the write) */
};
typedef uint8_t wiced_bt_gatt_write_type_t;     /**< GATT write type (see #wiced_bt_gatt_write_type_e) */

/**  Response data for read operations */
typedef struct
{
    uint16_t                    handle;     /**< handle */
    uint16_t                    len;        /**< length of response data */
    uint16_t                    offset;     /**< offset */
    uint8_t                     *p_data;    /**< attribute data */
} wiced_bt_gatt_data_t;

/** Client Operation Complete response data (dependent on operation completed) */
typedef union
{
    wiced_bt_gatt_data_t    att_value;      /**< Response data for read operations (initiated using #wiced_bt_gatt_send_read) */
    uint16_t                mtu;            /**< Response data for configuration operations */
    uint16_t                handle;         /**< Response data for write operations (initiated using #wiced_bt_gatt_send_write) */
} wiced_bt_gatt_operation_complete_rsp_t;   /**< GATT operation complete response type */

/** GATT client operation type, used in client callback function
*/
enum wiced_bt_gatt_optype_e
{
    GATTC_OPTYPE_NONE             = 0,    /**< None      */
    GATTC_OPTYPE_DISCOVERY        = 1,    /**< Discovery */
    GATTC_OPTYPE_READ             = 2,    /**< Read      */
    GATTC_OPTYPE_WRITE            = 3,    /**< Write     */
    GATTC_OPTYPE_EXE_WRITE        = 4,    /**< Execute Write */
    GATTC_OPTYPE_CONFIG           = 5,    /**< Configure */
    GATTC_OPTYPE_NOTIFICATION     = 6,    /**< Notification */
    GATTC_OPTYPE_INDICATION       = 7     /**< Indication */
};

/*  GATT Client Operation Codes */
typedef uint8_t wiced_bt_gatt_optype_t; /**< GATT operation type (see #wiced_bt_gatt_optype_e) */

/** characteristic declaration */
typedef struct
{
    wiced_bt_gatt_char_properties_t characteristic_properties;  /**< characteristic properties (see @link wiced_bt_gatt_char_properties_e wiced_bt_gatt_char_properties_t @endlink) */
    uint16_t                        val_handle;                 /**< characteristic value attribute handle */
    uint16_t                        handle;                     /**< characteristic declaration handle */
    wiced_bt_uuid_t                 char_uuid;                  /**< characteristic UUID type */
} wiced_bt_gatt_char_declaration_t;

/** GATT group value */
typedef struct
{
    wiced_bt_uuid_t service_type;   /**< group type */
    uint16_t        s_handle;       /**< starting handle of the group */
    uint16_t        e_handle;       /**< ending handle of the group */
} wiced_bt_gatt_group_value_t;


/** included service attribute value */
typedef struct
{
    wiced_bt_uuid_t service_type;   /**< included service UUID */
    uint16_t        handle;         /**< included service handle */
    uint16_t        s_handle;       /**< starting handle */
    uint16_t        e_handle;       /**< ending handle */
} wiced_bt_gatt_included_service_t;

/** characteristic descriptor information */
typedef struct
{
    wiced_bt_uuid_t         type;      /**< descriptor UUID type */
    uint16_t                handle;    /**< descriptor attribute handle */
}wiced_bt_gatt_char_descr_info_t;


/**
 * Discovery result data
 * Use  GATT_DISCOVERY_RESULT_SERVICE_* or GATT_DISCOVERY_RESULT_CHARACTERISTIC_* macros to parse discovery data)
 */
typedef union
{
    wiced_bt_gatt_included_service_t    included_service;           /**< Result for GATT_DISCOVER_INCLUDED_SERVICES */
    wiced_bt_gatt_group_value_t         group_value;                /**< Result for GATT_DISCOVER_SERVICES_ALL or GATT_DISCOVER_SERVICES_BY_UUID  */
    wiced_bt_gatt_char_declaration_t    characteristic_declaration; /**< Result for GATT_DISCOVER_CHARACTERISTICS */
    wiced_bt_gatt_char_descr_info_t     char_descr_info;            /**< Result for GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS */
} wiced_bt_gatt_discovery_data_t;


/** advertisement type (used when calling wiced_bt_start_advertisements) */
#ifndef BTM_BLE_ADVERT_MODE
#define BTM_BLE_ADVERT_MODE
enum wiced_bt_ble_advert_mode_e
{
    BTM_BLE_ADVERT_OFF,                 /**< Stop advertising */
    BTM_BLE_ADVERT_DIRECTED_HIGH,       /**< Directed advertisement (high duty cycle) */
    BTM_BLE_ADVERT_DIRECTED_LOW,        /**< Directed advertisement (low duty cycle) */
    BTM_BLE_ADVERT_UNDIRECTED_HIGH,     /**< Undirected advertisement (high duty cycle) */
    BTM_BLE_ADVERT_UNDIRECTED_LOW,      /**< Undirected advertisement (low duty cycle) */
    BTM_BLE_ADVERT_NONCONN_HIGH,        /**< Non-connectable advertisement (high duty cycle) */
    BTM_BLE_ADVERT_NONCONN_LOW,         /**< Non-connectable advertisement (low duty cycle) */
    BTM_BLE_ADVERT_DISCOVERABLE_HIGH,   /**< discoverable advertisement (high duty cycle) */
    BTM_BLE_ADVERT_DISCOVERABLE_LOW     /**< discoverable advertisement (low duty cycle) */
};
#endif
typedef uint8_t wiced_bt_ble_advert_mode_t;   /**< Advertisement type (see #wiced_bt_ble_advert_mode_e) */

/**
 * @anchor WICED_BT_TRANSPORT_TYPE
 * @name Transport types
 * @{
 */
#define BT_TRANSPORT_BR_EDR         1       /**< BR/EDR transport */
#define BT_TRANSPORT_LE             2       /**< BLE transport */
typedef uint8_t wiced_bt_transport_t;       /**< Transport type (see @ref WICED_BT_TRANSPORT_TYPE "BT Transport Types") */

/**
 * @anchor WICED_BT_DEVICE_TYPE
 * @name Device Types
 * @{
 */
#define BT_DEVICE_TYPE_BREDR        0x01    /**< BR/EDR device */
#define BT_DEVICE_TYPE_BLE          0x02    /**< LE device */
#define BT_DEVICE_TYPE_BREDR_BLE    0x03    /**< Dual Mode device */
typedef uint8_t wiced_bt_device_type_t;     /**< Bluetooth device type (see @ref WICED_BT_DEVICE_TYPE "BT Device Types") */
/** @} WICED_BT_DEVICE_TYPE */

/**
 * @anchor WICED_BT_ADDR_TYPE
 * @name Address Types
 * @{
 */
#define BLE_ADDR_PUBLIC             0x00        /**< Public address */
#define BLE_ADDR_RANDOM             0x01        /**< Random address */
#define BLE_ADDR_PUBLIC_ID          0x02        /**< Public ID      */
#define BLE_ADDR_RANDOM_ID          0x03        /**< Random ID      */
typedef uint8_t wiced_bt_ble_address_type_t;    /**< BLE device address type (see @ref WICED_BT_ADDR_TYPE "BT Address Types")*/
#define BLE_ADDR_TYPE_MASK          (BLE_ADDR_RANDOM | BLE_ADDR_PUBLIC)

typedef uint8_t *wiced_bt_device_address_ptr_t;                 /**< Device address Pointer */

/** Advertisement data types */
enum wiced_bt_ble_advert_type_e {
    BTM_BLE_ADVERT_TYPE_FLAG                        = 0x01,                 /**< Advertisement flags */
    BTM_BLE_ADVERT_TYPE_16SRV_PARTIAL               = 0x02,                 /**< List of supported services - 16 bit UUIDs (partial) */
    BTM_BLE_ADVERT_TYPE_16SRV_COMPLETE              = 0x03,                 /**< List of supported services - 16 bit UUIDs (complete) */
    BTM_BLE_ADVERT_TYPE_32SRV_PARTIAL               = 0x04,                 /**< List of supported services - 32 bit UUIDs (partial) */
    BTM_BLE_ADVERT_TYPE_32SRV_COMPLETE              = 0x05,                 /**< List of supported services - 32 bit UUIDs (complete) */
    BTM_BLE_ADVERT_TYPE_128SRV_PARTIAL              = 0x06,                 /**< List of supported services - 128 bit UUIDs (partial) */
    BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE             = 0x07,                 /**< List of supported services - 128 bit UUIDs (complete) */
    BTM_BLE_ADVERT_TYPE_NAME_SHORT                  = 0x08,                 /**< Short name */
    BTM_BLE_ADVERT_TYPE_NAME_COMPLETE               = 0x09,                 /**< Complete name */
    BTM_BLE_ADVERT_TYPE_TX_POWER                    = 0x0A,                 /**< TX Power level  */
    BTM_BLE_ADVERT_TYPE_DEV_CLASS                   = 0x0D,                 /**< Device Class */
    BTM_BLE_ADVERT_TYPE_SM_TK                       = 0x10,                 /**< Security manager TK value */
    BTM_BLE_ADVERT_TYPE_SM_OOB_FLAG                 = 0x11,                 /**< Security manager Out-of-Band data */
    BTM_BLE_ADVERT_TYPE_INTERVAL_RANGE              = 0x12,                 /**< Slave connection interval range */
    BTM_BLE_ADVERT_TYPE_SOLICITATION_SRV_UUID       = 0x14,                 /**< List of solicitated services - 16 bit UUIDs */
    BTM_BLE_ADVERT_TYPE_128SOLICITATION_SRV_UUID    = 0x15,                 /**< List of solicitated services - 128 bit UUIDs */
    BTM_BLE_ADVERT_TYPE_SERVICE_DATA                = 0x16,                 /**< Service data - 16 bit UUID */
    BTM_BLE_ADVERT_TYPE_PUBLIC_TARGET               = 0x17,                 /**< Public target address */
    BTM_BLE_ADVERT_TYPE_RANDOM_TARGET               = 0x18,                 /**< Random target address */
    BTM_BLE_ADVERT_TYPE_APPEARANCE                  = 0x19,                 /**< Appearance */
    BTM_BLE_ADVERT_TYPE_ADVERT_INTERVAL             = 0x1a,                 /**< Advertising interval */
    BTM_BLE_ADVERT_TYPE_32SOLICITATION_SRV_UUID     = 0x1b,                 /**< List of solicitated services - 32 bit UUIDs */
    BTM_BLE_ADVERT_TYPE_32SERVICE_DATA              = 0x1c,                 /**< Service data - 32 bit UUID */
    BTM_BLE_ADVERT_TYPE_128SERVICE_DATA             = 0x1d,                 /**< Service data - 128 bit UUID */
    BTM_BLE_ADVERT_TYPE_MANUFACTURER                = 0xFF                  /**< Manufacturer data */
};
typedef uint8_t   wiced_bt_ble_advert_type_t;    /**< BLE advertisement data type (see #wiced_bt_ble_advert_type_e) */

typedef struct
{
    uint8_t                         *p_data;                        /**< ADV data */
    uint16_t                        len;                               /**< Advertisement length */
    wiced_bt_ble_advert_type_t      advert_type;                          /**< Advertisement data type */
}wiced_bt_ble_advert_elem_t;


/** GATT events */
typedef enum
{
    GATT_CONNECTION_STATUS_EVT,                         /**< GATT connection status change. Event data: #wiced_bt_gatt_connection_status_t */
    GATT_OPERATION_CPLT_EVT,                            /**< GATT operation complete. Event data: #wiced_bt_gatt_event_data_t */
    GATT_DISCOVERY_RESULT_EVT,                          /**< GATT attribute discovery result. Event data: #wiced_bt_gatt_discovery_result_t */
    GATT_DISCOVERY_CPLT_EVT,                            /**< GATT attribute discovery complete. Event data: #wiced_bt_gatt_event_data_t */
    GATT_ATTRIBUTE_REQUEST_EVT                          /**< GATT attribute request (from remote client). Event data: #wiced_bt_gatt_attribute_request_t */
} wiced_bt_gatt_evt_t;

/** Discovery result (used by GATT_DISCOVERY_RESULT_EVT notification) */
typedef struct
{
    uint16_t                                conn_id;            /**< ID of the connection */
    wiced_bt_gatt_discovery_type_t          discovery_type;     /**< Discovery type (see @link wiced_bt_gatt_discovery_type_e wiced_bt_gatt_discovery_type_t @endlink) */
    wiced_bt_gatt_discovery_data_t          discovery_data;     /**< Discovery data  */
} wiced_bt_gatt_discovery_result_t;

/** Discovery Complete (used by GATT_DISCOVERY_CPLT_EVT notification) */
typedef struct
{
    uint16_t                                conn_id;            /**< ID of the connection */
    wiced_bt_gatt_discovery_type_t          disc_type;          /**< Discovery type (see @link wiced_bt_gatt_discovery_type_e wiced_bt_gatt_discovery_type_t @endlink) */
    wiced_bt_gatt_status_t                  status;             /**< Status of operation */
} wiced_bt_gatt_discovery_complete_t;

/** Response to read/write/disc/config operations (used by GATT_OPERATION_CPLT_EVT notification) */
typedef struct
{
    uint16_t                                conn_id;            /**< ID of the connection */
    wiced_bt_gatt_optype_t                  op;                 /**< Type of operation completed (see @link wiced_bt_gatt_optype_e wiced_bt_gatt_optype_t @endlink) */
    wiced_bt_gatt_status_t                  status;             /**< Status of operation */
    wiced_bt_gatt_operation_complete_rsp_t  response_data;      /**< Response data (dependent on optype) */
} wiced_bt_gatt_operation_complete_t;

/** GATT connection status (used by GATT_CONNECTION_STATUS_EVT notification) */
typedef struct
{
    uint8_t                         *bd_addr;               /**< Remote device address */
    wiced_bt_ble_address_type_t     addr_type;              /**< Remmote device address type */
    uint16_t                        conn_id;                /**< ID of the connection */
    BOOLEAN                         connected;              /**< TRUE if connected, FALSE if disconnected */
    wiced_bt_gatt_disconn_reason_t  reason;                 /**< Reason code (see @link wiced_bt_gatt_disconn_reason_e wiced_bt_gatt_disconn_reason_t @endlink) */
    wiced_bt_transport_t            transport;              /**< Transport type of the connection */
    uint8_t                         link_role;              /**< Link role on this connection */
} wiced_bt_gatt_connection_status_t;

/** GATT attribute request (used by GATT_ATTRIBUTE_REQUEST_EVT notification) */
typedef struct
{
    uint16_t                                conn_id;            /**< ID of the connection */
    wiced_bt_gatt_request_type_t            request_type;       /**< Request type (see @link wiced_bt_gatt_request_type_e wiced_bt_gatt_request_type_t)  */
    wiced_bt_gatt_request_data_t            data;               /**< Information about attribute being request (dependent on request type) */
} wiced_bt_gatt_attribute_request_t;

/** GATT channel congestion/uncongestion (used by GATT_CONGESTION_EVT notification) */
typedef struct
{
    uint16_t                                conn_id;            /**< ID of the connection */
    BOOLEAN                                 congested;          /**< congestion state */
} wiced_bt_gatt_congestion_event_t;

/** Stuctures for GATT event notifications */
typedef union
{
    wiced_bt_gatt_discovery_result_t        discovery_result;   /**< Data for GATT_DISCOVERY_RESULT_EVT */
    wiced_bt_gatt_discovery_complete_t      discovery_complete; /**< Data for GATT_DISCOVERY_CPLT_EVT */
    wiced_bt_gatt_operation_complete_t      operation_complete; /**< Data for GATT_OPERATION_CPLT_EVT */
    wiced_bt_gatt_connection_status_t       connection_status;  /**< Data for GATT_CONNECTION_STATUS_EVT */
    wiced_bt_gatt_attribute_request_t       attribute_request;  /**< Data for GATT_ATTRIBUTE_REQUEST_EVT */
    wiced_bt_gatt_congestion_event_t        congestion;         /**< Data for GATT_CONGESTION_EVT */
} wiced_bt_gatt_event_data_t;

/**
 * GATT event notification callback
 *
 * Callback for GATT event notifications
 * Registered using wiced_bt_gatt_register()
 *
 * @param event             : Event ID
 * @param p_event_data      : Event data
 *
 * @return Status of event handling
*/
typedef wiced_bt_gatt_status_t wiced_bt_gatt_cback_t(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
