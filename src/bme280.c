 /* 
  * The library is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "bme280.h"
#include "nrf_gpio.h"
#include "bme280_register_map.h"

static nrf_drv_twi_t const * m_twi_instance;
volatile static bool twi_tx_done = false;
volatile static bool twi_rx_done = false;

comp_param_t comp_params;

void bme280_twi_event_handler(const nrf_drv_twi_evt_t *evt)
{ 
    switch(evt->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            nrf_gpio_pin_toggle(24);
            switch(evt->xfer_desc.type)
            {
                case NRF_DRV_TWI_XFER_TX:
                    twi_tx_done = true;
                    break;
                case NRF_DRV_TWI_XFER_TXTX:
                    twi_tx_done = true;
                    break;
                case NRF_DRV_TWI_XFER_RX:
                    twi_rx_done = true;
                    break;
                case NRF_DRV_TWI_XFER_TXRX:
                    twi_rx_done = true;
                    break;
                default:
                    break;
            }
            break;
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            break;
        case NRF_DRV_TWI_EVT_DATA_NACK:
            break;
        default:
            break;
    }      
}

static void buffer_merger(uint8_t * new_buffer, uint8_t reg, uint8_t * p_data, uint32_t length)
{
    uint8_t *ptr_new_buffer;
    uint8_t *ptr_data_place_holder;
    
    ptr_data_place_holder = p_data;
    ptr_new_buffer = new_buffer;
    *ptr_new_buffer = reg;
    ptr_new_buffer++;
    
    for(int i = 0; i < length; i++)
    {
        *ptr_new_buffer = *ptr_data_place_holder;
        ptr_new_buffer++;
        ptr_data_place_holder++;
    }
}


static uint32_t bme280_write_burst(uint8_t reg, uint8_t * p_data, uint32_t length)
{    
    // This burst write function is not optimal and needs improvement. 
    // The new SDK 11 TWI driver is not able to do two transmits without repeating the ADDRESS + Write bit byte
    uint32_t err_code;
    uint32_t timeout = BME280_TWI_TIMEOUT;
    
    // Merging MPU register address and p_data into one buffer.
    uint8_t buffer[20];
    buffer_merger(buffer, reg, p_data, length);
    
    // Setting up transfer
    nrf_drv_twi_xfer_desc_t xfer_desc;
    xfer_desc.address = BME280_ADDRESS;
    xfer_desc.type = NRF_DRV_TWI_XFER_TX;
    xfer_desc.primary_length = length + 1;
    xfer_desc.p_primary_buf = buffer;
    
    // Transfering
    err_code = nrf_drv_twi_xfer(m_twi_instance, &xfer_desc, 0);
    
    while((!twi_tx_done) && --timeout);  
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;
    
    return err_code;
}

uint32_t bme280_write_register(uint8_t reg, uint8_t data)
{
    uint32_t err_code;
    uint32_t timeout = BME280_TWI_TIMEOUT;
    
    uint8_t packet[2] = {reg, data};
    
    err_code = nrf_drv_twi_tx(m_twi_instance, BME280_ADDRESS, packet, 2, false);
    if(err_code != NRF_SUCCESS) return err_code;
    
    while((!twi_tx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    
    twi_tx_done = false;
    
    return err_code;
}


uint32_t bme280_read_registers(uint8_t reg, uint8_t * p_data, uint32_t length)
{
    uint32_t err_code;
    uint32_t timeout = BME280_TWI_TIMEOUT;
    
    err_code = nrf_drv_twi_tx(m_twi_instance, BME280_ADDRESS, &reg, 1, false);
    if(err_code != NRF_SUCCESS) return err_code;
    
    while((!twi_tx_done) && --timeout);  
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;
    
    err_code = nrf_drv_twi_rx(m_twi_instance, BME280_ADDRESS, p_data, length);
    if(err_code != NRF_SUCCESS) return err_code;
    
    timeout = BME280_TWI_TIMEOUT;
    while((!twi_rx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_rx_done = false;
    
    return err_code;
}

uint32_t bme280_init(nrf_drv_twi_t const * const p_instance)
{
    m_twi_instance = p_instance;
    
    return NRF_SUCCESS;
}

uint32_t bme280_get_chip_id(uint8_t * chip_id)
{
    return bme280_read_registers(BME_REG_CHIP_ID, chip_id, 1);
}

uint32_t bme280_set_mode(oversampling_t temp, oversampling_t hum, oversampling_t press, operating_mode_t mode)
{
    uint32_t err;

    err = bme280_write_register(BME_REG_CTRL_HUM, hum);
    if(err != NRF_SUCCESS)
        return err;

    uint8_t reg = (temp << 5) | (press << 2) | mode;
    err = bme280_write_register(BME_REG_CTRL_MEAS, reg);
    if(err != NRF_SUCCESS)
        return err;

    return NRF_SUCCESS;
}

uint32_t bme280_get_press(uint32_t * preassure_u32)
{
    uint32_t err;   
    uint8_t raw_value[3] = {0};

    *preassure_u32 = 0;
    err = bme280_read_registers(BME_REG_PRESS_MSB, raw_value, 3);
    *preassure_u32 =   ((raw_value[0] << 16) | (raw_value[1] << 8) | (raw_value[2])) >> 4;
    return err;
}

uint32_t bme280_get_temp(uint32_t * temperature)
{
    uint32_t err;
    uint8_t raw_value[3] = {0};

    *temperature = 0;
    err = bme280_read_registers(BME_REG_TEMP_MSB, raw_value, 3);
    *temperature =   ((raw_value[0] << 16) | (raw_value[1] << 8) | (raw_value[2])) >> 4;
    return err;
}

uint32_t bme280_get_hum(uint32_t * humidity_u32)
{
    uint32_t err;
    uint8_t raw_value[2] = {0};

    *humidity_u32 = 0;
    err = bme280_read_registers(BME_REG_HUM_MSB, raw_value, 2);
    *humidity_u32 =   (raw_value[0] << 8) | raw_value[1];
    return err;   
}


uint32_t bme280_get_weather(weather_values_t * weather_values)
{
    uint32_t err;
    uint8_t raw_values[8] = {0};

    err = bme280_read_registers(BME_REG_PRESS_MSB, raw_values, sizeof(raw_values));

    weather_values->pressure = ((raw_values[0] << 16) | (raw_values[1] << 8) | raw_values[2]) >> 4;
    weather_values->temperature = ((raw_values[3] << 16) | (raw_values[4] << 8) | raw_values[5]) >> 4;
    weather_values->humidity = (raw_values[6] << 8) | raw_values[7];

    return err;   
}


uint32_t bme280_get_calibration_values()
{
    uint32_t err;
    uint8_t comp_params_raw[BME_REG_CALIB_REG_END - BME_REG_CALIB_REG_START + 1];
    uint8_t comp_params_raw_size = sizeof(comp_params_raw);

    err = bme280_read_registers(BME_REG_CALIB_REG_START, comp_params_raw, comp_params_raw_size);
    if(err != NRF_SUCCESS)
        return err;

    uint8_t i = comp_params_raw_size - 1;

    /**** HUMIDITY COMPENSATION VALUES ****/
    comp_params.hum.dig_H6 = comp_params_raw[i]; // Reg 0xE7
    comp_params.hum.dig_H5 = ((comp_params_raw[--i] << 4) | // 0xE6 @  [11:4]
                             (comp_params_raw[--i] >> 4)); // 0xE5[7:4] @ [3:0]
    comp_params.hum.dig_H4 = ((comp_params_raw[i] & 0x0F) | // 0xE5[3:0] @ [3:0]
                             (comp_params_raw[--i] << 4)); // Reg 0xE4 @ [11:]
    comp_params.hum.dig_H3 = comp_params_raw[--i]; // Reg 0xE3
    comp_params.hum.dig_H2 = ((comp_params_raw[--i] << 8) | // Reg 0xE2
                             (comp_params_raw[--i])); // Reg 0xE1
    i = 0xA1 - BME_REG_CALIB_REG_START; // Go to register 0xA1
    comp_params.hum.dig_H1 = comp_params_raw[i]; 
    // END OF HUMIDITY VALUES

    /**** TEMPERATURE AND PRESSURE VALUES ****/
    uint8_t * comp_params_ptr; 
    comp_params_ptr = (uint8_t*)&comp_params.temp;    

    for (int j = BME_REG_CALIB_REG_START; j <= BME_REG_DIG_P_END; j++)
    {
        *comp_params_ptr = comp_params_raw[j - BME_REG_CALIB_REG_START];
        comp_params_ptr++;
    }

    return NRF_SUCCESS;
}


