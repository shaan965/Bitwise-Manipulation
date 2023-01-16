
/* Purpose(s): floating-pointing point manipulation through bit-wise operators
 * Author: Safwan
 * Date: 11/19/2021
 * Persons discussed w/: Habib
 * References: Rebecca's solution
 */

#include "lab11.h"

// return true if `f` is a valid single precision floating-point number
bool validate(float f) {
    
    unsigned int new = *(unsigned int *)&f;
    //get the exponent bits e
    int e = new & 0x7f800000;
    e >>= 23;
 
    if (e == 255)
    {
        return false;
    }
    else
    {
        return true;
    }


}

//requires: `f` is a valid single precision floating-point number
// 1) terminates the program if `f` is invalid;
// 2) terminates as well if `2 * f` is invalid;
// 3) returns `2 * f`;
// 4) all calculations done by bit-wise operations
float doubling(float f) {
    if (!validate(f))
    {
        exit(0);
    }
    unsigned int new = *(unsigned int *)&f;
    int s, e, m;
    s = new >> 31;
    e = new & 0x7f800000;
    e >>= 23;
    m = new & 0x007fffff;
    //printf("%i", e);
    if (e==254)
    {
        exit(0);
    }
    else if (e==0)
    {
        m <<= 1;
        unsigned int result = m | (e << 23) | (s << 31);
        return *((float *)&result);
    }
    else
    {
        e = e + 1;
        unsigned int result = m | (e << 23) | (s << 31);
        return *((float *)&result);
    }
    


}

//requires: `f` and `g` are valid single precision floating-point numbers
// 1) terminates the program if `f` or `g` is invalid, or if their signs are different;
// 2) terminates as well if `f + g` is invalid;
// 3) returns `f + g`;
// 4) all calculations done by bit-wise operations
float adding1(float f, float g) {
    if (!validate(f) || !validate(g)) {
        exit(0);
    }

    // convert to unsigned ints and get their signs
    unsigned int fI = *(unsigned int *) &f;
    unsigned int gI = *(unsigned int *) &g;
    unsigned int signF = fI >> 31;
    unsigned int signG = gI >> 31;

    // signs must be the same
    if (signG != signF) {
        exit(0);
    }

    // make fI the bigger number
    if (gI > fI) {
        unsigned int temp = fI;
        fI = gI;
        gI = temp;
    }
    
    // get the exponents and mantissas
    unsigned int expoF = (fI >> 23) & 0xff;
    unsigned int expoG = (gI >> 23) & 0xff;
    unsigned int mantissaF = fI & 0x7fffff;
    unsigned int mantissaG = gI & 0x7fffff;

    if (expoF == 0) {
        // special case, denormalized number
        expoF++;
    } else {
        // add the missing 1 on
        mantissaF = mantissaF | 0x800000;
    }

    if (expoG == 0) {
        // special case, denormalized number
        expoG++;
    } else {
        // add the missing 1 on
        mantissaG = mantissaG | 0x800000;
    }

    mantissaG = mantissaG >> (expoF - expoG);
    unsigned int sum = mantissaF + mantissaG;

    // get the number of leading 0s (subtract the 8 from the exponent taking up space)
    int i = 0;
    unsigned int temp = sum;
    while ((temp & 0x80000000) == 0 && i < expoF + 8) {
	    i += 1;
	    temp = temp << 1;
    }
    expoF = expoF + 8 - i;

    // remove leading 1 
    if (expoF != 0) {
	    sum = sum << 1;
    }

    // put everything back together and validate
    unsigned int result = (signF << 31) | (expoF << 23) | ((sum << i) >> 9);
    // check and return the result
    if (!validate(*((float *)&result)))
    {
        exit(0);
    }
    else
    {
        return *((float *)&result);
    }
}

