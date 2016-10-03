 /* 
  * This code is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#ifndef BME280_SERVICE_H__
#define BME280_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "bme280.h"
#include "ble_date_time.h"

#define BLE_UUID_BASE_UUID              {0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00} // 128-bit base UUID
#define BLE_UUID_BME280_SERVICE_UUID    0xF00D
#define BLE_UUID_HUMIDITY_CHAR          0x01D1
#define BLE_UUID_PRESSURE_CHAR          0xE55E
#define BLE_UUID_TEMPERATURE_CHAR       0xEEAE

typedef struct
{
    uint16_t                    conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                    service_handle; /**< Handle of ble Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    temperature_char_handles;   /**< Handles related to the our new characteristic. */
    ble_gatts_char_handles_t    humidity_char_handles;   /**< Handles related to the our new characteristic. */
    ble_gatts_char_handles_t    pressure_char_handles;
}ble_bme280_t;


/**@brief Temperature Service measurement structure. This contains 
 *        measurement. */
//typedef struct ble_hts_meas_s
//{

//} ble_temp_meas_t;

/**@brief Function for handling BLE Stack events related to bme280 service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to bme280 Service.
 *
 * @param[in]   p_bme280       bme280 structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_bme280_on_ble_evt(ble_bme280_t * p_bme280, ble_evt_t * p_ble_evt);

/**@brief Function for initializing our new service.
 *
 * @param[in]   p_bme280       Pointer to ble bme280 structure.
 */
void ble_bme280_service_init(ble_bme280_t * p_bme280);

/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_bme280                     bme280 structure.
 * @param[in]   characteristic_value     New characteristic value.
 */
void ble_bme280_temperature_update(ble_bme280_t *p_bme280, uint32_t * temperature_raw);

/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_bme280                     bme280 structure.
 * @param[in]   characteristic_value     New characteristic value.
 */
void ble_bme280_humidity_update(ble_bme280_t *p_bme280, uint32_t * humidity_raw);


/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_bme280                     bme280 structure.
 * @param[in]   characteristic_value     New characteristic value.
 */
void ble_bme280_pressure_update(ble_bme280_t *p_bme280, uint32_t * pressure_raw);


#endif  /* _ BME280_SERVICE_H__ */
