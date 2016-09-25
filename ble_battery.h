 /* 
  * This code is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#ifndef BATTERY_SERVICE_H__
#define BATTERY_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"


typedef struct
{
    uint16_t                    conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                    service_handle; /**< Handle of ble Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    battery_char_handles;   /**< Handles related to the our new characteristic. */
    uint8_t                     battery_level;
}ble_battery_t;


ret_code_t battery_level_measure_start(void);

/**@brief Function for handling BLE Stack events related to battery service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to battery Service.
 *
 * @param[in]   p_battery       ble_battery_t structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_battery_on_ble_evt(ble_battery_t * p_battery, ble_evt_t * p_ble_evt);

/**@brief Function for initializing our new service.
 *
 * @param[in]   p_battery       Pointer to ble battery structure.
 */
void ble_battery_service_init(ble_battery_t * p_battery);

/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_battery                     battery structure.
 * @param[in]   characteristic_value     New characteristic value.
 */
 void ble_battery_level_update(ble_battery_t *p_battery, uint8_t battery_level);



#endif  /* BATTERY_SERVICE_H__ */
