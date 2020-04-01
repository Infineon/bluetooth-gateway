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

#include <iostream>
#include "cloud_client.h"
#include "cloud_client_default_config.h"
#include <string.h>


AWSMQTTClient::AWSMQTTClient(NetworkInterface& iface, const char* name, ClientSecurity* dev_security):
   CloudClient(iface, name, CLIENT_MQTT_AWS, dev_security),publish_topic(NULL),subscribe_topic(NULL),
   aws_client(NULL)
{
    if (!dev_security || (dev_security->get_type() != CLIENT_SECURITY_TYPE_TLS))
    {
        return;
    }

    /* Thing-name provided during Client initialization will take precedence over what is set in Security Object */
    if (!name)
    {
        dev_security->set_tls_params(name);
    }

    const char* private_key = dev_security->get_device_key();
    const char* tls_name = dev_security->get_tls_client_name();
    uint16_t key_length = strlen(private_key);
    const char* certificate = dev_security->get_device_certificate();
    uint16_t cert_length = strlen(certificate);

    aws_client = new AWSIoTClient((NetworkInterface*)&iface, tls_name,
                                    private_key, key_length,
                                    certificate, cert_length);

    if (!aws_client)
    {
        return;
    }
}

AWSMQTTClient::AWSMQTTClient(NetworkInterface& iface, ClientSecurity* dev_security):
    CloudClient(iface, "aws-mqtt", CLIENT_MQTT_AWS, dev_security),publish_topic(NULL),subscribe_topic(NULL),
    aws_client(NULL)
{
    if (!dev_security || (dev_security->get_type() != CLIENT_SECURITY_TYPE_TLS))
    {
        return;
    }

    const char* private_key = dev_security->get_device_key();
    const char* tls_name = dev_security->get_tls_client_name();
    uint16_t key_length = strlen(private_key);
    const char* certificate = dev_security->get_device_certificate();
    uint16_t cert_length = strlen(certificate);

    aws_client = new AWSIoTClient((NetworkInterface*)&iface, tls_name,
                                    private_key, key_length,
                                    certificate, cert_length);

    if (!aws_client)
    {
        return;
    }
}

AWSMQTTClient::~AWSMQTTClient()
{
    if (aws_client)
    {
        delete aws_client;
    }
}

cy_rslt_t AWSMQTTClient::initialize(void)
{
    return CY_RSLT_SUCCESS;
}

void AWSMQTTClient::shutdown(void)
{
    return;
}

