/*
* UART Driver for SmartAlarm system
*/

#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"

/*
* ESP32 default UART port pins
* UART 	    GPIO 	    UART 	GPIO
* U0_RXD 	GPIO3 	    U0_CTS 	GPIO19
* U0_TXD 	GPIO1 	    U0_RTS 	GPIO22
* U1_RXD 	GPIO9 	    U1_CTS 	GPIO6
* U1_TXD 	GPIO10 	    U1_RTS 	GPIO11
* U2_RXD 	GPIO16 	    U2_CTS 	GPIO8
* U2_TXD 	GPIO17 	    U2_RTS 	GPIO7
*/

/*
* ESP32 typdefs
* uart_port_t UART_NUMBER
* uart_config_t
*/

#define STD_UART_BUF_SIZE ((int)1024)
#define UART_RX_TICKS_TO_WAIT (pdMS_TO_TICKS(100))

/**
 * @brief UART Driver function return value enum
 */
typedef enum
{
    UART_DriverRetVal_OK = 0,
    UART_DriverRetVal_NOK,
    UART_DriverRetVal_End
} UART_DriverRetVal_e;

/**
 * @brief UART Driver parity enum
 */
typedef enum
{
    UART_DriverParity_Disable = 0x0, /*!< Disable UART parity*/
    UART_DriverParity_Even = 0x2,    /*!< Enable UART even parity*/
    UART_DriverParity_Odd = 0x3      /*!< Enable UART odd parity*/
} UART_DriverUARTParity_e;

/**
 * @brief UART Driver definition structure
 */
typedef struct
{
    uint8_t UART_number;
    uint32_t UART_baud_rate;
    uint8_t UART_data_bits;
    uint8_t UART_stop_bits;
    uart_parity_t UART_parity;
    int UART_TxBuff_Size;
    int UART_RxBuff_Size;
    uint32_t UART_TxGPIO_Pin;
    uint32_t UART_RxGPIO_Pin;
} UART_DriverUARTConfig_s;


/*
* TBD
static UART_Peripheral_s *UART_DriverGetPeripheralReference(UART_DriverUARTPeripheralList_e uartPeripheral_i)
{
    return &UART_DriverUARTPeripherals[uartPeripheral_i].Instance;
}
*/

/*
* TBD
1. Interrupt handling (Rx interrupt)
2. Polling mode
*/
UART_DriverRetVal_e UART_Driver_MultiInit(UART_DriverUARTConfig_s *pUARTsArray_i, uint8_t UARTsArrayLen_i);
UART_DriverRetVal_e UART_Driver_MultiDeInit(UART_DriverUARTConfig_s *pUARTsArray_i, uint8_t UARTsArrayLen_i);
UART_DriverRetVal_e UART_DriverUARTInit(UART_DriverUARTConfig_s *pUARTPeripheral_i);
UART_DriverRetVal_e UART_DriverUARTDeInit(UART_DriverUARTConfig_s *pUARTPeripheral_i);
UART_DriverRetVal_e UART_DriverSendByte(UART_DriverUARTConfig_s *pUARTPeripheral_i, uint8_t data_i);
UART_DriverRetVal_e UART_DriverSendData(UART_DriverUARTConfig_s *pUARTPeripheral_i, char *pData_i, uint16_t length_i);
int UART_DriverReceiveData(UART_DriverUARTConfig_s *pUARTPeripheral_i, uint8_t *pData_o, uint16_t length_i);
int UART_DriverReceiveByte(UART_DriverUARTConfig_s *pUARTPeripheral_i, uint8_t *pData_o);
