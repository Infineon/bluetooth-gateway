/*
 * Copyright 2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "aws_client.h"
#include "aws_common.h"
#include "cy_result_mw.h"
#include "cloud_client_default_config.h"

/**
********************************************************************************
* \mainpage Overview
 ********************************************************************************
 * Bluetooth Mesh is a BT SIG ratified profile which describes how mesh network can be formed using Bluetooth Low Energy (BLE) devices.
 * The details of the Mesh spec can be found at https://www.bluetooth.com/specifications/mesh-specifications/
 * Typically a user would expose his mesh network to IoT using a Bluetooth Mesh Gateway.
 * Cypress Bluetooth Mesh Gateway Library can be used by application developers to implement a Bluetooth Mesh Gateway.
 * The library consists of CloudClient and Embedded BLE Library.
 *
 *
 * * CloudClient Library   : Enables Application to use different Cloud Clients for bluetooth mesh gateway application.
 * * Embedded BLE Library  : Enables applications to use Cypress BLE embedded mode, download mesh gateway firmware, transmit and recieve mesh data.
 *
 * \defgroup cloud_client Cloud Client library
 * \defgroup cloud_client_enums Cloud Client enumerations
 * @ingroup cloud_client
 * \defgroup cloud_client_classes Cloud Client base class and sub-classes, CloudClient Factory class
 * @ingroup cloud_client
 */

/**
 * @addtogroup cloud_client_enums
 *
 * @{
 */

/** Defines different MQTT clients */
enum ClientType
{
    CLIENT_MQTT_AWS = 0,            /**< AWS IoT MQTT client */
    CLIENT_MQTT_BLUEMIX,            /**< IBM Bluemix MQTT client */
    CLIENT_MQTT_GOOGLE_CLOUD,       /**< Google Cloud MQTT client */
    CLIENT_MQTT_AZURE,              /**< Microsoft Azure MQTT client */
    CLIENT_MQTT_GENERIC,            /**< Any other third-party MQTT client */
};

/** Defines different security-conventions used by the Client*/
enum ClientSecurityType
{
    CLIENT_SECURITY_TYPE_TLS,               /**< Uses TLS */
    CLIENT_SECURITY_TYPE_USERID_PASSWORD,   /**< Uses user-id/password */
    CLIENT_SECURITY_TYPE_NONE,              /**< No security */
};

/**
 * @}
 */


/**
 * @addtogroup cloud_client_classes
 *
 * CloudClient defines a base class to enable Bluetooth Mesh Gateway with different Cloud clients.
 * Goal is to abstract the cloud-client properties from application flow of Bluetooth Mesh Gateway and
 * allow application to choose and configure different Cloud services seamlessly with minimal application
 * code change.
 * For example, one may wish to use Bluetooth Mesh Gateway with different cloud services like AWS-IoT, IBM BlueMix, Azure etc.
 * This selection may be hard-coded at compile time or in more complex applications can be run-time as well. It is also possible
 * that a Bluetooth Mesh Gateway requires to be connected to two or more cloud services. Such a scenario is out-of-scope
 * for now but these classes and data-structures are designed to handle such scenarios easily.
 * @code
 *
 * CloudClientFactory f;
 * ClientSecurity aws_security_params;
 * ClientConnectionParams aws_conn_params;
 *
 * CloudClient* c = f.getClient("my_client", CLIENT_MQTT_AWS, aws_security_params);
 * c.initialize();
 * c.connect(aws_conn_params);
 * c.disconnect();
 * c.shutdown();
 *
 * ClientSecurity bmix_security_params;
 * ClientConnectionParams bmix_conn_params;
 * c = f.getClient("my_client", CLIENT_MQTT_BLUEMIX, bmix_security_params);
 * c.initialize();
 * c.connect(bmix_conn_params);
 * c.disconnect();
 * c.shutdown();
 *
 * @endcode
 * @{
 */


/**
 * Defines a CloudClient base class. For now, only AWS IoT Cloud client child-class has been implemented.
 * Support for Other Cloud clients like Bluemix, Azure, GCP will be added in future.
 */
union _ClientSecurity
{
    /** Defines security-params required for TLS security */
    struct
    {
        const char* client_name;            /**< Client-Name or Device name -required for TLS */
        const char* root_ca_certificate;    /**< Root CA Certificate */
        uint16_t root_ca_cert_length;       /**< Length of Root CA certificate */
        const char* private_key;            /**< Private key of the device */
        uint16_t key_length;                /**< Private key length */
        const char* certificate;            /**< Device Certificate */
        uint16_t cert_length;               /**< Device Certificate length */
    }tls;

    /** Defines security-params required for User-ID/Password security */
    struct
    {
        const char* name;                   /**< user-name or client-name to be used while connecting to server */
        const char* password;               /**< password for the given user-name */
    }user;
};

/** Defines Client Security object with security-params and methods */
class ClientSecurity
{
    ClientSecurityType type;
    union _ClientSecurity security;
public:
    /**
     * ClientSecurity Constructor with no params
     */
    ClientSecurity():type(CLIENT_SECURITY_TYPE_NONE)
    {
    }

