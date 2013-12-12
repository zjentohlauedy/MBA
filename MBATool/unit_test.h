#ifndef __INC_UNIT_TEST_H__
#define __INC_UNIT_TEST_H__

#include <stdio.h>
#include <string.h>

static char __unit_test_msgbuf__[9999+1];
static int  __unit_tests_run__    = 0;
static int  __unit_tests_passed__ = 0;
static int  __unit_tests_failed__ = 0;

/*
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
*/

#define assert(message, test) do { if (!(test)) return message; } while (0)

#define assertEqualsDbl( E, A )                                         \
     do                                                                 \
     {                                                                  \
          float __expected__ = (E);                                     \
          float __actual__   = (A);                                     \
          if ( (__expected__ > __actual__  && __expected__ - __actual__ > 0.0001) ||   \
               (__expected__ < __actual__  && __actual__ - __expected__ > 0.0001)    ) \
          {                                                             \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected: %f, actual: %f\033[0m", #A, __expected__, __actual__ ); \
               return __unit_test_msgbuf__;                             \
          }                                                             \
     } while ( 0 )

#define assertEqualsInt( E, A )                                         \
     do                                                                 \
     {                                                                  \
          int __expected__ = (E);                                       \
          int __actual__   = (A);                                       \
          if ( __expected__ != __actual__ )                             \
          {                                                             \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected: %d, actual: %d\033[0m", #A, __expected__, __actual__ ); \
               return __unit_test_msgbuf__;                             \
          }                                                             \
     } while ( 0 )

#define assertEqualsStr( E, A )                                         \
     do                                                                 \
     {                                                                  \
          char *__expected__ = (E);                                     \
          char *__actual__   = (A);                                     \
          if ( strcmp( __expected__, __actual__ ) != 0 )                \
          {                                                             \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected: %s, actual: %s\033[0m", #A, __expected__, __actual__ ); \
               return __unit_test_msgbuf__;                             \
          }                                                             \
     } while ( 0 )

#define assertEqualsBfr( E, A, L )                                      \
     do                                                                 \
     {                                                                  \
          char *__expected__ = (E);                                     \
          char *__actual__   = (A);                                     \
          int   __length__   = (L);                                     \
          if ( memcmp( __expected__, __actual__, __length__ ) != 0 )    \
          {                                                             \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected: %.*s, actual: %.*s\033[0m", #A, \
                        __length__, __expected__, __length__, __actual__ ); \
               return __unit_test_msgbuf__;                             \
          }                                                             \
     } while ( 0 )

#define assertNotNull( actual )                                    \
     do                                                            \
     {                                                             \
          if ( (actual) == NULL )                                  \
          {                                                        \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected to not be NULL but was NULL\033[0m", #actual ); \
               return __unit_test_msgbuf__;                        \
          }                                                        \
     } while ( 0 )

#define assertNull( actual )                                       \
     do                                                            \
     {                                                             \
          if ( (actual) != NULL )                                  \
          {                                                        \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected to be NULL but was not\033[0m", #actual ); \
               return __unit_test_msgbuf__;                        \
          }                                                        \
     } while ( 0 )

#define assertNonZero( actual )                                    \
     do                                                            \
     {                                                             \
          if ( (actual) == 0 )                                     \
          {                                                        \
               sprintf( __unit_test_msgbuf__, "\033[0;31mFailed: %s; expected to not be zero but was zero\033[0m", #actual ); \
               return __unit_test_msgbuf__;                        \
          }                                                        \
     } while ( 0 )

#define assertEquals assertEqualsInt

#define run_test(test, callback)                           \
     do                                                    \
     {                                                     \
          printf( "Running %s ", #test );                  \
          char *message = test();                          \
          __unit_tests_run__++;                            \
          if (message)                                     \
          {                                                \
               __unit_tests_failed__++;                    \
               printf( "\033[0;31mFAILED\033[0m\n" );      \
               printf( "%s\n", message );                  \
               if ( callback != NULL ) callback();         \
               printf( "\n" );                             \
          }                                                \
          else                                             \
          {                                                \
               __unit_tests_passed__++;                    \
               printf( "\033[0;32mPASSED\033[0m\n" );      \
          }                                                \
     } while ( 0 )


#define show_test_results()                                        \
     do                                                            \
     {                                                             \
          if ( __unit_tests_failed__ > 0 ) printf( "\033[0;31m" ); else printf( "\033[0;32m" ); \
          printf( "Tests Run: %d, Passed: %d, Failed: %d\n", __unit_tests_run__, __unit_tests_passed__, __unit_tests_failed__ ); \
          printf( "\033[0m" );                                     \
     } while ( 0 )

#define tests_pass() (__unit_tests_failed__ == 0)

static void null() {}

#endif
