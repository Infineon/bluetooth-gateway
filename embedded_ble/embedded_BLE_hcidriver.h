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

#include "embedded_BLE_hcitransportdriver.h"
#include "cybsp_types.h"

/**
 * \defgroup embedded_ble_hci Embedded BLE HCI Interface
 * @ingroup embedded_ble
 */

namespace cypress
{
namespace embedded
{

/**
 * @addtogroup embedded_ble_hci
 *
 * Embedded BLE HCI Driver class
 * @{
 */

/** Defines Embedded BLE HCI Driver class */
class EmbeddedHCIDriver
{
public:
    /**
     * Construct a new instance of an HCI driver.
     * @param transport_driver:  The driver used to communicate with the chip.
     * @param bt_power_name : BT Power-on Pin
     */
    EmbeddedHCIDriver(EmbeddedHCITransportDriver& transport_driver, PinName bt_power_name):
        _transport_driver(transport_driver),
        bt_power_name(bt_power_name),
        bt_power(bt_power_name, PIN_OUTPUT, PullUp, 0)
        { }

    /**
     * Driver destructor
     */
    virtual ~EmbeddedHCIDriver() { }

    /**
     * Initialize the Embedded HCI driver.
     * This function start by initializing the transport driver then it delegates
     * what's remain of the initialization to the function do_initialize.
     */
    void initialize();

    /**
     * Termination of the driver.
     * It call in sequence:
     *   - do_terminate
     *   - terminate the transport driver.
     */
    void terminate();

    /**
     * Signal to the stack that the reset sequence has been done.
     */
    void signal_reset_sequence_done() {}

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

private:
    /**
     * Initialize the chip.
     * The transport is up at that time.
     */
    void do_initialize()
    {
        bt_power = 1;
        wait_us(500 * 1000);
    }

    /**
     * Terminate the driver
     */
    void do_terminate() { }

private:
    EmbeddedHCITransportDriver& _transport_driver;
    PinName bt_power_name;
    DigitalInOut bt_power;
};
/** @} */
} // end of namespace 'embedded'

} // end of namespace 'cypress'
