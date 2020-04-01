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

#ifdef __cplusplus
extern "C" {
#endif
#include "cy_result.h"
#include "wiced_defs.h"
#include "wiced_bt_hci.h"

/**
 * Bluetooth Management callback
 *
 * Callback for Bluetooth Management event notifications.
 * Registered using wiced_bt_stack_init()
 *
 * @param event             : Event ID
 * @param p_event_data      : Event data
 *
 * @return cy_rslt_t : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
typedef cy_rslt_t (wiced_bt_management_cback_t) (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);

/****************************************************************************/

/**
 * Function         wiced_bt_stack_init
 *
 *                  Initialize the Bluetooth controller and stack; register
 *                  callback for Bluetooth event notification.
 *
 * @param[in] p_bt_management_cback     : Callback for receiving Bluetooth management events
 *
 * @return cy_rslt_t : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
cy_rslt_t wiced_bt_stack_init(wiced_bt_management_cback_t *p_bt_management_cback);

/**
 * Function         wiced_bt_stack_deinit
 *
 *                  This function blocks until all de-initialisation procedures are complete.
 *                  It is recommended that the application disconnect any outstanding connections prior to invoking this function.
 *
 * @return cy_rslt_t : CY_RSLT_SUCCESS - on success, CY_RESULT_MW_ERROR otherwise
 */
cy_rslt_t wiced_bt_stack_deinit(void);

/**
 * Function         wiced_bt_set_local_bdaddr
 *
 *                  Set Local Bluetooth Device Address
 *
 * @param[in]      bd_addr    : device address to use
 *
 * @return          void
 *
 */
void wiced_bt_set_local_bdaddr( wiced_bt_device_address_t  bda );

/**
 * Function         wiced_bt_dev_read_local_addr
 *
 * Read the local device address
 *
 * @param[out]      bd_addr        : Local bd address
 *
 * @return          void
 *
 */
void wiced_bt_dev_read_local_addr (wiced_bt_device_address_t bd_addr);

#ifdef __cplusplus
} /* extern C */
#endif
