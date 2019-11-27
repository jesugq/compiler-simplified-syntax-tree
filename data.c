#include "data.h"

/**
 * Union Number stores the content of the value in either an integer type or a
 * floating point type. Which type is used rests on its type, present in an 
 * union called data.
 * @param   int_value   Integer depiction of this number.
 * @param   float_value Float depiction of this number.
 */
typedef union data_number {
    int int_value;
    float float_value;
} data_number;

/**
 * Struct Data Content stores the value of the number that the identifier is 
 * currently assigned to, and the type to know how to handle it.
 * @param   numtype     Type of the data.
 * @param   number      Value of the data.
 */
typedef struct data_value {
    char numtype;
    union data_number number;
} data_value;

/**
 * Data Create Integer returns a DATA type using the parameters sent.
 * @param   number      Value of this number.
 * @return  Data instance of an integer.
 */
data_value * data_create_integer(int number){
    data_value * value = (data_value *)calloc(1, sizeof(data_value));
    value->numtype = TYPE_INTEGER;
    value->number.int_value = number;
    return value;
}

/**
 * Data Create Float returns a DATA type using the parameters sent.
 * @param   number      Value of this number.
 * @return  Data instance of a float.
 */
data_value * data_create_float(float number){
    data_value * value = (data_value *)calloc(1, sizeof(data_value));
    value->numtype = TYPE_FLOAT;
    value->number.float_value = number;
    return value;
}