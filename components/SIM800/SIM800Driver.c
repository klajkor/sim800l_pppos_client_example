#include "SIM800Driver.h"

static const char tag[] = "SIM800Driver";

static SIM800_Command_s cmd_AT = {.command = "AT\r\n",
                                  .responseOnOk = SIM800_OK_Str, // handle multiple OK responses!
                                  .responseOnNotOk = {"1", "", ""},
                                  .numOfNotOkResponses = 1,
                                  .timeoutMs = 250,
                                  .delayMs = 50};

static SIM800_Command_s cmd_ATinfo = {.command = "ATI\r\n",
                                      .responseOnOk = "\r\nSIM800",
                                      .responseOnNotOk = {"1", "", ""},
                                      .numOfNotOkResponses = 1,
                                      .timeoutMs = 250,
                                      .delayMs = 50};

/**
 * @brief SIM800 Driver GPIO initialization
 */
SIM800Driver_RetVal_e SIM800Driver_SIM800_GPIO_Init(SIM800Driver_SIM800Config_s *pSIM800Modem_i)
{
    SIM800Driver_RetVal_e driverRetVal;
    driverRetVal = SIM800Driver_RetVal_OK;

    ESP_LOGI(tag, "Initializing SIM800 modem GPIO pins");
    do
    {
        if (gpioDriverPinInit_Output(&(pSIM800Modem_i->Pin_PWKEY)) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        if (gpioDriverPinInit_Output(&(pSIM800Modem_i->Pin_RST)) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        if (gpioDriverPinInit_Output(&(pSIM800Modem_i->Pin_POWERON)) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
    } while (0);
    vTaskDelay(pdMS_TO_TICKS(10));

    while (driverRetVal == SIM800Driver_RetVal_OK)
    {
        if (gpioDriverSetPin_Level(&(pSIM800Modem_i->Pin_POWERON), GPIO_DriverGPIOLevel_High) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
        if (gpioDriverSetPin_Level(&(pSIM800Modem_i->Pin_RST), GPIO_DriverGPIOLevel_High) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
        if (gpioDriverSetPin_Level(&(pSIM800Modem_i->Pin_PWKEY), GPIO_DriverGPIOLevel_Low) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
        if (gpioDriverSetPin_Level(&(pSIM800Modem_i->Pin_PWKEY), GPIO_DriverGPIOLevel_High) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(SIM800_PWKEY_PULSE_WAIT));
        if (gpioDriverSetPin_Level(&(pSIM800Modem_i->Pin_PWKEY), GPIO_DriverGPIOLevel_Low) != GPIO_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
            break;
        }
        break;
    }

    if (driverRetVal == SIM800Driver_RetVal_OK)
    {
        ESP_LOGI(tag, "Initializing SIM800 modem GPIO pins completed");
    }
    else
    {
        ESP_LOGE(tag, "Initializing SIM800 modem GPIO pins failed!");
    }

    return driverRetVal;
}

/**
 * @brief SIM800 Driver UART initialization
 */
SIM800Driver_RetVal_e SIM800Driver_SIM800_UART_Init(SIM800Driver_SIM800Config_s *pSIM800Modem_i)
{
    SIM800Driver_RetVal_e driverRetVal;
    driverRetVal = SIM800Driver_RetVal_OK;

    uint8_t rx_buffer[32];
    int     rx_bytes;

    if (driverRetVal == SIM800Driver_RetVal_OK)
    {
        ESP_LOGI(tag, "Initializing SIM800 modem UART");
        if (UART_DriverUARTInit(&(pSIM800Modem_i->SIM800_UART)) != UART_DriverRetVal_OK)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
        }
    }

    while (driverRetVal == SIM800Driver_RetVal_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(SIM800_INIT_WAIT));
        ESP_LOGI(tag, "Testing SIM800 modem's response to AT command");
        UART_DriverSendData(&(pSIM800Modem_i->SIM800_UART), cmd_AT.command, strlen(cmd_AT.command));
        vTaskDelay(pdMS_TO_TICKS(cmd_AT.delayMs));
        memset(rx_buffer, 0, sizeof(rx_buffer));
        rx_bytes = UART_DriverReceiveData(&(pSIM800Modem_i->SIM800_UART), rx_buffer, sizeof(rx_buffer) - 1);
        if (rx_bytes > 0 && strcmp((char *)rx_buffer, cmd_AT.responseOnOk) != 0)
        {
            driverRetVal = SIM800Driver_RetVal_NOK;
        }
        break;
    }

    if (driverRetVal == SIM800Driver_RetVal_OK)
    {
        ESP_LOGI(tag, "Initializing SIM800 modem UART completed");
    }
    else
    {
        ESP_LOGE(tag, "Initializing SIM800 modem UART failed!");
    }

    return driverRetVal;
}

/**
 * @brief SIM800 Driver initialization
 */
SIM800Driver_RetVal_e SIM800Driver_SIM800_Init(SIM800Driver_SIM800Config_s *pSIM800Modem_i)
{
    SIM800Driver_RetVal_e driverRetVal;
    driverRetVal = SIM800Driver_SIM800_GPIO_Init(pSIM800Modem_i);
    if (driverRetVal == SIM800Driver_RetVal_OK)
    {
        driverRetVal = SIM800Driver_SIM800_UART_Init(pSIM800Modem_i);
    }

    return driverRetVal;
}

SIM800Driver_RetVal_e SIM800Driver_SIM800_SendATcommand(SIM800Driver_SIM800Config_s *pSIM800Modem_i,
                                                        SIM800_Command_s *           pATcommand_i)
{
    SIM800Driver_RetVal_e driverRetVal;
    driverRetVal = SIM800Driver_RetVal_OK;
    if (UART_DriverSendData(&(pSIM800Modem_i->SIM800_UART), pATcommand_i->command, strlen(pATcommand_i->command)) ==
        UART_DriverRetVal_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(cmd_ATinfo.delayMs));
    }
    else
    {
        driverRetVal = SIM800Driver_RetVal_NOK;
    }
    return driverRetVal;
}

SIM800Driver_RetVal_e SIM800Driver_SIM800_GetModemInfo(SIM800Driver_SIM800Config_s *pSIM800Modem_i, char *pModemInfo_o)
{
    uint8_t rx_buffer[SIM800_MODEM_INFO_MAX_LEN];
    int     rx_bytes;
    uint8_t response_len;

    SIM800Driver_RetVal_e driverRetVal;
    driverRetVal = SIM800Driver_RetVal_OK;
    response_len = strlen(cmd_ATinfo.responseOnOk);

    ESP_LOGI(tag, "Testing SIM800 modem's response to ATI command");
    SIM800Driver_SIM800_SendATcommand(pSIM800Modem_i, &cmd_ATinfo);
    memset(rx_buffer, 0, sizeof(rx_buffer));
    rx_bytes = UART_DriverReceiveData(&(pSIM800Modem_i->SIM800_UART), rx_buffer, sizeof(rx_buffer) - 1);
    ESP_LOGI(tag, "Read %d bytes", rx_bytes);
    strcpy(pModemInfo_o, (char *)rx_buffer);
    if (rx_bytes > response_len)
    {
        rx_buffer[response_len] = 0;
    }
    if (strcmp((char *)rx_buffer, cmd_ATinfo.responseOnOk) != 0)
    {
        driverRetVal = SIM800Driver_RetVal_NOK;
    }
    return driverRetVal;
}
