 /* 
  * This code is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#include <stdint.h>
#include <string.h>
#include <nrf_adc.h>
#include <nrf_drv_adc.h>
#include <ble_battery.h>
#include <app_util_platform.h>
#include "ble_srv_common.h"
#include "app_error.h"
#include "ble_battery.h"
#include "SEGGER_RTT.h"

#define BATTERY_CHAR_MAX_LEN    sizeof(uint8_t)

volatile bool battery_cccd_is_enabled;

static ble_battery_t *m_battery;

volatile uint8_t battery_level;

static uint8_t battery_level_convert(nrf_adc_value_t adc_value){

    uint16_t input_millivolts = (uint16_t) (((float) adc_value / 1023) * 1.2 * 3 * 1000); // Ref sel VBG = 1.2 V, Input scale 1/3, * 1000 for mV

    uint8_t battery_level_percent = battery_level_in_percent(input_millivolts);

    SEGGER_RTT_printf(0, "ADC: %d, mV: %d, bat level: %d\n", adc_value, input_millivolts, battery_level_percent);
    return battery_level_percent;
}

ret_code_t battery_level_measure_start()
{
    ret_code_t err;
    nrf_adc_value_t adc_value;

    err = nrf_drv_adc_buffer_convert(&adc_value, 1);
    if (err == NRF_ERROR_BUSY) {
        return NRF_ERROR_BUSY;
    }

    nrf_drv_adc_sample();
}

void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
    switch (p_event->type) {
        case NRF_DRV_ADC_EVT_DONE:
            battery_level = battery_level_convert(p_event->data.done.p_buffer[0]);
            ble_battery_level_update(m_battery, battery_level);
            break;
        case NRF_DRV_ADC_EVT_SAMPLE:
            break;
        default:
            break;
    }
}

static void adc_init(void)
{
    ret_code_t ret_code;
    nrf_drv_adc_config_t adc_config = NRF_DRV_ADC_DEFAULT_CONFIG;                                              //Get default ADC configuration
    static nrf_drv_adc_channel_t adc_channel_config = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_DISABLED);     //Get default ADC channel configuration

    adc_channel_config.config.config.input = NRF_ADC_CONFIG_SCALING_SUPPLY_ONE_THIRD;
//    adc_channel_config.config.config.ain = NRF_ADC_CONFIG_INPUT_DISABLED;

    ret_code = nrf_drv_adc_init(&adc_config, adc_event_handler);              //Initialize the ADC
    APP_ERROR_CHECK(ret_code);

    nrf_drv_adc_channel_enable(&adc_channel_config);                          //Configure and enable an ADC channel
}


 static void on_ble_write(ble_battery_t * p_battery, ble_evt_t * p_ble_evt)
 {
     uint32_t err_code;
     ble_gatts_value_t gatts_value;

     uint16_t cccd_value;
     gatts_value.len = sizeof(uint16_t);
     gatts_value.offset = 0;
     gatts_value.p_value = (uint8_t*)&cccd_value;


     if(p_ble_evt->evt.gatts_evt.params.write.handle == p_battery->battery_char_handles.cccd_handle)
     {
         err_code = sd_ble_gatts_value_get(p_battery->conn_handle, p_battery->battery_char_handles.cccd_handle, &gatts_value);
         APP_ERROR_CHECK(err_code);
         if(cccd_value == BLE_GATT_HVX_NOTIFICATION)
         {
             battery_cccd_is_enabled = true;
         }
         else
         {
             battery_cccd_is_enabled = false;
         }
     }
 }

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
            on_ble_write(p_battery, p_ble_evt);
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

    attr_char_value.p_value     = &p_battery->battery_level;

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

    m_battery = p_battery;

    adc_init();

//    err_code = battery_level_measure_start(); // Get initial battery level
//    APP_ERROR_CHECK(err_code);

    ble_uuid_t          service_uuid;
    BLE_UUID_BLE_ASSIGN(service_uuid, BLE_UUID_BATTERY_SERVICE);

    p_battery->conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_battery->service_handle);
    APP_ERROR_CHECK(err_code);
    
    ble_char_battery_add(p_battery);

}


void ble_battery_level_update(ble_battery_t *p_battery, uint8_t current_battery_level)
{

    static uint8_t previous_battery_level = 0;

    // Send value if connected and notifying
    if ((p_battery->conn_handle != BLE_CONN_HANDLE_INVALID) && (battery_cccd_is_enabled == true))
    {
        if (1)//previous_battery_level != current_battery_level)
        {
            uint16_t               len = sizeof(current_battery_level);
            ble_gatts_hvx_params_t hvx_params;
            memset(&hvx_params, 0, sizeof(hvx_params));

            hvx_params.handle = p_battery->battery_char_handles.value_handle;
            hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
            hvx_params.offset = 0;
            hvx_params.p_len  = &len;
            hvx_params.p_data = &current_battery_level;
            uint32_t err = sd_ble_gatts_hvx(p_battery->conn_handle, &hvx_params);
            if(err != NRF_SUCCESS)
                SEGGER_RTT_printf(0, "Temp HVX ERROR: %#x\n\n", err);
        }
    }
    previous_battery_level = current_battery_level;
}

