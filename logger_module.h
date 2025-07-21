/** @file logger_module.h
 *
 * @brief Per-module logging support.
 *
 * It provides:
 *  - `LOG_MODULE_REGISTER()` to create a module-specific logger.
 *  - `LOG_MODULE_DECLARE()` to reference and use an already registered module.
 *  - `LOG_MODULE_EXTERN()` to reference other modules without altering the current one.
 *  - `LOG_MODULE_SET_LEVEL()` to dynamically change a module’s log level at runtime.
 *
 * @note
 *  - Always include this header instead of `logger.h` if you plan to use module-based logging.
 *  - Including `logger.h` directly before `logger_module.h` will cause a compile-time error.
 */

#ifdef LOGGER_H
#error "logger.h was included before logger_module.h. Please include only logger_module.h first."
#endif

#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#define MODULE_REGISTRED

#include "logger.h"

/* =======================================================================
 * [TYPEDEF]
 * ======================================================================= */

/**
 * @brief Logging instance for a specific module.
 */
typedef struct
{
    const char *name;  /**< Module name (used as log prefix). */
    log_level_t level; /**< Minimum severity level to log for this module. */
} log_instance_t;

/* =======================================================================
 * [MACROS]
 * ======================================================================= */

/**
 * @brief Registers a log instance for the current module.
 *
 * Defines a static `log_instance_t` named `log_inst_<name>` and sets
 * `CURRENT_LOG_MODULE` to point to it.
 *
 * @param name  Identifier name of the module (used as log prefix).
 * @param level Initial minimum severity level for this module.
 *
 * @warning
 *  - A module must only be registered once.
 *  - In other files that reference the same module, use `LOG_MODULE_DECLARE()` instead.
 *
 * @example
 * // Register the device01 logging module.
 * LOG_MODULE_REGISTER(device01, LOG_LEVEL_DEBUG);
 */
#define LOG_MODULE_REGISTER(name, level)                                                           \
    log_instance_t log_inst_##name = {#name, level};                                               \
    static log_instance_t *const CURRENT_LOG_MODULE = &log_inst_##name

/**
 * @brief Declares an existing log instance defined elsewhere.
 *
 * Used in source files that want to log using a module instance defined
 * in another translation unit.
 *
 * Also sets `CURRENT_LOG_MODULE` so all `LOG_*` macros use the declared instance.
 *
 * @param name Name of the previously defined module log instance.
 *
 * @example
 * // Set the current source file's logging module to the already defined device01 module.
 * LOG_MODULE_DECLARE(device01);
 */
#define LOG_MODULE_DECLARE(name)                                                                   \
    extern log_instance_t log_inst_##name;                                                         \
    static log_instance_t *const CURRENT_LOG_MODULE = &log_inst_##name

/**
 * @brief Returns a pointer to a named log instance.
 *
 * Use this when you want to manipulate the log level of another module
 * without switching the current module.
 *
 * @param name Name of the module log instance.
 * @return Pointer to `log_instance_t`.
 *
 * @example
 * // Get a pointer to the device01 module instance:
 * log_instance_t *inst = LOG_MODULE_NAME(device01);
 */
#define LOG_MODULE_NAME(name) (&log_inst_##name)

/**
 * @brief Sets the minimum severity level for a log instance.
 *
 * @param[in,out] inst  Pointer to the log instance.
 * @param[in]     level New severity level to assign.
 *
 * @example
 * // Disable logging on the module registered as "device01".
 * LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(device01), LOG_LEVEL_OFF);
 *
 * // Re-enable logging on the same module.
 * LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(device01), LOG_LEVEL_DEBUG);
 */
static inline void LOG_MODULE_SET_LEVEL(log_instance_t *inst, log_level_t level)
{
    if (inst)
    {
        inst->level = level;
    }
}

/**
 * @brief Declares a log instance defined elsewhere, without modifying `CURRENT_LOG_MODULE`.
 *
 * Use this when you want to reference a module from another source file
 * without changing the active module used by the logging macros.
 *
 * @param name Name of the log module instance to reference.
 *
 * @example
 * // In device01.c you have:
 * LOG_MODULE_REGISTER(device01, LOG_LEVEL_DEBUG);
 *
 * // In device02.c you want to disable device01 logs:
 * LOG_MODULE_EXTERN(device01);
 * LOG_MODULE_SET_LEVEL(LOG_MODULE_NAME(device01), LOG_LEVEL_OFF);
 */
#define LOG_MODULE_EXTERN(name) extern log_instance_t log_inst_##name

#endif /* LOGGER_MODULE_H */

/*** end of file ***/
