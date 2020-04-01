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

#define CLIENT_PRIVATE_KEY_MAX_LENGTH    (8*1024)
#define CLIENT_CERTIFICATE_MAX_LENGTH    (8*1024)
#define CLIENT_ROOT_CA_MAX_LENGTH        (16*1024)
#define CLIENT_NAME_MAX_LENGTH           (23)
/**
 * ----- AWS MQTT Default values -----
 */
#define AWS_MQTT_DEFAULT_SECURE_PORT        (8883)
#define AWS_MQTT_DEFAULT_ENDPOINT           ""
#define AWS_MQTT_DEFAULT_KEEP_ALIVE         (5)

#define AWS_MQTT_DEFAULT_COMMAND_TIMEOUT    (5000)
