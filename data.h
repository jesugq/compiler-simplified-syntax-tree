// Imports
#include <stdlib.h>

// Definitions
#define TABLE_SIZE      20
#define TYPE_INTEGER    'I'
#define TYPE_FLOAT      'F'

// Declarations
typedef union data_number {
    int int_value;
    float float_value;
} data_number;
typedef struct data_value {
    char numtype;
    union data_number number;
} data_value;
data_value * data_create_integer(int);
data_value * data_create_float(float);