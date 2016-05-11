#pragma once

#ifdef TESTING
   #define SKIP_IF_TEST(SKIPPED_CODE)
#else
   #define SKIP_IF_TEST(SKIPPED_CODE) SKIPPED_CODE
#endif

