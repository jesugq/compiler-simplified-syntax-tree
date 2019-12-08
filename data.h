// Imports
#include <stdlib.h>
#include <stdbool.h>

// Definitions
#define DATA_INTEGER    'I'
#define DATA_FLOAT      'F'
#define DATA_ZERO       'z'
#define DATA_SUM        '+'
#define DATA_SUBSTRACT  '-'
#define DATA_MULTIPLY   '*'
#define DATA_DIVIDE     '/'

// Declarations
typedef union data_number {
    int int_value;
    float float_value;
} data_number;
typedef struct data_value {
    char numtype;
    union data_number number;
} data_value;

bool data_numtype_match(data_value * one, data_value * two);
// Missing: bool data_expression.
data_value * data_create_integer(int);
data_value * data_create_float(float);
data_value * data_operation(data_value * one, data_value * two, char operation);
data_value * data_negative(data_value * one);