cy_rslt_t AWSMQTTClient::connect(ClientConnectionParams* params)
{
    int use_default = 0;
    if (!aws_client)
    {
        return CY_RSLT_MW_ERROR;
    }

    if (!params)
    {
        use_default = 1;;
    }

    if (params && !params->uri)
    {
        return CY_RSLT_MW_ERROR;
    }

    ClientSecurity& security_ctxt = *_security;
    char* uri = (char *)(use_default ? AWS_MQTT_DEFAULT_ENDPOINT : params->uri);
    if (security_ctxt.get_type() == CLIENT_SECURITY_TYPE_NONE ||
        security_ctxt.get_type() == CLIENT_SECURITY_TYPE_USERID_PASSWORD)
    {
        /* Not supported - especially user-id/password mode */
        return CY_RSLT_MW_ERROR;
    }

    aws_endpoint_params_t endpoint_params = {AWS_TRANSPORT_MQTT_NATIVE,NULL,0,NULL,0 };
    /* set MQTT endpoint parameters */
    endpoint_params.transport = AWS_TRANSPORT_MQTT_NATIVE;
    endpoint_params.uri = uri;
    endpoint_params.port = use_default? AWS_MQTT_DEFAULT_SECURE_PORT : params->port;
    endpoint_params.root_ca = security_ctxt.get_tls_root_certificate();
    endpoint_params.root_ca_length = strlen(security_ctxt.get_tls_root_certificate());

    /* set MQTT connection parameters */
    aws_client->set_command_timeout(AWS_MQTT_DEFAULT_COMMAND_TIMEOUT);

    aws_connect_params_t conn_params;
    conn_params.username = NULL;
    conn_params.password = NULL;
    conn_params.alpn_string = NULL;
    conn_params.clean_session = 1;
    conn_params.peer_cn = (uint8_t *)uri;
    conn_params.keep_alive = use_default? AWS_MQTT_KEEP_ALIVE_TIMEOUT : params->keep_alive_interval;
    conn_params.client_id = (uint8_t *)security_ctxt.get_tls_client_name();

    /* connect to an AWS endpoint */
    cy_rslt_t result = aws_client->connect (conn_params, endpoint_params);
    if (result != CY_RSLT_SUCCESS)
    {
        cout<<"[Error] connection to AWS endpoint failed result: "<<result<<endl;
        return CY_RSLT_MW_ERROR;
    }

    cout<<"Connected to AWS endpoint: "<<(use_default?AWS_MQTT_DEFAULT_ENDPOINT:params->uri)<<endl;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t AWSMQTTClient::disconnect(void)
{
   cout<< "AWSMQTTClient::disconnect \n" << endl ;
   aws_client->disconnect();
   delete aws_client;
   return CY_RSLT_SUCCESS;
}

cy_rslt_t AWSMQTTClient::publish(const char* topic, uint8_t* data, uint32_t length)
{
    if (!aws_client || !topic)
    {
        return CY_RSLT_MW_ERROR;
    }

    aws_publish_params_t once;
    once.QoS = AWS_QOS_ATMOST_ONCE;

    cy_rslt_t result = aws_client->publish(topic, (const char *)data, length, once);
    if (result != CY_RSLT_SUCCESS)
    {
        cout<<"[Error] Publish to AWS failed result: "<<result<<endl;
        return CY_RSLT_MW_ERROR;
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t AWSMQTTClient::subscribe(const char* topic, subscriber_callback cb)
{
    if (!aws_client || !topic)
    {
        return CY_RSLT_MW_ERROR;
    }

    aws_iot_qos_level_t once = AWS_QOS_ATMOST_ONCE;

    cy_rslt_t result = aws_client->subscribe(topic, once, cb);
    if (result != CY_RSLT_SUCCESS)
    {
        return CY_RSLT_MW_ERROR;
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t AWSMQTTClient::yield(int timeout)
{
    if (!aws_client)
    {
        return CY_RSLT_MW_ERROR;
    }
    cy_rslt_t result = aws_client->yield(timeout);
    return result;
}

cy_rslt_t ClientSecurity::set_user_id_password(const char* user_id, const char* password)
{
    if (type == CLIENT_SECURITY_TYPE_TLS || !user_id)
        return 0;

    if (type == CLIENT_SECURITY_TYPE_NONE)
    {
        type = CLIENT_SECURITY_TYPE_USERID_PASSWORD;
    }

    security.user.name = user_id;
    security.user.password = password;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t ClientSecurity::set_tls_params(const char* client_name, const char* private_key, const char* certificate)
{
    if (type == CLIENT_SECURITY_TYPE_NONE || type == CLIENT_SECURITY_TYPE_USERID_PASSWORD)
    {
        cout<<"[Error] Invalid Security type is set"<<endl;
        return CY_RSLT_MW_ERROR;
    }

    uint16_t key_length = strlen(private_key);
    uint16_t cert_length = strlen(certificate);
    uint8_t  client_name_length = strlen(client_name);

    if (key_length == 0 || cert_length == 0 || client_name_length == 0)
    {
        cout<<"[Error] Invalid Private-Key or Certificate or Client name length"<<endl;
        return CY_RSLT_MW_ERROR;
    }
    else if (key_length >= CLIENT_PRIVATE_KEY_MAX_LENGTH || cert_length >= CLIENT_CERTIFICATE_MAX_LENGTH || client_name_length >= CLIENT_NAME_MAX_LENGTH)
    {
        cout<<"[Warning] Private-Key or Certificate or Client name matches or exceeds max-length"<<endl;
    }

    security.tls.client_name = client_name;
    security.tls.private_key = private_key;
    security.tls.key_length = key_length;
    security.tls.certificate = certificate;
    security.tls.cert_length = cert_length;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t ClientSecurity::set_tls_params(const char* client_name)
{
    if (type == CLIENT_SECURITY_TYPE_NONE || type == CLIENT_SECURITY_TYPE_USERID_PASSWORD)
    {
        cout<<"[Error] Invalid Security type is set"<<endl;
        return CY_RSLT_MW_ERROR;
    }

    security.tls.client_name = client_name;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t ClientSecurity::set_tls_root_certificate(const char* root_cert)
{
    if (type == CLIENT_SECURITY_TYPE_NONE || type == CLIENT_SECURITY_TYPE_USERID_PASSWORD)
    {
        cout<<"[Error] Invalid Security type is set"<<endl;
        return CY_RSLT_MW_ERROR;
    }

    uint16_t root_ca_cert_length = strlen(root_cert);
    if (root_ca_cert_length == 0)
    {
        cout<<"[Error] Invalid Root-Certificate length"<<endl;
        return CY_RSLT_MW_ERROR;
    }
    else if (root_ca_cert_length == CLIENT_ROOT_CA_MAX_LENGTH)
    {
        cout<<"[Warning] Root Certificate matches or exceeds max-length"<<endl;
    }

    security.tls.root_ca_certificate = root_cert;
    security.tls.root_ca_cert_length = root_ca_cert_length;
    return CY_RSLT_SUCCESS;
}
