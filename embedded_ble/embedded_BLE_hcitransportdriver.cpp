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

#if DEVICE_SERIAL && DEVICE_SERIAL_FC

#include "embedded_BLE_hcitransportdriver.h"
#include "cycfg_pins.h"

using namespace cypress::embedded;

extern void wiced_hci_serial_data_rcv_handler(uint8_t* data, uint8_t len);

EmbeddedHCITransportDriver::data_received_handler_t
    EmbeddedHCITransportDriver::data_received_handler = wiced_hci_serial_data_rcv_handler;

void EmbeddedHCITransportDriver::bt_host_wake_irq_handler(void)
{
    sleep_manager_lock_deep_sleep();
    EmbeddedHCITransportDriver::on_controller_irq();
    sleep_manager_unlock_deep_sleep();
}

void EmbeddedHCITransportDriver::initialize()
{
    uart.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    uart.set_flow_control(
        /* flow */ SerialBase::RTSCTS,
        /* rts */ rts,
        /* cts */ cts
    );

    uart.attach(
        callback(this, &EmbeddedHCITransportDriver::on_controller_irq),
        SerialBase::RxIrq
    );

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    InterruptIn *host_wake_pin;
    //Register IRQ for Host WAKE
    host_wake_pin = new InterruptIn(bt_host_wake_name);
    host_wake_pin->fall(callback(this, &EmbeddedHCITransportDriver::bt_host_wake_irq_handler));
#endif
    bt_device_wake = 0;
    wait_us(500 * 1000);
}

void EmbeddedHCITransportDriver::terminate() {  }

uint16_t EmbeddedHCITransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    assert_bt_dev_wake();

    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
        while (uart.writeable() == 0);
        uart.putc(to_write);
        ++i;
    }

    deassert_bt_dev_wake();
    return len;
}

void EmbeddedHCITransportDriver::on_controller_irq()
{
    assert_bt_dev_wake();

    while (uart.readable()) {
        uint8_t char_received = uart.getc();
        on_data_received(&char_received, 1);
    }

    deassert_bt_dev_wake();
}

void EmbeddedHCITransportDriver::assert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    bt_device_wake = 0;
#endif
}

void EmbeddedHCITransportDriver::deassert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    //De-assert bt_device_wake
    bt_device_wake = 1;
#endif
}


void EmbeddedHCITransportDriver::on_data_received(uint8_t* data, uint16_t len)
{
    while (len) {
        uint8_t chunk_length = std::min(len, (uint16_t) std::numeric_limits<uint8_t>::max());
        data_received_handler(data, chunk_length);
        len = len - chunk_length;
        data = data + chunk_length;
    }
}

#endif // DEVICE_SERIAL && DEVICE_SERIAL_FC
