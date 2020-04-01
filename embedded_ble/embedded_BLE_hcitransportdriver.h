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

#include "mbed.h"
#include "drivers/DigitalInOut.h"

/**
 * \defgroup embedded_ble_hci_transport Embedded BLE HCI Transport Driver Interface
 * @ingroup embedded_ble
 */

namespace cypress
{
namespace embedded
{

/**
 * @addtogroup embedded_ble_hci_transport
 *
 * Embedded BLE HCI Transport Driver Class
 * @{
 */

/** Defines Embedded BLE HCI Transport Driver class */
class EmbeddedHCITransportDriver
{
public:
    /**
     * Embedded HCI Transport Driver constructor
     */
    EmbeddedHCITransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name) :
    uart(tx, rx, baud), cts(cts), rts(rts),
    bt_host_wake_name(bt_host_wake_name),
    bt_device_wake_name(bt_device_wake_name),
    bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
    bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullDefault, 1)
    { }

    /**
     * Driver destructor.
     */
    virtual ~EmbeddedHCITransportDriver() { }

    /**
     * Inialization of the transport
     */
    void initialize();

    /**
     * termination of the transport.
     */
    void terminate();

    /**
     * Write data in the transport channel.
     *
     * @param type The type of packet to transmit. It might be an HCI command
     * packet, ACL packet or EVT packet. Depending on the type of transport
     * it can prefix the packet itself.
     * @param len Number of bytes to transmit.
     * @param pData pointer to the data to transmit.
     *
     * @return The number of bytes which have been transmited.
     */
    uint16_t write(uint8_t type, uint16_t len, uint8_t *pData);

    /**
     * The driver shall call this function whenever data bytes are received.
     *
     * @param data Pointer to the data received.
     * @param len Number of bytes received.
     */
    static void on_data_received(uint8_t* data, uint16_t len);

    /**
     *  Bluetooth Transport driver IRQ handler
     */
    void bt_host_wake_irq_handler();

private:
    void on_controller_irq();
    void assert_bt_dev_wake();
    void deassert_bt_dev_wake();
    typedef void (*data_received_handler_t)(uint8_t* data, uint8_t len);

    static data_received_handler_t data_received_handler;

    static void set_data_received_handler(data_received_handler_t handler);

    // Use RawSerial as opposed to Serial as we don't require the locking primitives
    // provided by the Serial class (access to the UART should be exclusive to this driver)
    // Furthermore, we access the peripheral in interrupt context which would clash
    // with Serial's locking facilities
    RawSerial uart;
    PinName cts;
    PinName rts;
    PinName bt_host_wake_name;
    PinName bt_device_wake_name;
    DigitalInOut bt_host_wake;
    DigitalInOut bt_device_wake;
};
/** @} */
} // end of namespace 'embedded'

} // end of namespace 'cypress'