/*
 * Returns temperature in DegC, resolution is 0.01 DegC. Output value of ‚Äú5123‚Äù equals 51.23 DegC.
 * t_fine carries fine temperature as global value
 */
int32_t t_fine;
uint32_t bme280_calibrate_temperature(int32_t raw_temperature_32)
{
    int32_t var1, var2, T;
    var1 = ((((raw_temperature_32>>3) - ((int32_t)comp_params.temp.dig_T1<<1))) * ((int32_t)comp_params.temp.dig_T2)) >> 11;
    var2 = (((((raw_temperature_32>>4) - ((int32_t)comp_params.temp.dig_T1)) * ((raw_temperature_32>>4) - ((int32_t)comp_params.temp.dig_T1))) >> 12) *
                ((int32_t)comp_params.temp.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;

    return T;
}

/* Returns pressure in Pa as unsigned 32 bit integer. Output value of 96386‚ equals 96386 Pa = 963.86 hPa */
uint32_t bme280_calibrate_pressure(int32_t raw_pressure_u32)
{
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)comp_params.press.dig_P6);
    var2 = var2 + ((var1*((int32_t)comp_params.press.dig_P5))<<1);
    var2 = (var2>>2)+(((int32_t)comp_params.press.dig_P4)<<16);
    var1 = (((comp_params.press.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)comp_params.press.dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((int32_t)comp_params.press.dig_P1))>>15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = (((uint32_t)(((int32_t)1048576)-raw_pressure_u32)-(var2>>12)))*3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((uint32_t)var1);
    }
    else
    {
        p = (p / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)comp_params.press.dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)comp_params.press.dig_P8))>>13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + comp_params.press.dig_P7) >> 4));

    return p;
}


/* 
 * Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
 * Output value of 47445ù represents 47445/1024 = 46.333 %RH
 */
uint32_t bme280_calibrate_humidity(int32_t raw_humidity_u32)
{
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((raw_humidity_u32 << 14) - (((int32_t)comp_params.hum.dig_H4) << 20) - (((int32_t)comp_params.hum.dig_H5) * v_x1_u32r)) +
                ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)comp_params.hum.dig_H6)) >> 10) * (((v_x1_u32r *
                ((int32_t)comp_params.hum.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
                ((int32_t)comp_params.hum.dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)comp_params.hum.dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return v_x1_u32r>>12;
}

/**
  @}
*/
