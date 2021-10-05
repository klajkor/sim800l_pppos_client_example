/*
 * SIM800 Driver for smartAlarm system
 */

#pragma once

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "gpioDriver.h"
#include "uartDriver.h"

#define SIM800_INIT_WAIT (3500U)
#define SIM800_PWKEY_PULSE_WAIT (1300U)
#define SIM800_OK_Str ("\r\nOK\r\n")
#define SIM800_MODEM_INFO_MAX_LEN (64U)

typedef enum
{
    SIM800Driver_RetVal_OK = 0,
    SIM800Driver_RetVal_NOK,
    SIM800Driver_RetVal_End
} SIM800Driver_RetVal_e;

typedef struct
{
    GPIO_DriverGPIOConfig_s Pin_PWKEY;
    GPIO_DriverGPIOConfig_s Pin_RST;
    GPIO_DriverGPIOConfig_s Pin_POWERON;
    UART_DriverUARTConfig_s SIM800_UART;
} SIM800Driver_SIM800Config_s;

typedef struct
{
    char *   command;
    char *   responseOnOk;
    char *   responseOnNotOk[3];
    uint16_t numOfNotOkResponses;
    uint16_t timeoutMs;
    uint16_t delayMs;
} SIM800_Command_s;

SIM800Driver_RetVal_e SIM800Driver_SIM800_Init(SIM800Driver_SIM800Config_s *pSIM800Modem_i);
SIM800Driver_RetVal_e SIM800Driver_SIM800_GPIO_Init(SIM800Driver_SIM800Config_s *pSIM800Modem_i);
SIM800Driver_RetVal_e SIM800Driver_SIM800_SendATcommand(SIM800Driver_SIM800Config_s *pSIM800Modem_i,
                                                        SIM800_Command_s *           pATcommand_i);
SIM800Driver_RetVal_e SIM800Driver_SIM800_GetModemInfo(SIM800Driver_SIM800Config_s *pSIM800Modem_i,
                                                       char *                       pModemInfo_o);
