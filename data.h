// Imports
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definitions
#define DATA_INTEGER    'I'
#define DATA_FLOAT      'F'

#define DATA_SUM        '+'
#define DATA_SUBSTRACT  '-'
#define DATA_MULTIPLY   '*'
#define DATA_DIVIDE     '/'
#define DATA_NEGATIVE   '~'

#define DATA_LESS         '<'
#define DATA_GREATER      '>'
#define DATA_EQUALS       '='
#define DATA_LTE          'l'
#define DATA_GTE          'g'
#define DATA_ZERO         'z'

// Declarations
typedef union data_number {
    int int_value;
    float float_value;
} data_number;
typedef struct data_value {
    char numtype;
    union data_number number;
} data_value;

bool data_numtype_match(data_value*, data_value*);
bool data_evaluation(data_value*, data_value*, char);
bool data_is_zero(data_value*);
data_value * data_create_integer(int);
data_value * data_create_float(float);
data_value * data_operation(data_value*, data_value*, char);
data_value * data_negative(data_value*);