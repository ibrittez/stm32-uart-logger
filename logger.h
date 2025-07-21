/** @file logger.h
 *
 * @brief UART-based logging module for STM32 HAL.
 *
 * @author Ignacio Brittez
 * @thanks Agustín Catellani – for the original console logger implementation that inspired this
 * work
 *
 * This module provides logging with different severity levels (DEBUG, INFO, WARNING, ERROR)
 * over UART using STM32 HAL.
 *
 * @note Before using this module:
 *  - The USART1 peripheral must be properly configured and initialized.
 *  - A global instance `UART_HandleTypeDef huart1` must exist and be accessible.
 *  - Logging is performed using `HAL_UART_Transmit()` in blocking mode with `HAL_MAX_DELAY`.
 *
 * @details
 * This implementation is heavily inspired by a console logger originally developed by Agustín 
 * Catellani for a POSIX-like environment, and adapted for STM32 bare-metal/RTOS projects using HAL.
 */

#ifndef LOGGER_H
#define LOGGER_H

/* =======================================================================
 * [INCLUDES]
 * =======================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

/* =======================================================================
 * [EXTERNAL DATA DECLARATION]
 * =======================================================================
 */

extern UART_HandleTypeDef huart1;

/* =======================================================================
 * [TYPEDEFS]
 * =======================================================================
 */

/*!
 * @brief Log level enumeration.
 * @note DEBUG is the most verbose level.
 */
typedef enum
{
    LOG_LEVEL_DEBUG = 0, //!< Detailed debugging information (most verbose)
    LOG_LEVEL_INFO,      //!< General informational messages
    LOG_LEVEL_WARNING,   //!< Warnings that do not stop execution
    LOG_LEVEL_ERROR,     //!< Critical errors
    LOG_LEVEL_COUNT,     //!< Number of log levels (used for validation)
    LOG_LEVEL_OFF = 99   //!< Disable all logging output
} log_level_t;

/* =======================================================================
 * [GLOBAL LOG LEVEL]
 * =======================================================================
 */

/*!
 * @brief Current global log level.
 *
 * @note Do NOT modify this directly. Use LOGGER_SET_LOGGING_LEVEL() instead.
 *       This variable is defined as a common symbol so it can be overridden.
 */
__attribute__((common)) log_level_t gLogLevel;

/*!
 * @brief Sets the global logging level.
 *
 * @param level Desired log level (must be between LOG_LEVEL_DEBUG and LOG_LEVEL_ERROR).
 *
 * @note Messages with a severity lower than the selected log level will be ignored.
 */
static inline void LOGGER_SET_LOGGING_LEVEL(log_level_t level)
{
    assert(level >= LOG_LEVEL_DEBUG && level < LOG_LEVEL_COUNT);
    gLogLevel = level;
}

/*!
 * @brief Helper macro, checks if a message with a given severity should be logged.
 *
 * @param severity The log level of the message.
 * @return 1 if the message should be logged, 0 otherwise.
 */
#define CHECK_LOG_LEVEL(severity) ((severity) >= gLogLevel ? 1 : 0)

/* =======================================================================
 * [CONFIGURATION CONSTANTS]
 * =======================================================================
 */

/** @brief Maximum size of the temporary buffer for formatted log messages. */
#define LOG_BUFFER_SIZE 128

/** @brief ANSI escape codes for terminal colors. */
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

/* =======================================================================
 * [LOGGING MACROS]
 * =======================================================================
 */

/*!
 * @brief Logs a raw, unformatted message without severity or color codes.
 */
#define LOG_RAW(fmt, ...)                                                                          \
        do                                                                                         \
        {                                                                                          \
            char msg[LOG_BUFFER_SIZE];                                                             \
            (void) snprintf(msg, LOG_BUFFER_SIZE, fmt, ##__VA_ARGS__);                             \
            HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);               \
        } while (0)

/*!
 * @brief Logs a DEBUG-level message.
 */
#define LOG_DEBUG(fmt, ...)                                                                        \
        do                                                                                         \
        {                                                                                          \
            if (CHECK_LOG_LEVEL(LOG_LEVEL_DEBUG))                                                  \
            {                                                                                      \
                char msg[LOG_BUFFER_SIZE];                                                         \
                (void) snprintf(msg, LOG_BUFFER_SIZE, KWHT "[DBG][%s:%d]: " fmt KNRM, __func__,    \
                                __LINE__, ##__VA_ARGS__);                                          \
                HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);           \
            }                                                                                      \
        } while (0)

/*!
 * @brief Logs an INFO-level message.
 */
#define LOG_INFO(fmt, ...)                                                                         \
        do                                                                                         \
        {                                                                                          \
            if (CHECK_LOG_LEVEL(LOG_LEVEL_INFO))                                                   \
            {                                                                                      \
                char msg[LOG_BUFFER_SIZE];                                                         \
                (void) snprintf(msg, LOG_BUFFER_SIZE, KGRN "[INF][%s:%d]: " KNRM fmt, __func__,    \
                                __LINE__, ##__VA_ARGS__);                                          \
                HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);           \
            }                                                                                      \
        } while (0)

/*!
 * @brief Logs a WARNING-level message.
 */
#define LOG_WARNING(fmt, ...)                                                                      \
        do                                                                                         \
        {                                                                                          \
            if (CHECK_LOG_LEVEL(LOG_LEVEL_WARNING))                                                \
            {                                                                                      \
                char msg[LOG_BUFFER_SIZE];                                                         \
                (void) snprintf(msg, LOG_BUFFER_SIZE, KYEL "[WRN][%s:%d]: " KNRM fmt, __func__,    \
                                __LINE__, ##__VA_ARGS__);                                          \
                HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);           \
            }                                                                                      \
        } while (0)

/*!
 * @brief Logs an ERROR-level message.
 */
#define LOG_ERROR(fmt, ...)                                                                        \
        do                                                                                         \
        {                                                                                          \
            if (CHECK_LOG_LEVEL(LOG_LEVEL_ERROR))                                                  \
            {                                                                                      \
                char msg[LOG_BUFFER_SIZE];                                                         \
                (void) snprintf(msg, LOG_BUFFER_SIZE, KRED "[ERR][%s:%d]: " KNRM fmt, __func__,    \
                                __LINE__, ##__VA_ARGS__);                                          \
                HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);           \
            }                                                                                      \
        } while (0)

#endif /* LOGGER_H */

/*** end of file ***/
