/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup app_util Utility Functions and Definitions
 * @{
 * @ingroup app_common
 *
 * @brief Various types and definitions available to all applications.
 */

#ifndef APP_UTIL_H__
#define APP_UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include "compiler_abstraction.h"
#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

//lint -save -e27 -e10 -e19
#if defined ( __CC_ARM )
extern char STACK$$Base;
extern char STACK$$Length;
#define STACK_BASE    &STACK$$Base
#define STACK_TOP    ((void*)((uint32_t)STACK_BASE + (uint32_t)&STACK$$Length))
#elif defined ( __ICCARM__ )
extern char CSTACK$$Base;
extern char CSTACK$$Length;
#define STACK_BASE    &CSTACK$$Base
#define STACK_TOP    ((void*)((uint32_t)STACK_BASE + (uint32_t)&CSTACK$$Length))
#elif defined   ( __GNUC__ )
extern uint32_t __StackTop;
extern uint32_t __StackLimit;
#define STACK_BASE    &__StackLimit
#define STACK_TOP     &__StackTop
#endif
//lint -restore

enum
{
    UNIT_0_625_MS = 625,                                /**< Number of microseconds in 0.625 milliseconds. */
    UNIT_1_25_MS  = 1250,                               /**< Number of microseconds in 1.25 milliseconds. */
    UNIT_10_MS    = 10000                               /**< Number of microseconds in 10 milliseconds. */
};


/**@brief Implementation specific macro for delayed macro expansion used in string concatenation
*
* @param[in]   lhs   Left hand side in concatenation
* @param[in]   rhs   Right hand side in concatenation
*/
#define STRING_CONCATENATE_IMPL(lhs, rhs) lhs ## rhs


/**@brief Macro used to concatenate string using delayed macro expansion
*
* @note This macro will delay concatenation until the expressions have been resolved
*
* @param[in]   lhs   Left hand side in concatenation
* @param[in]   rhs   Right hand side in concatenation
*/
#define STRING_CONCATENATE(lhs, rhs) STRING_CONCATENATE_IMPL(lhs, rhs)


// Disable lint-warnings/errors for STATIC_ASSERT
//lint --emacro(10,STATIC_ASSERT)
//lint --emacro(18,STATIC_ASSERT)
//lint --emacro(19,STATIC_ASSERT)
//lint --emacro(30,STATIC_ASSERT)
//lint --emacro(37,STATIC_ASSERT)
//lint --emacro(42,STATIC_ASSERT)
//lint --emacro(26,STATIC_ASSERT)
//lint --emacro(102,STATIC_ASSERT)
//lint --emacro(533,STATIC_ASSERT)
//lint --emacro(534,STATIC_ASSERT)
//lint --emacro(132,STATIC_ASSERT)
//lint --emacro(414,STATIC_ASSERT)
//lint --emacro(578,STATIC_ASSERT)
//lint --emacro(628,STATIC_ASSERT)
//lint --emacro(648,STATIC_ASSERT)
//lint --emacro(830,STATIC_ASSERT)


/**@brief Macro for doing static (i.e. compile time) assertion.
*
* @note If the EXPR isn't resolvable, then the error message won't be shown.
*
* @note The output of STATIC_ASSERT will be different across different compilers.
*
* @param[in] EXPR Constant expression to be verified.
*/
#if defined ( __COUNTER__ )

#define STATIC_ASSERT(EXPR) \
    ;enum { STRING_CONCATENATE(static_assert_, __COUNTER__) = 1 / (!!(EXPR)) }

#else

#define STATIC_ASSERT(EXPR) \
    ;enum { STRING_CONCATENATE(assert_line_, __LINE__) = 1 / (!!(EXPR)) }

#endif

/**@brief Implementation details for NUM_VAR_ARGS */
#define NUM_VA_ARGS_IMPL(                              \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,       \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
    _61, _62, N, ...) N


/**@brief Macro to get the number of arguments in a call variadic macro call
 *
 * param[in]    ...     List of arguments
 *
 * @retval  Number of variadic arguments in the argument list
 */
#define NUM_VA_ARGS(...) NUM_VA_ARGS_IMPL(__VA_ARGS__, 63, 62, 61,  \
    60, 59, 58, 57, 56, 55, 54, 53, 52, 51,                         \
    50, 49, 48, 47, 46, 45, 44, 43, 42, 41,                         \
    40, 39, 38, 37, 36, 35, 34, 33, 32, 31,                         \
    30, 29, 28, 27, 26, 25, 24, 23, 22, 21,                         \
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11,                         \
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/**@brief Implementation details for NUM_VAR_ARGS */
#define NUM_VA_ARGS_LESS_1_IMPL(                       \
    _ignored,                                          \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,       \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
    _61, _62, N, ...) N

/**@brief Macro to get the number of arguments in a call variadic macro call.
 * First argument is not counted.
 *
 * param[in]    ...     List of arguments
 *
 * @retval  Number of variadic arguments in the argument list
 */
