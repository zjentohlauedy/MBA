#ifndef __INC_UNIT_TEST_H__
#define __INC_UNIT_TEST_H__

#include <string.h>

static char __unit_test_msgbuf__[9999+1];
static int  __unit_tests_run__    = 0;
static int  __unit_tests_passed__ = 0;
static int  __unit_tests_failed__ = 0;

#define assert(message, test) do { if (!(test)) return message; } while (0)

#define assertEquals( desc, expected, actual ) \
do \
{ \
     if ( (expected) != (actual) ) \
     { \
          sprintf( __unit_test_msgbuf__, "Failed: %s; expected: %d, actual: %d", (desc), (expected), (actual) ); \
          return __unit_test_msgbuf__; \
     } \
} while ( 0 )


#define assertEqualsStr( desc, expected, actual ) \
do \
{ \
     if ( strcmp( (expected), (actual) ) != 0 ) \
     { \
          sprintf( __unit_test_msgbuf__, "Failed: %s; expected: %s, actual: %s", (desc), (expected), (actual) ); \
          return __unit_test_msgbuf__; \
     } \
} while ( 0 )

#define run_test(test, callback) \
do \
{ \
     printf( "Running %s ", #test ); \
     char *message = test(); \
     __unit_tests_run__++; \
     if (message) \
     { \
          __unit_tests_failed__++; \
          printf( "FAILED\n" ); \
          printf( "%s\n", message ); \
          if ( callback != NULL ) callback(); \
          printf( "\n" ); \
     } \
     else \
     { \
          __unit_tests_passed__++; \
          printf( "PASSED\n" ); \
     } \
} while (0)


#define show_test_results() do { printf( "Tests Run: %d, Passed: %d, Failed: %d\n", __unit_tests_run__, __unit_tests_passed__, __unit_tests_failed__ ); } while (0)

#endif