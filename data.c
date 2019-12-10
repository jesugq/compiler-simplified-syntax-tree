#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

/**
 * Union Number stores the content of the value in either an integer type or a
 * floating point type. Which type is used rests on its type, present in an 
 * union called data.
 * @param   int_value   Integer depiction of this number.
 * @param   float_value Float depiction of this number.
 */
// typedef union data_number {
//     int int_value;
//     float float_value;
// } data_number;

/**
 * Struct Data Content stores the value of the number that the identifier is 
 * currently assigned to, and the type to know how to handle it.
 * @param   numtype     Type of the data.
 * @param   number      Value of the data.
 */
// typedef struct data_value {
//     char numtype;
//     union data_number number;
// } data_value;

/**
 * Data Numtype Match checks the types of two data unions.
 * @param   one     Data of the first element.
 * @param   two     Data of the second element.
 * @return  If both match the same numtype. 
 */
bool data_numtype_match(data_value * one, data_value * two) {
    return one->numtype == two->numtype;
}

/**
 * Data Evaluation evaluates two data unions.
 * @param   one     Data of the first element.
 * @param   two     Data of the second element.
 * @return  If the evaluation was true or false.
 */
bool data_evaluation(data_value * one, data_value * two, char operation) {
    if (!data_numtype_match(one, two))
        return false;

    if (one->numtype == DATA_INTEGER) {
        int input_one = one->number.int_value;
        int input_two = two->number.int_value;

        switch (operation) {
            case DATA_LESS:     return input_one < input_two;
            case DATA_GREATER:  return input_one > input_two;
            case DATA_EQUALS:   return input_one == input_two;
            case DATA_LTE:      return input_one <= input_two;
            case DATA_GTE:      return input_one >= input_two;
            default:            return false;
        }
    } else if (one->numtype == DATA_FLOAT) {
        float input_one = one->number.float_value;
        float input_two = two->number.float_value;

        switch (operation) {
            case DATA_LESS:     return input_one < input_two;
            case DATA_GREATER:  return input_one > input_two;
            case DATA_EQUALS:   return input_one == input_two;
            case DATA_LTE:      return input_one <= input_two;
            case DATA_GTE:      return input_one >= input_two;
            default:            return false;
        }
    } else return false;
}

/**
 * Data Is Zero evaluates a single value to zero.
 * @param   one     Data of the first element.
 * @return  If the evaluaton is zero.
 */
bool data_zero(data_value * one) {
    if (one->numtype == DATA_INTEGER)
        return one->number.int_value == 0;
    else if (one->numtype == DATA_FLOAT)
        return one->number.float_value == 0;
    else return false;
}

/**
 * Data Create Integer returns a DATA type using the parameters sent.
 * @param   number      Value of this number.
 * @return  Data instance of an integer.
 */
data_value * data_create_integer(int number) {
    data_value * value = (data_value *)calloc(1, sizeof(data_value));
    value->numtype = DATA_INTEGER;
    value->number.int_value = number;
    return value;
}

/**
 * Data Create Float returns a DATA type using the parameters sent.
 * @param   number      Value of this number.
 * @return  Data instance of a float.
 */
data_value * data_create_float(float number) {
    data_value * value = (data_value *)calloc(1, sizeof(data_value));
    value->numtype = DATA_FLOAT;
    value->number.float_value = number;
    return value;
}

/**
 * Data Operation checks which type of operation to do, an integer or a
 * floating point operation depending on the numtype of them. It is assumed
 * that both are the same, but error handling is present.
 * @param   one         Data of the first element.
 * @param   two         Data of the second element.
 * @param   operation   Operation to do with the two datas.
 * @return  Arithmetic expression between the two.
 */
data_value * data_operation(
    data_value * one, data_value * two, char operation
){
    if (!data_numtype_match(one, two)) return data_create_integer(0);
    if (one->numtype == DATA_INTEGER) {
        int input_one = one->number.int_value;
        int input_two = two->number.int_value;
        int output = 0;

        switch (operation) {
            case DATA_SUM:          output = input_one + input_two; break;
            case DATA_SUBSTRACT:    output = input_one - input_two; break;
            case DATA_MULTIPLY:     output = input_one * input_two; break;
            case DATA_DIVIDE:       output = input_one / input_two; break;
            default:                output = 0; break;
        }
        return data_create_integer(output);
    } else if (one->numtype == DATA_FLOAT) {
        float input_one = one->number.float_value;
        float input_two = two->number.float_value;
        float output = 0;

        switch (operation) {
            case DATA_SUM:          output = input_one + input_two; break;
            case DATA_SUBSTRACT:    output = input_one - input_two; break;
            case DATA_MULTIPLY:     output = input_one * input_two; break;
            case DATA_DIVIDE:       output = input_one / input_two; break;
            default:                output = 0; break;
        }
        return data_create_float(output);
    } else return data_create_integer(0);
}

/**
 * Data Negative simply returns a new data type with a negative
 * version of the one included as a paremter.
 * @param   one     Data of the only element.
 * @return  Negative data equivalent of the parameter.
 */
data_value * data_negative(data_value * one) {
    if (one->numtype == DATA_INTEGER)
        return data_create_integer(one->number.int_value * (-1));
    else if (one->numtype == DATA_FLOAT)
        return data_create_float(one->number.float_value * (-1));
    else return data_create_integer(0);
}