#define NUM_VA_ARGS_LESS_1(...) NUM_VA_ARGS_LESS_1_IMPL(__VA_ARGS__, 63, 62, 61,  \
    60, 59, 58, 57, 56, 55, 54, 53, 52, 51,                         \
    50, 49, 48, 47, 46, 45, 44, 43, 42, 41,                         \
    40, 39, 38, 37, 36, 35, 34, 33, 32, 31,                         \
    30, 29, 28, 27, 26, 25, 24, 23, 22, 21,                         \
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11,                         \
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, ~)


/**@brief type for holding an encoded (i.e. little endian) 16 bit unsigned integer. */
typedef uint8_t uint16_le_t[2];

/**@brief Type for holding an encoded (i.e. little endian) 32 bit unsigned integer. */
typedef uint8_t uint32_le_t[4];

/**@brief Byte array type. */
typedef struct
{
    uint16_t  size;                 /**< Number of array entries. */
    uint8_t * p_data;               /**< Pointer to array entries. */
} uint8_array_t;


/**@brief Macro for performing rounded integer division (as opposed to truncating the result).
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Rounded (integer) result of dividing A by B.
 */
#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))


/**@brief Macro for checking if an integer is a power of two.
 *
 * @param[in]   A   Number to be tested.
 *
 * @return      true if value is power of two.
 * @return      false if value not power of two.
 */
#define IS_POWER_OF_TWO(A) ( ((A) != 0) && ((((A) - 1) & (A)) == 0) )


/**@brief Macro for converting milliseconds to ticks.
 *
 * @param[in] TIME          Number of milliseconds to convert.
 * @param[in] RESOLUTION    Unit to be converted to in [us/ticks].
 */
#define MSEC_TO_UNITS(TIME, RESOLUTION) (((TIME) * 1000) / (RESOLUTION))


/**@brief Macro for performing integer division, making sure the result is rounded up.
 *
 * @details One typical use for this is to compute the number of objects with size B is needed to
 *          hold A number of bytes.
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Integer result of dividing A by B, rounded up.
 */
#define CEIL_DIV(A, B)      \
    (((A) + (B) - 1) / (B))


/**@brief Macro for creating a buffer aligned to 4 bytes.
 *
 * @param[in]   NAME        Name of the buffor.
 * @param[in]   MIN_SIZE    Size of this buffor (it will be rounded up to multiples of 4 bytes).
 */
#define WORD_ALIGNED_MEM_BUFF(NAME, MIN_SIZE) static uint32_t NAME[CEIL_DIV(MIN_SIZE, sizeof(uint32_t))]


/**@brief Macro for calculating the number of words that are needed to hold a number of bytes.
 *
 * @details Adds 3 and divides by 4.
 *
 * @param[in]  n_bytes  The number of bytes.
 *
 * @return The number of words that @p n_bytes take up (rounded up).
 */
#define BYTES_TO_WORDS(n_bytes) (((n_bytes) + 3) >> 2)


/**@brief The number of bytes in a word.
 */
#define BYTES_PER_WORD (4)


/**@brief Macro for increasing a number to the nearest (larger) multiple of another number.
 *
 * @param[in]  alignment  The number to align to.
 * @param[in]  number     The number to align (increase).
 *
 * @return The aligned (increased) @p number.
 */
#define ALIGN_NUM(alignment, number) ((number - 1) + alignment - ((number - 1) % alignment))

/**@brief Macro for getting first of 2 parameters.
 *
 * @param[in] a1    First parameter.
 * @param[in] a2    Second parameter.
 */
#define GET_ARG_1(a1, a2) a1

/**@brief Macro for getting second of 2 parameters.
 *
 * @param[in] a1    First parameter.
 * @param[in] a2    Second parameter.
 */
#define GET_ARG_2(a1, a2) a2

static __INLINE uint64_t value_rescale(uint32_t value, uint32_t old_unit_reversal, uint16_t new_unit_reversal);

static __INLINE uint8_t uint16_encode(uint16_t value, uint8_t * p_encoded_data);

static __INLINE uint8_t uint24_encode(uint32_t value, uint8_t * p_encoded_data);

static __INLINE uint8_t uint32_encode(uint32_t value, uint8_t * p_encoded_data);

static __INLINE uint8_t uint48_encode(uint64_t value, uint8_t * p_encoded_data);

static __INLINE uint16_t uint16_decode(const uint8_t * p_encoded_data);

static __INLINE uint16_t uint16_big_decode(const uint8_t * p_encoded_data);

static __INLINE uint32_t uint24_decode(const uint8_t * p_encoded_data);

static __INLINE uint32_t uint32_decode(const uint8_t * p_encoded_data);

static __INLINE uint32_t uint32_big_decode(const uint8_t * p_encoded_data);

static __INLINE uint8_t uint32_big_encode(uint32_t value, uint8_t * p_encoded_data);

static __INLINE uint64_t uint48_decode(const uint8_t * p_encoded_data);

static __INLINE uint8_t battery_level_in_percent(const uint16_t mvolts);

static __INLINE bool is_word_aligned(void const* p);

static __INLINE bool is_address_from_stack(void * ptr);



#ifdef __cplusplus
}
#endif

#endif // APP_UTIL_H__

/** @} */
