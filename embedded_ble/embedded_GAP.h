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
 * @file
 *
 * Embedded Bluetooth GAP Interface
 */

#pragma once

/**
 * \defgroup embedded_ble_gap Embedded BLE GAP Interface
 * @ingroup embedded_ble
 */

namespace cypress
{
namespace embedded
{

typedef uint8_t BluetoothAddress[6];

/**
 * @addtogroup embedded_ble_gap
 *
 * Embedded BLE GAP Class
 * @{
 */

/** Defines Embedded BLE GAP Class */
class Gap
{

public:
    /**
     * Gets static singleton instance of Embedded BLE GAP interface
     */
    static Gap& getGapInstance(void)
    {
        if (gap == NULL)
        {
            gap = new Gap;
        }
        return (Gap&)(*gap);
    }

    /** Generic Bluetooth Controller/Device Management routine */


    /** Gets Device' Bluetooth Address.
     *
     * @param[out] addr:  Device Bluetooth Address
     *
     * @return ble_error_t
     *
     */
    ble_error_t getAddress(BluetoothAddress addr);

    /** Sets Device' Bluetooth GAP name.
     *
     * @param[in] addr:  Device Bluetooth Address
     *
     * @return ble_error_t
     *
     */
    ble_error_t setAddress(BluetoothAddress addr);

    /** Gets Device' Bluetooth GAP name.
     *
     * @param[out] name:  Device Bluetooth Name
     *
     * @return ble_error_t
     *
     */
    ble_error_t getName(char* name);

    /** Sets Device' Bluetooth GAP name.
     *
     * @param[in] name:  Device Bluetooth Name
     *
     * @return ble_error_t
     *
     */
    ble_error_t setName(const char* name);

    /** Bluetooth Low Energy Advertisement APIs */


    /** Set Advertisement data.
     *
     * @param[in] adv_data:  Advertising data payload pointer
     * @param[in] length: Advertising data payload length
     *
     * @return ble_error_t
     *
     */
    ble_error_t setAdvertisementData(uint8_t* adv_data, uint8_t length);
    /** Start Advertising
     *
     * @return ble_error_t
     *
     */
    ble_error_t startAdvertisements(void);

    /** Stop advertising.
     *
     * @return ble_error_t
     *
     */
    ble_error_t stopAdvertisements(void);

    /** Bluetooth Low Energy Scan APIs */

    /** Start Scanning of nearby advertising devices.
     * @return ble_error_t
     *
     */

    ble_error_t startScan(void);

    /** Stop Scanning of nearby advertising devices.
     * @return ble_error_t
     *
     */
    ble_error_t stopScan(void);

private:
    static Gap* gap;
    Gap(){};
    Gap(Gap const&){};            // copy constructor is private
    Gap& operator=(Gap const&);   // assignment operator is private
};

/** @} */
}

}