    /**
     * ClientSecurity Constructor with single param
     */
    ClientSecurity(ClientSecurityType type):type(type)
    {
    }

    /**
     * Sets the security type
     */
    void set_type(ClientSecurityType t)
    {
        type = t;
    }

    /**
     * Set User-ID and password as SecurityParams
     * @param[in] user_id  : User identification
     * @param[in] password : User password
     *
     * @return cy_rslt_t   : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */

    cy_rslt_t set_user_id_password(const char* user_id, const char* password);

    /**
     * Set TLS as SecurityParams
     * @param[in] client_name : Name of the device/client to be used for TLS negotiation
     * @param[in] private_key : Null-Terminated Private key string
     * @param[in] certificate : Null-Terminated Device/Client Certificate
     *
     * @return cy_rslt_t      : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */
    cy_rslt_t set_tls_params(const char* client_name, const char* private_key, const char* certificate);

    /**
     * Set TLS as SecurityParams
     * @param[in] client_name : Name of the device/client to be used for TLS negotiation
     *
     * @return cy_rslt_t      : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */
    cy_rslt_t set_tls_params(const char* client_name);

    /**
     * Set TLS Root-Certificate to the object
     * @param[in] root_cert : NULL-terminated root-certificate
     *
     * @return cy_rslt_t    : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */
    cy_rslt_t set_tls_root_certificate(const char* root_cert);

    /**
     * Return Security-type associated with the object
     *
     * @return ClientSecurityType
     */
    ClientSecurityType get_type(void)
    {
        return type;
    }

    /**
     * Return user-id(user-id) associated with the object
     *
     * @return NULL if invalid object, else return Null-terminated user-id string
     */
    const char* get_username(void)
    {
        if (type != CLIENT_SECURITY_TYPE_USERID_PASSWORD)
        {
            return NULL;
        }
        return security.user.name;
    }

    /**
     * Return password associated with the object
     *
     * @return NULL if invalid object, else return Null-terminated password string
     */
    const char* get_password(void)
    {
        if (type != CLIENT_SECURITY_TYPE_USERID_PASSWORD)
        {
            return NULL;
        }
        return security.user.password;
    }

    /**
     * Return TLS Client-name
     *
     * @return NULL if invalid object, else return Null-terminated client-name
     */
    const char* get_tls_client_name(void)
    {
        if (type != CLIENT_SECURITY_TYPE_TLS)
        {
            return NULL;
        }
        return security.tls.client_name;
    }

    /**
     * Return Private Device-key in use
     *
     * @return NULL if invalid object, else return Null-terminated Device-key
     */
    const char* get_device_key(void)
    {
        if (type != CLIENT_SECURITY_TYPE_TLS)
        {
            return NULL;
        }
        return security.tls.private_key;
    }

    /**
     * Return Device certificate
     *
     * @return NULL if invalid object, else return Null-terminated Device-certificate
     */
    const char* get_device_certificate(void)
    {
        if (type != CLIENT_SECURITY_TYPE_TLS)
        {
            return NULL;
        }
        return security.tls.certificate;
    }

    /**
     * Return TLS root certificate
     *
     * @return NULL if invalid object, else return Null-terminated root-certificate
     */
    const char* get_tls_root_certificate(void)
    {
        if (type != CLIENT_SECURITY_TYPE_TLS)
        {
            return NULL;
        }
        return security.tls.root_ca_certificate;
    }
};

/** Defines Cloud-Client Connection Parameters */
struct ClientConnectionParams
{
    const char* uri;                 /**< Remote server/broker endpoint */
    uint16_t port;                   /**< Server port */
    uint16_t keep_alive_interval;    /**< keep-alive interval in seconds */
public:
    /**
     * Defines Constructor with uri, port and keep-alive params
     */
    ClientConnectionParams(const char* uri, uint16_t port, uint16_t keep_alive_interval):
    uri(uri), port(port), keep_alive_interval(keep_alive_interval)
    {
    }
    /* TODO - Tie the Connection object with Client-type(Client object) - that way,
     * we can pick default values for the connection client */
    /**
     * Defines Constructor with uri only
     */
    ClientConnectionParams(const char* uri): uri(uri), port(0), keep_alive_interval(0)
    {
    }
};


/** Defines a CloudClient base class */
class CloudClient
{
protected:
    NetworkInterface&   network;            /**< Network Interface to be used by the cloud-client */
    const char*         _client_name;       /**< user-defined name for this device/client */
    ClientSecurity*     _security;          /**< Security params to be used by this cloud-client*/
    enum ClientType     _type;              /**< Client type */

public:
    /**
     * CloudClient Destructor
     */
    virtual ~CloudClient() {};

    /**
     * CloudClient constructor
     * @param[in] iface : Network Interface instance to use
     *
     * By default, it assumes to be an AWS IoT Client.
     */
    CloudClient(NetworkInterface& iface):network(iface),_client_name(NULL),_security(NULL),_type(CLIENT_MQTT_AWS)
    {

    }

