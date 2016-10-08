 /* 
  * This code is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "ble_bme280.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "bme280.h"
#include "SEGGER_RTT.h"

#define PRESSURE_CHAR_LEN       sizeof(min_max_current_t)
#define HUMIDITY_CHAR_LEN       sizeof(min_max_current_t)
#define TEMPERATURE_CHAR_LEN    sizeof(min_max_current_t)

volatile bool temperature_cccd_is_enabled = false;
volatile bool pressure_cccd_is_enabled = false;
volatile bool humidity_cccd_is_enabled = false;


static void on_ble_write(ble_bme280_t * p_bme280, ble_evt_t * p_ble_evt)
{
    uint32_t err_code;
    ble_gatts_value_t gatts_value;

    //uint16_t write_handle = p_ble_evt->evt.gatts_evt.params.write.handle;

    uint16_t cccd_value;
    gatts_value.len = sizeof(uint16_t);
    gatts_value.offset = 0;
    gatts_value.p_value = (uint8_t*)&cccd_value;


    if(p_ble_evt->evt.gatts_evt.params.write.handle == p_bme280->temperature_char_handles.cccd_handle)
    {
        err_code = sd_ble_gatts_value_get(p_bme280->conn_handle, p_bme280->temperature_char_handles.cccd_handle, &gatts_value);
        APP_ERROR_CHECK(err_code);
        if(cccd_value == BLE_GATT_HVX_NOTIFICATION)
        {
            temperature_cccd_is_enabled = true;
        }
        else
        {
            temperature_cccd_is_enabled = false;
        }
    }
    else if(p_ble_evt->evt.gatts_evt.params.write.handle == p_bme280->humidity_char_handles.cccd_handle)
    {
        err_code = sd_ble_gatts_value_get(p_bme280->conn_handle, p_bme280->humidity_char_handles.cccd_handle, &gatts_value);
        APP_ERROR_CHECK(err_code);
        if(cccd_value == BLE_GATT_HVX_NOTIFICATION)
        {
            humidity_cccd_is_enabled = true;
        }
        else
        {
            humidity_cccd_is_enabled = false;
        }
    }
    else if(p_ble_evt->evt.gatts_evt.params.write.handle == p_bme280->pressure_char_handles.cccd_handle)
    {
        err_code = sd_ble_gatts_value_get(p_bme280->conn_handle, p_bme280->pressure_char_handles.cccd_handle, &gatts_value);
        APP_ERROR_CHECK(err_code);
        if(cccd_value == BLE_GATT_HVX_NOTIFICATION)
        {
            pressure_cccd_is_enabled = true;
        }
        else
        {
            pressure_cccd_is_enabled = false;
        }
    }
}

void ble_bme280_on_ble_evt(ble_bme280_t * p_bme280, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            p_bme280->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            p_bme280->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        case BLE_GATTS_EVT_WRITE:
            on_ble_write(p_bme280, p_ble_evt);
            break;
        default:
            // No implementation needed.
            break;
    }
}

/**@brief PRESSURE CHARACTERISTIC
 *
 * @param[in]   p_bme280        bme280 structure.
 *
 */
static uint32_t ble_char_pressure_add(ble_bme280_t * p_bme280)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    ble_uuid_t          char_uuid;   
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_PRESSURE_CHAR);
    
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
        
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;    
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    
    
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    attr_char_value.max_len     = PRESSURE_CHAR_LEN;
    attr_char_value.init_len    = PRESSURE_CHAR_LEN;
    uint8_t value[PRESSURE_CHAR_LEN] = {0};
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_bme280->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_bme280->pressure_char_handles);
    APP_ERROR_CHECK(err_code);   

    return NRF_SUCCESS;
}




/**@brief HUMIDITY CHARACTERISTIC
 *
 * @param[in]   p_bme280        bme280 structure.
 *
 */
