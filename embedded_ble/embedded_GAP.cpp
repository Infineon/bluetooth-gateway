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
 * @file Cypress' Embedded Bluetooth GAP Implementation
 */

#include <stdio.h>
#include <string.h>
#include "embedded_BLE.h"
#include "embedded_GAP.h"

#include "wiced_hci_bt_dm.h"

using namespace cypress::embedded;

Gap* Gap::gap = NULL;

ble_error_t Gap::getAddress(BluetoothAddress addr)
{
    wiced_bt_device_address_t waddr = {0};

    wiced_bt_dev_read_local_addr(waddr);
    memcpy(addr, waddr, 6);

    return BLE_ERROR_NONE;
}

ble_error_t Gap::setAddress(BluetoothAddress addr)
{
    wiced_bt_device_address_t waddr = {0};
    memcpy(waddr, addr, 6);
    wiced_bt_set_local_bdaddr(waddr);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::getName(char* name)
{
    (void)name;
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setName(const char* name)
{
    (void)name;
    return BLE_ERROR_NOT_IMPLEMENTED;
}

/* Bluetooth Low Energy Advertisement APIs */
ble_error_t Gap::setAdvertisementData(uint8_t* adv_data, uint8_t length)
{
    (void)adv_data;
    (void)length;
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::startAdvertisements(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::stopAdvertisements(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

/* Bluetooth Low Energy Scan APIs */
ble_error_t Gap::startScan(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::stopScan(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}
