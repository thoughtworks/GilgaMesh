#include <stdint.h>

#define HEX_DEVICE_ID_TEMPLATE    "Mesh-0000"
#define HEX_DEVICE_ID_LENGTH      10    //refer to above id template length

inline uint32_t get_raw_device_id(void);
char* get_short_hex_device_id(uint32_t deviceId, char* result);