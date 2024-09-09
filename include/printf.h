#ifndef _PRINTF_H_
#define _PRINTF_H_

#include <stdarg.h>
#include <stddef.h>

/* make c++ compiler to c compiler */
#ifdef __cplusplus
extern "C" {
#endif


/**
 * Output a character to a custom device like UART, used by printf() function
 * \param character: to output
 */

void _putchar(char character);

/**
 * Tiny printf implementation
 * To avoid conflicts with the regular printf() API by overriding macro defines
 * \param format: A string that specifies the format of the output
 * \return the number of characters that are written into the array
 */
#define printf printf_
int printf_(const char *format, ...);

/**
 * Tiny sprintf implement
 * \param buffer: A pointer to the buffer where to store the formatted string
 * \param format: A string that specifies the format of the output
 * \return the number of characters that are written into the array
 */
#define sprintf sprintf_
int sprintf(char *buffer, const char *format, ...);

/**
 * Tiny snprintf/vsnprintf implementatio
 * \param buffer: A pointer to the buffer where to store the formatted string
 * \param count: The maximum number of character to store in the buffer
 * \param format: A string that specifies the format of the output
 * \param va: A value identifying a variable arguments list
 * \return The number of characters that COULD have been written into the buffer, 
 *          not inclued terminating character
 */
#define snprintf snprintf_
#define vsnprintf vsnprintf_
int snprintf_(char *buffer, size_t count, const char *format, ...);
int vsnprintf_(char *buffer, size_t count, const char *format, va_list va);

/**
 * Tiny vprintf implementation
 * \param format: A string that specifies the format of the output
 * \param va: A value identifying a variable arguments list
 * \return The number of characters that COULD have been written into the buffer, 
 *          not inclued terminating character
 */
#define vprintf vprintf_
int vprintf_(const char *format, va_list va);

/**
 * printf with output function
 * \param out: An output function thtat takes one character and an argument pointer
 * \param arg: An argument pointer for USER data passed to output function
 * \param format: A string that specifies the format of the output
 * \return The number of characters that COULD have been written into the buffer, 
 *          not inclued terminating character
 */
int fctprintf(void (*out) (char character, void *arg), void *arg, const char *format, ...);

#ifdef __cplusplus
}
#endif


#endif  // _PRINTF_H_