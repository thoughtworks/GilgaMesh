#include <app_util.h>
#include "cmocka_includes.h"

static __INLINE uint64_t value_rescale(uint32_t value, uint32_t old_unit_reversal, uint16_t new_unit_reversal) {}

static __INLINE uint8_t uint16_encode(uint16_t value, uint8_t * p_encoded_data) {}

static __INLINE uint8_t uint24_encode(uint32_t value, uint8_t * p_encoded_data) {}

static __INLINE uint8_t uint32_encode(uint32_t value, uint8_t * p_encoded_data) {}

static __INLINE uint8_t uint48_encode(uint64_t value, uint8_t * p_encoded_data) {}

static __INLINE uint16_t uint16_decode(const uint8_t * p_encoded_data) {}

static __INLINE uint16_t uint16_big_decode(const uint8_t * p_encoded_data) {}

static __INLINE uint32_t uint24_decode(const uint8_t * p_encoded_data) {}

static __INLINE uint32_t uint32_decode(const uint8_t * p_encoded_data) {}

static __INLINE uint32_t uint32_big_decode(const uint8_t * p_encoded_data) {}

static __INLINE uint8_t uint32_big_encode(uint32_t value, uint8_t * p_encoded_data) {}

static __INLINE uint64_t uint48_decode(const uint8_t * p_encoded_data) {}

static __INLINE uint8_t battery_level_in_percent(const uint16_t mvolts) {}

static __INLINE bool is_word_aligned(void const* p) {}

static __INLINE bool is_address_from_stack(void * ptr) {}