    /**
     * CloudClient constructor
     * @param[in] iface : Network Interface instance to use
     * @param[in] name: CloudClient name to use
     * @param[in] cli_type: Cloud Client type
     * @param[in] security: Client Security Object - defines security attributes to be used for the client
     *
     */
    CloudClient(NetworkInterface& iface, const char*name, ClientType cli_type, ClientSecurity* security):
    network(iface),_client_name(NULL),_security(security),_type(cli_type)
    {

    }

    /* Basic Setter/Getter */

    /**
     * Sets CloudClient name
     * @param[in] name: CloudClient name to use
     */
    void set_name(const char* name)
    {
        if (name)
            _client_name = name;
    }

    /**
     * Gets CloudClient name
     *
     * @return Null-terminated Cloud-client name
     */
    const char* get_name(void)
    {
        return _client_name;
    }

    /**
     * Returns CloudClient Object type
     *
     * @return ClientType
     */

    ClientType get_type(void)
    {
        return _type;
    }

    /**
     * Returns 1 if CloudClient is of MQTT type, 0 otherwise
     */
    int isMQTTClient(void)
    {
        if (_type <= CLIENT_MQTT_GENERIC)
            return 1;
        return 0;
    }

    /**
     * Returns NetworkInterface Instance in use by the client.
     */
    NetworkInterface& get_network_instance(void)
    {
        return network;
    }

    /**
     * Initialize Cloud-Client
     */
    virtual cy_rslt_t initialize(void) = 0;

    /**
     * Shut-down Cloud-Client
     */
    virtual void shutdown(void) = 0;
    /**
     * Connect to the server as specified in connection-parameters
     */
    virtual cy_rslt_t connect(ClientConnectionParams* params) = 0;
    /**
     * Disconnect from the server.
     */
    virtual cy_rslt_t disconnect(void) = 0;
};

/** Defines AWS MQTT Cloud Client class - Uses Cypress' AWS IoT library */
class AWSMQTTClient: public CloudClient
{

private:
    const char*         publish_topic;      /**< cache the publish-topic */
    const char*         subscribe_topic;    /**< cache the subscribe-topic */
    AWSIoTClient*       aws_client;         /**< AWS IoT Client */

public:
    /**
     * AWS CloudClient constructor
     * @param[in] iface : Network Interface instance to use
     * @param[in] security: Security Params to use
     */
    AWSMQTTClient(NetworkInterface& iface, ClientSecurity* security);
    /**
     * AWS CloudClient constructor
     * @param[in] iface : Network Interface instance to use
     * @param[in] name : AWS IoT Client Name
     * @param[in] security: Security Params to use
     */
    AWSMQTTClient(NetworkInterface& iface, const char* name, ClientSecurity* security);

    /**
     * AWS IoT Client Destructor
     */
    virtual ~AWSMQTTClient();

    /**
     * Publishes message to the broker at-most once.
     * @param[in] topic: Topic Name to use while publishing
     * @param[in] data : Payload pointer
     * @param[in] length: Length of payload
     * @return cy_rslt_t : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */
    cy_rslt_t publish(const char* topic, uint8_t* data, uint32_t length);

    /**
     * Subscribes to a topic
     * @param[in] topic : Topic Name to subscribe to
     * @param[in] cb : callback to invoke when a message is received for the given topic
     * @return cy_rslt_t : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */
    cy_rslt_t subscribe(const char* topic, subscriber_callback cb);

    /**
     * Busy-waits for any pending message on subscribed topics
     * @param[in] timeout : Busy-wait timeout period.
     * @return cy_rslt_t : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
     */
    cy_rslt_t yield(int timeout);

    /**
     * Initialize Cloud-Client
     */
    virtual cy_rslt_t initialize(void);
    /**
     * Shut-down Cloud-Client
     */
    virtual void shutdown(void);
    /**
     * Connect to the server as specified in connection-parameters
     */
    virtual cy_rslt_t connect(ClientConnectionParams* params);
    /**
     * Disconnect from the server.
     */
    virtual cy_rslt_t disconnect(void);
};

/** Defines Cloud-Client Factory class */
class CloudClientFactory
{
private:
    CloudClient* client;
public:
    /**
     * Get a Cloud Client Object based on the type.
     * @param[in] network_if: Network Interface instance used by the client
     * @param[in] type: Client-type
     * @param[in] security: Security attributes for this client
     */
    CloudClient* getClient(NetworkInterface& network_if, ClientType type, ClientSecurity* security)
    {
        switch (type)
        {
            case CLIENT_MQTT_AWS:
            {
                client = new AWSMQTTClient(network_if, security);
                return client;
            }

            case CLIENT_MQTT_BLUEMIX:
            case CLIENT_MQTT_GOOGLE_CLOUD:
            case CLIENT_MQTT_AZURE:
            case CLIENT_MQTT_GENERIC:
            default:
                return NULL;
        };
    }
};

/**
 * @}
 */
