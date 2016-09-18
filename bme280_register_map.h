

 /* MPU-6000/MPU-6050 Register Map
  * Revision: 4.0
  * Release Date: 03/09/2012
  *
  * This code is not extensively tested and only 
  * meant as a simple explanation and for inspiration. 
  * THE LIST OF REGISTERS MIGHT NOT BE ACCURATE
  * NO WARRANTY of ANY KIND is provided. 
  */
  
#ifndef BME280_REG_MAP_H
#define BME280_REG_MAP_H

#define BME_REG_HUM_LSB			0XFE
#define BME_REG_HUM_MSB			0xFD
#define BME_REG_TEMP_XLSB		0xFC
#define BME_REG_TEMP_LSB 		0xFB
#define BME_REG_TEMP_MSB		0xFA
#define BME_REG_PRESS_XLSB		0xF9
#define BME_REG_PRESS_LSB 		0xF8
#define BME_REG_PRESS_MSB		0xF7
#define BME_REG_CONFIG			0xF5
#define BME_REG_CTRL_MEAS		0xF4
#define BME_REG_STATUS			0xF3
#define BME_REG_CTRL_HUM		0xF2

#define BME_REG_RESET			0xE0
#define BME_REG_CHIP_ID         0xD0 // Chip ID Register 

#define BME_REG_CALIB_REG_START 0x88
#define BME_REG_DIG_T_START		0x88
#define BME_REG_DIG_T_END		0x8D

#define BME_REG_DIG_P_START		0x8E
#define BME_REG_DIG_P_END		0x9F

#define BME_REG_DIG_H1			0xA1
#define BME_REG_DIG_H2_LSB		0xE1
#define BME_REG_DIG_H2_MSB		0xE2
#define BME_REG_DIG_H3			0xE3
#define BME_REG_DIG_H4_11_4		0xE4	// dig_H4 [11:4] 
#define BME_REG_DIG_H4_3_0		0xE5	// dig_H4 [3:0] 
#define BME_REG_DIG_H5_3_0		0xE5	// dig_H5 [3:0] 
#define BME_REG_DIG_H5_11_4		0xE6	// dig_H5 [11:4] 
#define BME_REG_DIG_H6			0xE7


#define BME_REG_CALIB_REG_END   0xE7

#define BME_REG_CALIB_00		0x88
#define BME_REG_CALIB_01		0x89
#define BME_REG_CALIB_02		0x8A
#define BME_REG_CALIB_03		0x8B
#define BME_REG_CALIB_04		0x8C
#define BME_REG_CALIB_05		0x8D
#define BME_REG_CALIB_06		0x8E
#define BME_REG_CALIB_07		0x8F
#define BME_REG_CALIB_08		0x90
#define BME_REG_CALIB_09		0x91
#define BME_REG_CALIB_10		0x92
#define BME_REG_CALIB_11		0x93
#define BME_REG_CALIB_12		0x94
#define BME_REG_CALIB_13		0x95
#define BME_REG_CALIB_14		0x96
#define BME_REG_CALIB_15		0x97
#define BME_REG_CALIB_16		0x98
#define BME_REG_CALIB_17		0x99
#define BME_REG_CALIB_18		0x9A
#define BME_REG_CALIB_19		0x9B
#define BME_REG_CALIB_20		0x9C
#define BME_REG_CALIB_21		0x9D
#define BME_REG_CALIB_22		0x9E
#define BME_REG_CALIB_23		0x9F
#define BME_REG_CALIB_24		0xA0
#define BME_REG_CALIB_25		0xA1

#define BME_REG_CALIB_26		0xE1
#define BME_REG_CALIB_27		0xE2
#define BME_REG_CALIB_28		0xE3
#define BME_REG_CALIB_29		0xE4
#define BME_REG_CALIB_30		0xE5
#define BME_REG_CALIB_31		0xE6
#define BME_REG_CALIB_32		0xE7
#define BME_REG_CALIB_33		0xE8
#define BME_REG_CALIB_34		0xE9
#define BME_REG_CALIB_35		0xEA
#define BME_REG_CALIB_36		0xEB
#define BME_REG_CALIB_37		0xEC
#define BME_REG_CALIB_38		0xED
#define BME_REG_CALIB_39		0xEE
#define BME_REG_CALIB_40		0xEF
#define BME_REG_CALIB_41		0xF0


#endif // BME280_REG_MAP_H
