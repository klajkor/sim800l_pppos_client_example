#include "gpioDriver.h"

GPIO_DriverRetVal_e gpioDriverPinInit_Input(GPIO_DriverGPIOConfig_s *pGPIOPin_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    gpio_config_t gpio_conf;

    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_conf.mode = GPIO_DriverGPIO_MODE_INPUT;
    gpio_conf.pin_bit_mask = (1ULL << pGPIOPin_i->GPIO_Pin_Number);
    gpio_conf.pull_up_en = 0;
    gpio_conf.pull_down_en = 0;
    if ((pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLUP_ONLY) || (pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLUP_PULLDOWN))
    {
        gpio_conf.pull_up_en = 1;
    }
    if ((pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLDOWN_ONLY) || (pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLUP_PULLDOWN))
    {
        gpio_conf.pull_up_en = 1;
    }
    if (gpio_config(&gpio_conf) == ESP_OK)
    {
        driverRetVal = GPIO_DriverRetVal_OK;
    }
    else
    {
        driverRetVal = GPIO_DriverRetVal_NOK;
    }

    return driverRetVal;
}

GPIO_DriverRetVal_e gpioDriverPinInit_Output(GPIO_DriverGPIOConfig_s *pGPIOPin_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    gpio_config_t gpio_conf;

    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_conf.mode = GPIO_DriverGPIO_MODE_OUTPUT;
    gpio_conf.pin_bit_mask = (1ULL << pGPIOPin_i->GPIO_Pin_Number);
    gpio_conf.pull_up_en = 0;
    gpio_conf.pull_down_en = 0;
    if ((pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLUP_ONLY) || (pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLUP_PULLDOWN))
    {
        gpio_conf.pull_up_en = 1;
    }
    if ((pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLDOWN_ONLY) || (pGPIOPin_i->GPIO_PullUpDown_Selector == GPIO_DriverGPIO_PULLUP_PULLDOWN))
    {
        gpio_conf.pull_up_en = 1;
    }
    if (gpio_config(&gpio_conf) == ESP_OK)
    {
        driverRetVal = GPIO_DriverRetVal_OK;
    }
    else
    {
        driverRetVal = GPIO_DriverRetVal_NOK;
    }

    return driverRetVal;
}

GPIO_DriverRetVal_e gpioDriverPinDeInit(GPIO_DriverGPIOConfig_s *pGPIOPin_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    gpio_config_t gpio_conf;

    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_conf.mode = GPIO_MODE_DISABLE;
    gpio_conf.pin_bit_mask = (1ULL << pGPIOPin_i->GPIO_Pin_Number);
    gpio_conf.pull_up_en = 0;
    gpio_conf.pull_down_en = 0;
    if (gpio_config(&gpio_conf) == ESP_OK)
    {
        driverRetVal = GPIO_DriverRetVal_OK;
    }
    else
    {
        driverRetVal = GPIO_DriverRetVal_NOK;
    }
    return driverRetVal;
}

GPIO_DriverRetVal_e gpioDriverSetPin_Level(GPIO_DriverGPIOConfig_s *pGPIOPin_i, GPIO_DriverGPIOLevel_e eGPIOLevel_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    if (gpio_set_level(pGPIOPin_i->GPIO_Pin_Number, eGPIOLevel_i) == ESP_OK)
    {
        driverRetVal = GPIO_DriverRetVal_OK;
    }
    else
    {
        driverRetVal = GPIO_DriverRetVal_NOK;
    }
    return driverRetVal;
}

GPIO_DriverRetVal_e gpioDriverTogglePin(GPIO_DriverGPIOConfig_s *pGPIOPin_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    GPIO_DriverGPIOLevel_e new_GPIOLevel;
    if (gpio_get_level(pGPIOPin_i->GPIO_Pin_Number) == GPIO_DriverGPIOLevel_Low)
    {
        new_GPIOLevel = GPIO_DriverGPIOLevel_High;
    }
    else
    {
        new_GPIOLevel = GPIO_DriverGPIOLevel_Low;
    }
    if (gpio_set_level(pGPIOPin_i->GPIO_Pin_Number, new_GPIOLevel) == ESP_OK)
    {
        driverRetVal = GPIO_DriverRetVal_OK;
    }
    else
    {
        driverRetVal = GPIO_DriverRetVal_NOK;
    }
    return driverRetVal;
}

GPIO_DriverRetVal_e gpioDriverReadPin(GPIO_DriverGPIOConfig_s *pGPIOPin_i, uint8_t *pData_o)
{
    *pData_o = (uint8_t)gpio_get_level(pGPIOPin_i->GPIO_Pin_Number);
    return GPIO_DriverRetVal_OK;
}

GPIO_DriverRetVal_e gpioDriver_Init(GPIO_DriverGPIOConfig_s *pGPIOPinsArray_i, uint8_t GPIOPinsArrayLen_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    uint8_t i;
    driverRetVal = GPIO_DriverRetVal_OK;
    for (i = 0; (i < GPIOPinsArrayLen_i && driverRetVal == GPIO_DriverRetVal_OK); i++)
    {
        if (pGPIOPinsArray_i[i].GPIO_Mode == GPIO_DriverGPIO_MODE_OUTPUT)
        {
            driverRetVal = gpioDriverPinInit_Output(&pGPIOPinsArray_i[i]);
        }
        else
        {
            driverRetVal = gpioDriverPinInit_Input(&pGPIOPinsArray_i[i]);
        }
    }
    return driverRetVal;
}

GPIO_DriverRetVal_e gpioDriver_DeInit(GPIO_DriverGPIOConfig_s *pGPIOPinsArray_i, uint8_t GPIOPinsArrayLen_i)
{
    GPIO_DriverRetVal_e driverRetVal;
    uint8_t i;
    driverRetVal = GPIO_DriverRetVal_OK;
    for (i = 0; (i < GPIOPinsArrayLen_i && driverRetVal == GPIO_DriverRetVal_OK); i++)
    {
        driverRetVal = gpioDriverPinDeInit(&pGPIOPinsArray_i[i]);
    }
    return driverRetVal;
}
