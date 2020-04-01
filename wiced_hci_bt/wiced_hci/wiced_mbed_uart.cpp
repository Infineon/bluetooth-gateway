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
#include "cy_result_mw.h"
#include "mbed.h"
#include "wiced_mbed_uart.h"
#include "embedded_BLE_hcidriver.h"
#include "platform/CircularBuffer.h"


#define HCI_UART_BUFFER_SIZE    (2048)

using cypress::embedded::EmbeddedHCIDriver;

extern cypress::embedded::EmbeddedHCIDriver& ble_get_embedded_hci_driver();

static EmbeddedHCIDriver* hci_driver;
static CircularBuffer<uint8_t, HCI_UART_BUFFER_SIZE> hci_uart_buffer;

void wiced_hci_serial_data_rcv_handler(uint8_t* data, uint8_t len){
    uint8_t pos = 0;
    while(len)
    {
        hci_uart_buffer.push(data[pos++]);
        len--;
    }
}

void mbed_os_uart_read(uint8_t* data, uint32_t* length, uint32_t timeout_ms)
{
    if (!data || !length)
    {
        printf("[UART] Error Reading from Wiced HCI UART - Bad parameter\n");
        return;
    }

    if (*length == 0)
    {
        return;
    }

    uint32_t requested_len = *length;
    int i = 0;

    /* not using timeout for now, will be added later  */
    (void)timeout_ms;

    while(hci_uart_buffer.size() < requested_len)
    {
        // busy-wait till we get the buffer filled upto the size we are looking for...
    }

    /* wait until the requested length */
    while(requested_len){
        /* transfer it to the buffer given by user */
        hci_uart_buffer.pop(data[i++]);
        requested_len--;
    }
}

void mbed_os_uart_write(uint8_t* data, uint16_t length)
{
    uint8_t cmd_type = data[0];
    data++;
    uint16_t bytes_transmitted = hci_driver->write(cmd_type, length-1, data);
    (void)bytes_transmitted;
}

void mbed_os_uart_init(void)
{
    hci_driver = &(ble_get_embedded_hci_driver());
    hci_driver->initialize();
}

void mbed_os_uart_deinit(void)
{
    hci_driver->terminate();
}

