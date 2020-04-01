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

#include "embedded_BLE_hcidriver.h"
#include "PinNames.h"

using namespace cypress::embedded;

void EmbeddedHCIDriver::initialize()
{
    _transport_driver.initialize();
    do_initialize();
}

void EmbeddedHCIDriver::terminate()
{
    do_terminate();
    _transport_driver.terminate();
}

uint16_t EmbeddedHCIDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    return _transport_driver.write(type, len, pData);
}

EmbeddedHCIDriver& ble_get_embedded_hci_driver() {
    static EmbeddedHCITransportDriver transport_driver(
        /* TX */ CYBSP_BT_UART_TX, /* RX */ CYBSP_BT_UART_RX,
        /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, 115200,
        CYBSP_BT_HOST_WAKE, CYBSP_BT_DEVICE_WAKE
    );
    static EmbeddedHCIDriver hci_driver(
        transport_driver,
        /* bt_power */ CYBSP_BT_POWER
    );
    return hci_driver;
}
