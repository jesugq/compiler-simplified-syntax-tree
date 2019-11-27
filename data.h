// Imports
#include <stdlib.h>

// Definitions
#define TABLE_SIZE      20
#define TYPE_INTEGER    'I'
#define TYPE_FLOAT      'F'

// Declarations
typedef union data_number data_number;
typedef struct data_value data_value;
data_value * data_create_integer(int);
data_value * data_create_float(float);