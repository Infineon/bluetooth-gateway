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

/**
 * @file
 *
 * Embedded Bluetooth Stack APIs
 *
 * This header file exposes APIs for an application developer to control and interact with
 * Cypress' Bluetooth controller running Bluetooth Embedded application. The user should be
 * able to issue commands and receive events from Bluetooth controller using different service
 * sub-classes like GAP, Mesh etc. The APIs/Classes are in-line with mbed-OS
 * class based design philosophy and tries to imitate look-and-feel of Cordio BLE Stack APIs.
 * For example - cypress::embedded:GAP class provides similar methods to globally-available
 * Cordio Stack GAP class.
 *
 * @warning @note Cordio BLE Interface and Cypress' Embedded Bluetooth Interface are not compatible.
 * Application developer MUST use either Cordio or Embedded BLE instance, NOT BOTH simultaenously.
 *
 *
 */

#pragma once

#include <stdint.h>
#include "ble/blecommon.h"

/**
 * \defgroup embedded_ble Embedded BLE Library
 */

namespace cypress
{
namespace embedded
{

class Mesh;
class Gap;

/**
 * @addtogroup embedded_ble
 *
 * Embedded BLE Interface
 * @{
 */

/** Defines BLE Interface Initialization callback */
typedef void (*ble_init_callback_t)(void);

/** Defines Embedded BLE Class */
class BLE
{

public:
    /**
     * Opaque type used to store the ID of a BLE instance.
     */
    typedef unsigned InstanceID_t;

    /**
     * The value of the BLE::InstanceID_t for the default BLE instance.
     */
    static const InstanceID_t DEFAULT_INSTANCE = 0;

    /**
     * Get a reference to the Embedded BLE singleton corresponding to a given interface.
     *
     * @note Calling Instance() is preferred over constructing a BLE object
     * directly because it returns references to singleton.
     *
     * @return A reference to a single object.
     *
     */
    static BLE &Instance(void)
    {
        if (Eble == NULL)
        {
            Eble = new BLE;
        }
        return (BLE &)*Eble;
    }

    /**
     * Fetch the ID of a BLE instance.
     *
     * @return Instance id of this BLE instance.
     */
    InstanceID_t getInstanceID(void) const {
        return (InstanceID_t)DEFAULT_INSTANCE;
    }

    /**
     * Initialize the Embedded BLE stack/controller
     */
    ble_error_t init(void (*callback)(void));

    /**
     * Shutdown the Embedded BLE stack/controller
     */
    ble_error_t shutdown(void);

    /**
     * Reset the Embedded BLE stack/controller - think of it is a s/w reset button for BT controller
     */
    ble_error_t reset(void);

    /**
     * Returns whether the Embedded Bluetooth stack has been initialized or not.
     */
    inline int hasInitialized(void)
    {
        return initialized;
    }

    /**
     * Sets Embedded Bluetooth stack status to True(1).
     */
    inline void setInitialized(void)
    {
        initialized = 1;
    }

    /**
     * Returns Embedded Bluetooth stack initialization-callback method
     */
    inline ble_init_callback_t get_initCallback(void)
    {
        return init_callback;
    }

    /**
     * Returns Mesh instance associated with BLE
     */
    Mesh& mesh();
    /**
     * Returns GAP instance associated with BLE
     */
    Gap& gap();

    /**
     * Translate error code into a printable string.
     *
     * @param[in] error Error code returned by BLE functions.
     *
     * @return A pointer to a const string describing the error.
     */
    static const char* errorToString(ble_error_t error);

private:
    int initialized;
    void (*init_callback)(void);

    // Private so that it can not be called outside from class
    BLE(): initialized(0), init_callback(NULL)
    {
    };

    BLE(BLE const&): initialized(0), init_callback(NULL){};             // copy constructor is private
    BLE& operator=(BLE const&);    // assignment operator is private
    static BLE* Eble;
};

/** @} */
}

}

