# `stm32-uart-logger`

UART-based logging module for STM32 devices using the STM32 HAL API. Designed and tested
on the STM32F1 Blue Pill board, but compatible with other STM32 MCUs.

## Description

This module provides logging with severity levels (DEBUG, INFO, WARNING, ERROR)
over UART using `HAL_UART_Transmit()` in blocking mode.

It is intended for bare-metal or RTOS projects on STM32 MCUs, and requires the USART1 peripheral
to be configured and enabled externally.

## Features

* Runtime configurable log levels.
* Formatted messages with function name and line number.
* ANSI colors for compatible terminals (only INFO, WARNING, and ERROR).
* Easy to use via macros (`LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR`, `LOG_RAW`).

## Usage

1. Include `logger.h` in your source files.

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

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Author

Ignacio Brittez

Based on the original implementation by Agustín Catellani.
