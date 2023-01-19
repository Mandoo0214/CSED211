/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

 /*
 * Stu Name: Lee Ju Hyeon
 * Stu Num: 20210479
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
    /* using two's complement */
    return ~x + 1;
}

/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y)
{
    /* First, there are two situations: x<0 && y>=0 or same sign && y - x > 0
    * So, we have to check each situation.
    * When checking, it is better to use MSB(sign bit), so we wiil make conditions with negaitve numbers so that sign bit will be 1.
    * And then, we can get right answer more easily. */

    /* Situation1: x < 0, y >= 0 */
    //compare the sign bits

    /* Situation2: has the same sign, y - x > 0 */
    //compare the sign bits
    //compare the difference
    //with the value comparison, we can check if x and y have the same value or not.

    /* we use >> 31 & 1 opearation in two situations. so we can merge then. */

    int situation1 = x & (~y); //sign bit will be 1 when x, y meet the first condition
    int situation2 = (x + (~y + 1)) & (~(x ^ y)); //sign bit will be 1 when x, y meet the second condition

    return ((situation1 | situation2) >> 31) & 1;
}

/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf)
{
    /* there is one exception: NaN */
    //so we have to check if uf is NaN or not. This comparison can be done by comparing bitwise expression

    unsigned abs = uf & 0x7FFFFFFF; //absolute value(sign bit changed as 0)

    if (abs > 0x7F800000) //if uf is NaN (with using abs, we can compare numbers more easily because we exactly know sign bit of abs variable
    {
        return uf; //if uf is Nan, return itself
    }
    
    else //if not, return abs
    {
        return abs;
    }
}

/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf)
{
    /* we have to separate numbers as three categories: normalized, denormalized, NaN */
    //normalized: +1 for exp
    //denormalized: left shift
    //NaN: return original one

    unsigned sign = uf & 0x80000000; //sign bit (1 bit)
    unsigned exp = (uf >> 23) & 0xFF; //exp bit (8 bit)
    unsigned frac = uf & 0x7FFFFF; //frac bit (23 bit)

    if (exp == 0xFF) //when uf is NaN
    {
        return uf;
    }

    else if (exp == 0) //when uf is denormalized
    {
        return sign | (exp << 23) | (frac << 1); //we can get double number by multiplying 2 to frac number
    }

    else //when uf is normalized
    {
        return uf + (1 << 23); //we can easliy get double number by adding 1 to exp number
    }
}

/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x)
{
    /* there are two exceptions: 0 and TMin */
    //if x is 0: return 0
    //if x is TMin: can't calculate -> return constant that responds to float casting in accordance with IEEE rules

    /* for the ordinary situation, number will be casted following IEEE rules */
    //sign: MSB
    //exp: calculating shift counts -> add it to bias
    //frac: extract 23 bits -> execute rounding if necessary

    unsigned sign = x & 0x80000000; //MSB of x will be sign bit
    unsigned shift = 1, exp, frac, round;

    if (x == 0)//if x is 0
    {
        return 0;
    }

    else if (x == 0x80000000) //if x is TMin
    {
        return 0xCF000000; //float-casted constant of 0x80000000
    }

    else //all of the etc situation -> following IEEE rule
    {
        if (sign) //if x is negative, we have to change it to positive
        {
            x = -x;
        }

        while ((x >> shift) != 0)
        {
            shift++;
        }

        shift--; //substracting unnecessary 1
        exp = shift + 127; //calculating exp bits
        frac = (((x << (31 - shift)) >> 8) & 0x7FFFFF); //calculating frac bits using shift variable

        if (shift > 23) //when rounding is necessary
        {
            round = (x << (31 - shift)) & 0xFF; //comparison variable for rounding

            if ((round > 128) || ((round == 128) && (frac & 1))) //when frac increasing is needed
            {
                frac++;

                if (frac >> 23) //when exceeding 23 bits, the values need to be adjusted
                {
                    exp++;
                    frac = 0;
                }
            }

        }
    }

    return sign | (exp << 23) | frac;
}

/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf)
{
    /* there are two exceptions: NaN and infinity */
    //according to explanation, we have to return 0x80000000u in this special situations 
    
    /* ordinary ones separate as two categories: denormalized and normalized */
    //denormalized: return 0
    //normalized: calculate the opposite of the previous function(i2f)
    //Cf: in this process, we have to treat another exception-overflow

    unsigned sign = !!(uf >> 31); //sign bit (1 bit)
    unsigned exp = (uf >> 23) & 0xFF; //exp bit (8 bit)
    unsigned frac = uf & 0x7FFFFF; //frac bit (23 bit)
    unsigned answer; //will store final integer number

    if (exp == 0xFF) //special situation
    {
        return 0x80000000u;
    }

    if (exp < 0x7F) //denormalized
    {
        return 0x0;
    }

    /* else: normalized */
    exp -= 0x7F; //substracting bias

    /* handling exceptions */
    if (exp >= 31) //overflow: another exception
    {
        return 0x80000000u;
    }

    /* shifting frac bits according to exp bits */
    if (exp >= 23) //categorized according to the value of exp (based on 23)
    {
        answer = frac << (exp - 23);
    }

    else
    {
        answer = frac >> (23 - exp);
    }

    /* exponential processing */
    answer += (1 << exp);

    /* correcting sign according to the original number */
    if (sign) //if original was negative, sign of 'answer' variable should be changed 
    {
        answer = -answer;
    }

    return answer;
}
