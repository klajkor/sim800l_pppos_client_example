#include "uartDriver.h"

static const char tag[] = "HAL uartDriver";

/**
 * @brief UART Driver initialization
 */
UART_DriverRetVal_e UART_DriverUARTInit(UART_DriverUARTConfig_s *pUARTPeripheral_i)
{
    UART_DriverRetVal_e ret_val;
    esp_err_t           esp_ret_val;
    uart_config_t       driver_uart_config;
    if (pUARTPeripheral_i != NULL)
    {
        ret_val = UART_DriverRetVal_OK;
        driver_uart_config.baud_rate = (int)pUARTPeripheral_i->UART_baud_rate;
        switch (pUARTPeripheral_i->UART_data_bits)
        {
        case 5:
            driver_uart_config.data_bits = UART_DATA_5_BITS;
            break;
        case 6:
            driver_uart_config.data_bits = UART_DATA_6_BITS;
            break;
        case 7:
            driver_uart_config.data_bits = UART_DATA_7_BITS;
            break;
        case 8:
            driver_uart_config.data_bits = UART_DATA_8_BITS;
            break;
        default:
            driver_uart_config.data_bits = UART_DATA_8_BITS;
            ret_val = UART_DriverRetVal_NOK;
            break;
        }
        switch (pUARTPeripheral_i->UART_stop_bits)
        {
        case 1:
            driver_uart_config.stop_bits = UART_STOP_BITS_1;
            break;
        case 2:
            driver_uart_config.stop_bits = UART_STOP_BITS_2;
            break;
        default:
            driver_uart_config.stop_bits = UART_STOP_BITS_1;
            ret_val = UART_DriverRetVal_NOK;
            break;
        }
        switch (pUARTPeripheral_i->UART_parity)
        {
        case UART_DriverParity_Even:
            driver_uart_config.parity = UART_PARITY_EVEN;
            break;
        case UART_DriverParity_Odd:
            driver_uart_config.parity = UART_PARITY_ODD;
            break;
        case UART_DriverParity_Disable:
            driver_uart_config.parity = UART_PARITY_DISABLE;
            break;
        default:
            driver_uart_config.parity = UART_PARITY_DISABLE;
            ret_val = UART_DriverRetVal_NOK;
            break;
        }
        driver_uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
        driver_uart_config.rx_flow_ctrl_thresh = 122;
    }
    else
    {
        ret_val = UART_DriverRetVal_NOK;
    }
    if (ret_val == UART_DriverRetVal_OK)
    {
        esp_ret_val = uart_param_config(pUARTPeripheral_i->UART_number, &driver_uart_config);
        if (esp_ret_val != ESP_OK)
        {
            ret_val = UART_DriverRetVal_NOK;
            ESP_LOGE(tag, "uart_param_config() error");
        }
    }
    if (ret_val == UART_DriverRetVal_OK)
    {
        esp_ret_val = uart_set_pin(pUARTPeripheral_i->UART_number, pUARTPeripheral_i->UART_TxGPIO_Pin,
                                   pUARTPeripheral_i->UART_RxGPIO_Pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        if (esp_ret_val != ESP_OK)
        {
            ret_val = UART_DriverRetVal_NOK;
            ESP_LOGE(tag, "uart_set_pin() error");
        }
    }

    if (ret_val == UART_DriverRetVal_OK)
    {
        esp_ret_val = uart_driver_install(pUARTPeripheral_i->UART_number, pUARTPeripheral_i->UART_RxBuff_Size,
                                          pUARTPeripheral_i->UART_TxBuff_Size, 0, NULL, 0);
        if (esp_ret_val != ESP_OK)
        {
            ret_val = UART_DriverRetVal_NOK;
            ESP_LOGE(tag, "uart_driver_install() error");
        }
    }
    return ret_val;
}

UART_DriverRetVal_e UART_DriverUARTDeInit(UART_DriverUARTConfig_s *pUARTPeripheral_i)
{
    UART_DriverRetVal_e ret_val;
    if (pUARTPeripheral_i != NULL)
    {
        uart_set_pin(pUARTPeripheral_i->UART_number, pUARTPeripheral_i->UART_TxGPIO_Pin,
                     pUARTPeripheral_i->UART_RxGPIO_Pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        if (uart_driver_delete(pUARTPeripheral_i->UART_number) == ESP_OK)
        {
            ret_val = UART_DriverRetVal_OK;
        }
        else
        {
            ret_val = UART_DriverRetVal_NOK;
        }
    }
    else
    {
        ret_val = UART_DriverRetVal_NOK;
    }
    return ret_val;
}

UART_DriverRetVal_e UART_DriverSendByte(UART_DriverUARTConfig_s *pUARTPeripheral_i, uint8_t data_i)
{
    UART_DriverRetVal_e ret_val;
    char                data_to_be_sent[2];
    int                 sent_bytes;
    data_to_be_sent[0] = data_i;
    data_to_be_sent[1] = 0;
    if (pUARTPeripheral_i != NULL)
    {
        sent_bytes = uart_write_bytes(pUARTPeripheral_i->UART_number, data_to_be_sent, 1);
        if (sent_bytes < 1)
        {
            ret_val = UART_DriverRetVal_NOK;
        }
        else
        {
            ret_val = UART_DriverRetVal_OK;
        }
    }
    else
    {
        ret_val = UART_DriverRetVal_NOK;
    }
    return ret_val;
}

UART_DriverRetVal_e UART_DriverSendData(UART_DriverUARTConfig_s *pUARTPeripheral_i, char *pData_i, uint16_t length_i)
{
    UART_DriverRetVal_e ret_val;
    int                 sent_bytes;
    if (pUARTPeripheral_i != NULL && pData_i != NULL)
    {
        sent_bytes = uart_write_bytes(pUARTPeripheral_i->UART_number, (char *)pData_i, length_i);
        if (sent_bytes < length_i)
        {
            ret_val = UART_DriverRetVal_NOK;
        }
        else
        {
            ret_val = UART_DriverRetVal_OK;
        }
    }
    else
    {
        ret_val = UART_DriverRetVal_NOK;
    }
    return ret_val;
}

int UART_DriverReceiveData(UART_DriverUARTConfig_s *pUARTPeripheral_i, uint8_t *pData_o, uint16_t length_i)
{
    int    ret_length;
    int    received_bytes;
    size_t bytes_in_buffer;
    if (pUARTPeripheral_i != NULL && pData_o != NULL && length_i > 0)
    {
        if (uart_get_buffered_data_len(pUARTPeripheral_i->UART_number, &bytes_in_buffer) == ESP_OK)
        {
            if (bytes_in_buffer > 0)
            {
                if (bytes_in_buffer < length_i)
                {
                    length_i = bytes_in_buffer;
                }
                received_bytes =
                    uart_read_bytes(pUARTPeripheral_i->UART_number, pData_o, length_i, UART_RX_TICKS_TO_WAIT);
                if (received_bytes >= 0)
                {
                    ret_length = received_bytes;
                }
                else
                {
                    ret_length = -1;
                }
            }
            else
            {
                ret_length = 0;
            }
        }
        else
        {
            ret_length = -1;
        }
    }
    else
    {
        ret_length = -1;
    }
    return ret_length;
}

int UART_DriverReceiveByte(UART_DriverUARTConfig_s *pUARTPeripheral_i, uint8_t *pData_o)
{
    return UART_DriverReceiveData(pUARTPeripheral_i, pData_o, 1);
}

/**
 * @brief Multiple UART Driver initialization
 */
UART_DriverRetVal_e UART_Driver_MultiInit(UART_DriverUARTConfig_s *pUARTsArray_i, uint8_t UARTsArrayLen_i)
{
    UART_DriverRetVal_e ret_val;
    uint8_t             i;
    ret_val = UART_DriverRetVal_OK;
    for (i = 0; (i < UARTsArrayLen_i && ret_val == UART_DriverRetVal_OK); i++)
    {
        ret_val = UART_DriverUARTInit(&pUARTsArray_i[i]);
    }
    return ret_val;
}

/**
 * @brief Multiple UART Driver deinitialization
 */
UART_DriverRetVal_e UART_Driver_MultiDeInit(UART_DriverUARTConfig_s *pUARTsArray_i, uint8_t UARTsArrayLen_i)
{
    UART_DriverRetVal_e ret_val;
    uint8_t             i;
    ret_val = UART_DriverRetVal_OK;
    for (i = 0; (i < UARTsArrayLen_i && ret_val == UART_DriverRetVal_OK); i++)
    {
        ret_val = UART_DriverUARTDeInit(&pUARTsArray_i[i]);
    }
    return ret_val;
}
