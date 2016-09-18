 /* 
  * This code is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "ble_battery.h"
#include "SEGGER_RTT.h"

#define BATTERY_CHAR_MAX_LEN    sizeof(uint8_t)
 
volatile bool battery_cccd_is_enabled, pressure_cccd_is_enabled, humidity_cccd_is_enabled;


void ble_battery_on_ble_evt(ble_battery_t * p_battery, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            p_battery->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            p_battery->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        case BLE_GATTS_EVT_WRITE:
            break;
        default:
            // No implementation needed.
            break;
    }
}


/**@brief battery CHARACTERISTIC
 *
 * @param[in]   p_battery        battery structure.
 *
 */
static uint32_t ble_char_battery_add(ble_battery_t * p_battery)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    ble_uuid_t          char_uuid;   
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_BATTERY_LEVEL_CHAR);
    
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
    attr_char_value.max_len     = BATTERY_CHAR_MAX_LEN;
    attr_char_value.init_len    = BATTERY_CHAR_MAX_LEN;
    uint8_t value[BATTERY_CHAR_MAX_LEN]            = {0};
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_battery->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_battery->battery_char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}


/**@brief Function for initiating our new service.
 *
 * @param[in]   p_battery        Battery Service structure.
 *
 */
void ble_battery_service_init(ble_battery_t * p_battery)
{
    uint32_t   err_code; // Variable to hold return codes from library and softdevice functions

    ble_uuid_t          service_uuid;
    BLE_UUID_BLE_ASSIGN(service_uuid, BLE_UUID_BATTERY_SERVICE);

    p_battery->conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_battery->service_handle);
    APP_ERROR_CHECK(err_code);
    
    ble_char_battery_add(p_battery);
}


void ble_battery_level_update(ble_battery_t *p_battery, uint8_t battery_level)
{
    // Send value if connected and notifying
    if ((p_battery->conn_handle != BLE_CONN_HANDLE_INVALID) && (battery_cccd_is_enabled == true))
    {
        uint16_t               len = sizeof(battery_level);
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_battery->battery_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = &battery_level;
        uint32_t err = sd_ble_gatts_hvx(p_battery->conn_handle, &hvx_params);
        if(err != NRF_SUCCESS)
            SEGGER_RTT_printf(0, "Temp HVX ERROR: %#x\n\n", err);
    } 
}

