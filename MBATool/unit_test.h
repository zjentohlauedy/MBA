#ifndef __INC_UNIT_TEST_H__
#define __INC_UNIT_TEST_H__


static char __unit_test_msgbuf__[9999+1];
static int  tests_run = 0;

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

#define run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while (0)

#define tests_passed() do { printf( "Tests Passed: %d\n", tests_run ); } while (0)

#endif
