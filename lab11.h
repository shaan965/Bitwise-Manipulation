
/* Purpose(s): floating-pointing point manipulation through bit-wise operators
 * Author:
 * Date:
 * Persons discussed w/:
 * References:
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <math.h>

/*
 In IEEE 754 Standard, a valid 32-bit floating-point number has an internal form:
 s eeeeeeee fffffff ffffffff ffffffff
 where s is the sign  bit with value 0 for non-negative and 1 for negative,
 eeeeeeee is the 8-bit exponent ranging from 00000000 to 11111110,
 fffffff ffffffff ffffffff is the 23-bit fraction.
 When eeeeeeee > 0, the number it represents is (-1)^e * 1.fffffffffffffffffffffff * 2^{eeeeeeee - 127};
 when eeeeeeee == 0, the represented number is (-1)^e * 0.fffffffffffffffffffffff * 2^{-126}.

 In this lab exercise, you use
    - no arithmetic (unary or binary), relational, or equality operators on floating-point numbers
    - any operators on integers, including bit-wise operators
    - casting
 */

// return true if `f` is a valid single precision floating-point number
bool validate(float f);

//requires: `f` is a valid single precision floating-point number
// 1) terminates the program if `f` is invalid;
// 2) terminates as well if `2 * f` is invalid;
// 3) returns `2 * f`;
// 4) all calculations done by bit-wise operations
float doubling(float f);

//requires: `f` and `g` are valid single precision floating-point numbers
// 1) terminates the program if `f` or `g` is invalid, or if their signs are different;
// 2) terminates as well if `f + g` is invalid;
// 3) returns `f + g`;
// 4) all calculations done by bit-wise operations
float adding1(float f, float g);

//requires: `f` and `g` are valid single precision floating-point numbers
// 1) terminates the program if `f` or `g` is invalid, or if their signs are the same;
// 2) returns `f + g`;
// 3) all calculations done by bit-wise operations
float adding2(float f, float g);

//requires: `f` and `g` are valid single precision floating-point numbers
// 1) terminates the program if `f` or `g` is invalid;
// 2) terminates as well if `f - g` is invalid;
// 3) returns `f - g`;
// 4) all calculations done by bit-wise operations
float subtract(float f, float g);
