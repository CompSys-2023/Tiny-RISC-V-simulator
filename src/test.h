#ifndef TEST_H

#include "memory.h"
#include <stdint.h>

#define ASSERT_EQUAL_INT(expected, actual, message)                            \
  (((expected) == (actual))                                                    \
       ? (printf(                                                              \
              "\033[38;5;46m[\033[38;5;82m\033[1mPASSED\033[38;5;46m]\033[0m:" \
              "\033[38;5;82m `%s`: "                                           \
              "Expected: %d, "                                                 \
              "Actual: %d\n\033[0m",                                           \
              message, (expected), (actual)),                                  \
          1)                                                                   \
       : (printf("\033[38;5;196m[\033[91m\033[1m\033[4mFAILED\033[38;5;196m]"  \
                 "\033[0m:"                                                    \
                 "\033["                                                       \
                 "91m `%s`: "                                                  \
                 "Expected: %d, "                                              \
                 "Actual: %d\n\033[0m",                                        \
                 message, (expected), (actual)),                               \
          0))

#define ASSERT_MULTIPLE(test1, test2) ((test1) && (test2))

typedef struct {
  int32_t*       x_before;
  int32_t*       x_after;
  int32_t        pc_before;
  int32_t        pc_after;
  struct memory* mem;
  int32_t        instr_num;
} test_t;

int test(test_t test_data);

#endif