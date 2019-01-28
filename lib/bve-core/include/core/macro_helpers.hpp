#pragma once

/**
 * \brief Pre-expands the macro provided. Helpful due to MSVC's non-C99 compliant preprocessor
 * \param x Macro to expand.
 */
#define EXPAND(x) x
/**
 * \brief Turns C++ code in argument into C string with the C++ code in it.
 * \param x Code to stringify.
 */
#define STR(x) #x
/**
 * \brief Symbol concatenation helper due to MSVC. INTERNAL
 * \param a Left symbols already expanded.
 * \param b Right symbols already expanded.
 */
#define INTERNAL_CONCAT_HELPER(a, b) a##b
/**
 * \brief Does symbol concatenation on two arguments, turning them into a single symbol with both things in them.
 * \param a Left symbols.
 * \param b Right symbols.
 */
#define CONCAT(a, b) EXPAND(INTERNAL_CONCAT_HELPER(a, b))
