# `stm32-uart-logger`

UART-based logging module for STM32 devices using the STM32 HAL API. Designed and tested
on the STM32F1 Blue Pill board, but compatible with other STM32 MCUs.

## Description

This module provides logging with severity levels (DEBUG, INFO, WARNING, ERROR)
over UART using `HAL_UART_Transmit()` in blocking mode.

It is intended for bare-metal or RTOS projects on STM32 MCUs, and requires the USART1 peripheral
to be configured and enabled externally.

There are **two modes of usage**:

* **Global logging** using only `logger.h`.
* **Zephyr like per-module logging** using `logger_module.h`.

## Features

* Runtime configurable log levels.
* Formatted messages with function name and line number.
* ANSI colors for compatible terminals (only INFO, WARNING, and ERROR).
* Easy to use via macros (`LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR`, `LOG_RAW`).
* Zephyr like per-module severity filtering support.


## Usage

### 1. Basic Global Logging

1. Include `logger.h` in your source file.

2. Set the global log level if you want to filter messages:

```c
LOGGER_SET_LOGGING_LEVEL(LOG_LEVEL_INFO);
```

3. Use the macros to log messages:

```c
LOG_DEBUG("Value x: %d\r\n", x);
LOG_INFO("System started\r\n");
LOG_WARNING("High temperature: %d°C\r\n", temp);
LOG_ERROR("Critical error: %s\r\n", error_msg);
LOG_RAW("Unformatted message\r\n");
```

### 2. Per-Module Logging (Optional)

#### Registering a module

In the `.c` file of the module:

```c
#include "logger_module.h"

LOG_MODULE_REGISTER(device01, LOG_LEVEL_DEBUG);

void device01_init(void) {
    LOG_INFO("Device01 initialized\r\n");
}
```

#### Using the same module in another file

```c
#include "logger_module.h"

LOG_MODULE_DECLARE(device01);

void some_other_function(void) {
    LOG_WARNING("This warning belongs to the device01 module\r\n");
}
```

#### Adjusting a module log level at runtime

```c
LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(device01), LOG_LEVEL_OFF);  // disable logs for device01
LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(device01), LOG_LEVEL_INFO); // re-enable
```

## Requirements

* STM32 HAL enabled.
* `UART_HandleTypeDef huart1` defined and initialized before calling any logger macros.

## Example

```c
// test.c

#include "logger_module.h"

LOG_MODULE_REGISTER(test, LOG_LEVEL_DEBUG);

void TestFunc(void){
    LOG_WARNING("Warning from test module\r\n");
}

```

```c
// main.c
#include "logger_module.h"
#include "test.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DEBUG);
LOG_MODULE_EXTERN(test);

// ...
int main(void)
{
    // ...
    LOGGER_SET_LOGGING_LEVEL(LOG_LEVEL_WARNING);

    while (1)
    {
        /* USER CODE END WHILE */
        for (uint8_t i = 0; i < 5; i++)
        {
            LOG_DEBUG("Debug msg iter: %u\r\n", i);
            LOG_INFO("Info msg iter: %u\r\n", i);
            LOG_WARNING("Warning msg iter: %u\r\n", i);
            LOG_ERROR("Error msg iter: %u\r\n", i);

            TestFunc();
            LOG_RAW("\n");

            if (i == 2)
            {
                LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(test), LOG_LEVEL_OFF);
                LOG_ERROR("disabling test module on iteration %u... \r\n", i);
            }

            HAL_Delay(350);
        }

        // Re-enable test module logging.
        LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(test), LOG_LEVEL_DEBUG);
    }
```

output: 

```bash
[WRN][main][main:114]: Warning msg iter: 0
[ERR][main][main:115]: Error msg iter: 0
[WRN][test][TestFunc:16]: Warning from test module

[WRN][main][main:114]: Warning msg iter: 1
[ERR][main][main:115]: Error msg iter: 1
[WRN][test][TestFunc:16]: Warning from test module

[WRN][main][main:114]: Warning msg iter: 2
[ERR][main][main:115]: Error msg iter: 2
[WRN][test][TestFunc:16]: Warning from test module

[ERR][main][main:123]: disabling test module on iteration 2... 
[WRN][main][main:114]: Warning msg iter: 3
[ERR][main][main:115]: Error msg iter: 3

[WRN][main][main:114]: Warning msg iter: 4
[ERR][main][main:115]: Error msg iter: 4
```

## License

This project is licensed under the **MIT License**.
See the `LICENSE` file for details.

## Author

* **Ignacio Brittez [(@ibrittez)](https://github.com/ibrittez)**
* Based on the original console logger implementation by **Agustín Catellani [(@holaguz)](https://github.com/holaguz)**
