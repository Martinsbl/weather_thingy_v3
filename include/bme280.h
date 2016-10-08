 /* 
  * The library is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * NO WARRANTY of ANY KIND is provided. 
  */

#ifndef BME280_H_
#define BME280_H_

#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_twi.h"


#define BME280_TWI_TIMEOUT 1500000 
#define BME280_ADDRESS     0x76

#define BME280_ERR_CODE_BASE    0x1700
#define BME280_ERR_ADDRESS_NACK (BME280_ERR_CODE_BASE + 1)
#define BME280_ERR_DATA_NACK    (BME280_ERR_CODE_BASE + 2)
#define BME280_ERR_TWI_TIMEOUT      (BME280_ERR_CODE_BASE + 13)


typedef struct
{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
}dig_temp_t;

typedef struct
{
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
}dig_press_t;

typedef struct
{
	int8_t  dig_H6;
	int16_t dig_H5;
	int16_t dig_H4;
	uint8_t dig_H3;
	int16_t dig_H2;
	uint8_t dig_H1;
}dig_hum_t;


typedef struct
{
    dig_temp_t 	temp;
    dig_press_t press;
    dig_hum_t 	hum;

}comp_param_t;


typedef enum{
    SLEEP_MODE = 0,
    FORCE_MEAS,
    NORMAL_MODE = 3,
}operating_mode_t;

typedef enum{
	SKIPPED = 0, // Ouput = 0x8000000
	OVERSAMPLING_1,
	OVERSAMPLING_2,
	OVERSAMPLING_4,
	OVERSAMPLING_8,
	OVERSAMPLING_16,
}oversampling_t;


typedef struct 
{
    uint32_t humidity;
    uint32_t pressure;
    uint32_t temperature;
}weather_values_t;


typedef struct
{
    weather_values_t current;
    weather_values_t max;
    weather_values_t min;
}weather_values_minmax_t;


/**@brief Event handler used to pass TWI events from main application to the BME280 library
 *
 * @param[in]   evt             TWI driver event
 * @retval      uint32_t        Error code
 */
void bme280_twi_event_handler(const nrf_drv_twi_evt_t *evt);

/**@brief Function for initiating BME280
 * 
 *
 * @param[in]   p_instance      Instance of nRF5x TWI module
 * @retval      uint32_t        Error code
 */
uint32_t bme280_init(nrf_drv_twi_t const * const p_instance);

/**@brief Function for reading an arbitrary register
 *
 * @param[in]   reg             Register to write
 * @param[in]   data            Value
 * @retval      uint32_t        Error code
 */
uint32_t bme280_write_register(uint8_t reg, uint8_t data);

/**@brief Function for reading arbitrary register(s)
 *
 * @param[in]   reg             Register to read
 * @param[in]   p_data          Pointer to place to store value(s)
 * @param[in]   length          Number of registers to read
 * @retval      uint32_t        Error code
 */
uint32_t bme280_read_registers(uint8_t reg, uint8_t * p_data, uint32_t length);


/**@brief Function to get BME280 chip ID. Should be 0x60.
 *
 * @param[in]   chip_id         Pointer to store chip ID in
 * @retval      uint32_t        Error code
 */    
uint32_t bme280_get_chip_id(uint8_t * chip_id);


/**@brief Function to set temperature, humidity, and pressure oversampling value. 
 * If overampling value i zero measurement is skipped. 
 *
 * @param[in]   temp            Temperature oversampling value
 * @param[in]   hum             Humidity oversampling value
 * @param[in]   press           Pressure oversampling value
 * @param[in]   mode            BME280 Operating mode
 * @retval      uint32_t        Error code
 */    
uint32_t bme280_set_mode(oversampling_t temp, oversampling_t hum, oversampling_t press, operating_mode_t mode);


/**@brief Function to read pressure
 *
 * @param[in]   raw_pressure_u32    Pointer to unsigned 32 bit integer where raw pressure value is to be stored
 * @retval      uint32_t            Error code
 */    
uint32_t bme280_get_press(uint32_t * raw_pressure_u32);


/**@brief Function to read temperature
 *
 * @param[in]   raw_temperature_u32    Pointer to unsigned 32 bit integer where raw temeperature is to be stored
 * @retval      uint32_t               Error code
 */    
uint32_t bme280_get_temp(uint32_t * raw_temperature_32);


/**@brief Function to read humidity
 *
 * @param[in]   raw_humidity_u32    Pointer to unsigned 32 bit integer where raw humidity is to be stored
 * @retval      uint32_t            Error code
 */    
uint32_t bme280_get_hum(uint32_t * raw_humidity_u32);



/**@brief Function to read humidity, temperature and pressure
 *
 * @param[in]   raw_humidity_u32    Pointer to weather value structure
 * @retval      uint32_t            Error code
 */    
uint32_t bme280_get_weather(weather_values_t * weather_values);



/**@brief Function to retrive compensation values
 * Compensation values are stored in local variable
 *
 * @retval      uint32_t            Error code
 */    
uint32_t bme280_get_calibration_values(void);



/**@brief Function to calibrate temperature value
 * Returns temperature in DegC, resolution is 0.01 DegC. Output value of 5123 equals 51.23 DegC.
 * t_fine carries fine temperature as global value
 *
 * @param[in]   raw_temperature_u32    Raw temperature value
 * @retval      float               Calibrated temperature value
 */    
uint32_t bme280_calibrate_temperature(int32_t raw_temperature_32);


/**@brief Function to calibrate pressure value
 * Returns pressure in Pa as unsigned 32 bit integer. Output value of 96386 equals 96386 Pa = 963.86 hPa
 *
 * @param[in]   raw_pressure_u32    Raw pressure value
 * @retval      float               Calibrated pressure value
 */    
uint32_t bme280_calibrate_pressure(int32_t raw_pressure_u32);


/**@brief Function to calibrate humidity value
 * Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
 * Output value of 47445 represents 47445/1024 = 46.333 %RH
 *
 * @param[in]   raw_humidity_u32    Raw humidity value
 * @retval      uint32_t               Calibrated humidity value
 */    
uint32_t bme280_calibrate_humidity(int32_t raw_humidity_u32);

#endif /* BME280_H_ */

/**
  @}
*/