//requires: `f` and `g` are valid single precision floating-point numbers
// 1) terminates the program if `f` or `g` is invalid, or if their signs are the same;
// 2) returns `f + g`;
// 3) all calculations done by bit-wise operations
float adding2(float f, float g) {
    if (!validate(f) || !validate(g))
    {
        exit(0);
    }

    unsigned int fI = *(unsigned int *)&f;
    int s1 = fI >> 31;
    int e1 = (0x7f800000 & fI) >> 23;
    int m1 = 0x007fffff & fI;

    unsigned int gI = *(unsigned int *)&g;
    int s2 = gI >> 31;
    int e2 = (0x7f800000 & gI) >> 23;
    int m2 = 0x007fffff & gI;

    unsigned int result, sign;

    if (s1 == s2)
    {
        exit(0);
    }
    
    // Set the first floating number to be bigger one
    unsigned int new1;
    unsigned int new2;
    // get the largest num as the first (absolute)
    if (e1 > e2) {
        new1 = *(unsigned int *)&f;
        new2 = *(unsigned int *)&g;
    }
    else if (e1 < e2) {
        new1 = *(unsigned int *)&g;
        new2 = *(unsigned int *)&f;
    }
    else if (e1 == e2) {
        if (m1 > m2) {
            new1 = *(unsigned int *)&f;
            new2 = *(unsigned int *)&g;
        }
        else if (m1 < m2) {
            new1 = *(unsigned int *)&g;
            new2 = *(unsigned int *)&f;
        }
        else {
            return 0.0;
        }
    }

    unsigned int sign1 = (0x80000000 & new1) >> 31;
    unsigned int exponent1 = (0x7f800000 & new1) >> 23;
    unsigned int mantissa1 = 0x007fffff & new1;

    unsigned int sign2 = (0x80000000 & new2) >> 31;
    unsigned int exponent2 = (0x7f800000 & new2) >> 23;
    unsigned int mantissa2 = (0x007fffff & new2);
    //special case, denormalized number
    if (exponent1 == 0) {
        exponent1 = exponent1 + 1;
    }
    else {
        mantissa1 |= 0x00800000;
    }
    
    if (exponent2 == 0) {
        exponent2 = exponent2 + 1;
    }
    else {
        mantissa2 |= 0x00800000;
    }
    // get better precision for the subtraction, it fails otherwise
    mantissa1 <<= 8;
    mantissa2 <<= 8;
    
    unsigned int difference = exponent1 - exponent2;
    unsigned int exponent;
    // aligning the mantissa part, assigning the larger part of exponent
    if (difference > 0 && difference < 32) {
        mantissa2 >>= difference;
        exponent = exponent1;
    }
    else if (difference < 0 && difference < 32) {
        mantissa1 >>= (difference * -1);
        exponent = exponent2;
    }
    else if (difference == 0 && difference < 32) {
        exponent = exponent1;
    }

    unsigned int diff_mantissa = mantissa1 - mantissa2;
    // get the number of leading 0s 
    int i = 0;
    unsigned int temp = diff_mantissa;
    while ((temp & 0x80000000) == 0 && i < exponent) {
	    i += 1;
	    temp = temp << 1;
    }
    exponent = exponent - i;

    // eliminating the adding mantissa part
    if (exponent != 0) {
        diff_mantissa <<= 1;
    }
    // determine the sign bit of result
    if (sign1 == 0 && sign2 == 1) {
        sign = 0;
    }
    else if (sign1 == 1 && sign2 == 0) {
        sign = 1;
    }
    result = (sign << 31) | (exponent << 23) | ((diff_mantissa << i) >> 9);
        
    if (!validate(*((float *)&result)))
    {
        exit(0);
    }
    else
    {
        return *((float *)&result);
    }


}

//requires: `f` and `g` are valid single precision floating-point numbers
// 1) terminates the program if `f` or `g` is invalid;
// 2) terminates as well if `f - g` is invalid;
// 3) returns `f - g`;
// 4) all calculations done by bit-wise operations
float subtract(float f, float g) {
    if (!validate(f) || !validate(g)) {
        exit(0);
    }

    // convert to unsigned ints and get their signs
    unsigned int fI = *(unsigned int *) &f;
    unsigned int gI = *(unsigned int *) &g;
    unsigned int signF = fI >> 31;
    unsigned int signG = gI >> 31;

    // change sign of the second float so we do subtraction
    if (signG) {
        gI = gI & 0x7FFFFFFF;
        signG = 0;
    } else {
        gI = gI | 0x80000000;
        signG = 1;
    }

    // convert new g back to float
    g = *((float *)(&gI));

    // use the correct addition function to get the result
    float result;
    if (signF == signG) {
	    result = adding1(f, g);
    } else {
	    result = adding2(f, g);
    }
    return result;
}
