/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
/** @file
 *
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler and macros for utilizing a common error handler.
 */

#ifndef SHIFT_REG_H__
#define SHIFT_REG_H__

#include <stdlib.h>
#include <stdarg.h>
#include "Segger_RTT.h"

#ifdef MY_DEBUG
    #define MY_DEBUG_PRINTF(sFormat, ...)   \
    do                                      \
    {                                       \
        SEGGER_RTT_printf(0, sFormat, __VA_ARGS__);  \
    } while (0)
#else
    #define MY_DEBUG_PRINTF(sFormat, ...) ;
#endif
  
    
#ifdef MY_DEBUG
    #include "app_error.h"
    #define MY_APP_ERROR_CHECK(err_code)   \
    do                                      \
    {                                       \
        if(err_code != 0)   \
        {                       \
            SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_RED); \
            SEGGER_RTT_printf(0, "\n**** APP ERROR (NON BLOCKING) ****\nFile: %s\nLine nr: %d\nError: 0x%#x\n", __FILE__, __LINE__, err_code); \
            SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_WHITE); \
        }   \
    } while (0)
    
#else
    #define MY_APP_ERROR_CHECK(err_code) ;
#endif
    
#define MY_APP_ERROR_CHECK_NON_BLOCKING(err_code) MY_APP_ERROR_CHECK(err_code) // Legacy reasons

    
#ifdef MY_DEBUG   
    #include "app_error.h"
    #define MY_APP_ERROR_CHECK_BLOCKING(err_code)   \
    do                                      \
    {                                       \
        if(err_code != 0)   \
        {                       \
            SEGGER_RTT_printf(0, "\n**** APP ERROR (BLOCKING!) ****\nFile: %s\nLine nr: %d\nError: 0x%#x\n", __FILE__, __LINE__, err_code); \
            APP_ERROR_CHECK(err_code);  \
        }   \
    } while (0)
#else
    #define MY_APP_ERROR_CHECK_BLOCKING(err_code) do{((void)(err_code));}while(0);
#endif    
    

#endif // SHIFT_REG_H__
    
