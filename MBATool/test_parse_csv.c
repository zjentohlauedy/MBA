#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_list.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static char *parseCSV_ShouldReturnTrueOnSuccess_GivenACSVLineToParse()
{
     data_list_s list = { 0 };

     assertEquals( bl_True, parseCSV( &list, "" ) );

     return NULL;
}

static char *parseCSV_ShouldParseAnEmptyStringIntoASingleEmptyField_GivenAnEmptyString()
{
     data_list_s list = { 0 };

     assertEquals( bl_True, parseCSV( &list, "" ) );

     assertEquals( 1, list.count );

     data_field_s *actual_fields = list.data;

     assertNotNull( actual_fields );

     assertEqualsStr( "", actual_fields[0].value );

     return NULL;
}

static char *parseCSV_ShouldUpdateDataListWithFields_GivenACSVLineToParse()
{
     char  csv_buffer[100];
     char *expected_field1 = "Field One";
     char *expected_field2 = "Field Two";
     char *expected_field3 = "Field Three";

     sprintf( csv_buffer, "%s,%s,%s", expected_field1, expected_field2, expected_field3 );

     data_list_s list = { 0 };

     assertEquals( bl_True, parseCSV( &list, csv_buffer ) );

     assertEquals( 3, list.count );

     data_field_s *actual_fields = list.data;

     assertNotNull( actual_fields );

     assertEqualsStr( expected_field1, actual_fields[0].value );
     assertEqualsStr( expected_field2, actual_fields[1].value );
     assertEqualsStr( expected_field3, actual_fields[2].value );

     return NULL;
}

static char *parseCSV_ShouldReturnAFieldIfTheLineEndsWithAComma_GivenACSVLineToParse()
{
     data_list_s list = { 0 };

     assertEquals( bl_True, parseCSV( &list, ",,,," ) );

     assertEquals( 5, list.count );

     return NULL;
}

static char *parseCSV_ShouldTreatNewlineAsEndOfCSV_GivenACSVLineWithNewlineToParse()
{
     data_list_s list = { 0 };

     assertEquals( bl_True, parseCSV( &list, ",,,,\n,," ) );

     assertEquals( 5, list.count );

     return NULL;
}

static char *parseCSV_ShouldNotIncludeNewlineAsPartOfAField_GivenACSVLineToParse()
{
     char  csv_buffer[100];
     char *expected_field1 = "Field One";
     char *expected_field2 = "Field Two";
     char *expected_field3 = "Field Three";

     sprintf( csv_buffer, "%s,%s,%s\n", expected_field1, expected_field2, expected_field3 );

     data_list_s list = { 0 };

     assertEquals( bl_True, parseCSV( &list, csv_buffer ) );

     assertEquals( 3, list.count );

     data_field_s *actual_fields = list.data;

     assertNotNull( actual_fields );

     assertEqualsStr( expected_field1, actual_fields[0].value );
     assertEqualsStr( expected_field2, actual_fields[1].value );
     assertEqualsStr( expected_field3, actual_fields[2].value );

     return NULL;
}

static void run_all_tests()
{
     run_test( parseCSV_ShouldReturnTrueOnSuccess_GivenACSVLineToParse,                   null );
     run_test( parseCSV_ShouldParseAnEmptyStringIntoASingleEmptyField_GivenAnEmptyString, null );
     run_test( parseCSV_ShouldUpdateDataListWithFields_GivenACSVLineToParse,              null );
     run_test( parseCSV_ShouldReturnAFieldIfTheLineEndsWithAComma_GivenACSVLineToParse,   null );
     run_test( parseCSV_ShouldTreatNewlineAsEndOfCSV_GivenACSVLineWithNewlineToParse,     null );
     run_test( parseCSV_ShouldNotIncludeNewlineAsPartOfAField_GivenACSVLineToParse,       null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