static uint32_t ble_char_humidity_add(ble_bme280_t * p_bme280)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    ble_uuid_t          char_uuid;   
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_HUMIDITY_CHAR);
    
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
        
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;    
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    attr_char_value.max_len     = HUMIDITY_CHAR_LEN;
    attr_char_value.init_len    = HUMIDITY_CHAR_LEN;
    uint8_t value[HUMIDITY_CHAR_LEN]            = {0};
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_bme280->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_bme280->humidity_char_handles);
    APP_ERROR_CHECK(err_code);   

    return NRF_SUCCESS;
}




/**@brief TEMPERATURE CHARACTERISTIC
 *
 * @param[in]   p_bme280        bme280 structure.
 *
 */
static uint32_t ble_char_temperature_add(ble_bme280_t * p_bme280)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    ble_uuid_t          char_uuid;   
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_TEMPERATURE_CHAR);
    
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
        
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;    
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    attr_char_value.max_len     = TEMPERATURE_CHAR_LEN;
    attr_char_value.init_len    = TEMPERATURE_CHAR_LEN;
    uint8_t value[TEMPERATURE_CHAR_LEN]            = {0};
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_bme280->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_bme280->temperature_char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}


/**@brief Function for initiating our new service.
 *
 * @param[in]   p_bme280        Our Service structure.
 *
 */
void ble_bme280_service_init(ble_bme280_t * p_bme280)
{
    uint32_t   err_code; // Variable to hold return codes from library and softdevice functions

    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = BLE_UUID_BASE_UUID;
    service_uuid.uuid = BLE_UUID_BME280_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);    

    p_bme280->conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_bme280->service_handle);
    
    APP_ERROR_CHECK(err_code);
    
    ble_char_temperature_add(p_bme280);
    ble_char_humidity_add(p_bme280);
    ble_char_pressure_add(p_bme280);
}


void ble_bme280_temperature_update(ble_bme280_t *p_bme280, min_max_current_t *temperature)
{
    // Send value if connected and notifying
    if ((p_bme280->conn_handle != BLE_CONN_HANDLE_INVALID) && (temperature_cccd_is_enabled == true))
    {
        uint16_t               len = TEMPERATURE_CHAR_LEN;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_bme280->temperature_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t *)temperature;

        uint32_t err = sd_ble_gatts_hvx(p_bme280->conn_handle, &hvx_params);
        if(err != NRF_SUCCESS)
            SEGGER_RTT_printf(0, "Temp HVX ERROR: %#x\n\n", err);
    } 
}


void ble_bme280_humidity_update(ble_bme280_t *p_bme280, min_max_current_t *humidity)
{
    // Send value if connected and notifying
    if ((p_bme280->conn_handle != BLE_CONN_HANDLE_INVALID) && (humidity_cccd_is_enabled == true))
    {
        uint16_t               len = HUMIDITY_CHAR_LEN;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_bme280->humidity_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t *)humidity;

        uint32_t err = sd_ble_gatts_hvx(p_bme280->conn_handle, &hvx_params);
        if(err != NRF_SUCCESS)
            SEGGER_RTT_printf(0, "Hum HVX ERROR: %#x\n\n", err);
    }     
}


void ble_bme280_pressure_update(ble_bme280_t *p_bme280, min_max_current_t *pressure)
{
    // Send value if connected and notifying
    if ((p_bme280->conn_handle != BLE_CONN_HANDLE_INVALID) && (pressure_cccd_is_enabled == true))
    {
        nrf_gpio_pin_clear(22);
        uint16_t               len = PRESSURE_CHAR_LEN;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_bme280->pressure_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t *)pressure;

        uint32_t err = sd_ble_gatts_hvx(p_bme280->conn_handle, &hvx_params);
        if(err != NRF_SUCCESS)
            SEGGER_RTT_printf(0, "Press HVX ERROR: %#x\n\n", err);
        nrf_gpio_pin_set(22);
    }     
}
