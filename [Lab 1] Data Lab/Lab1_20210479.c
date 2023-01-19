/*
 *
 * CSED 211, Fall 2022
 * DataLAb: Manipulating Bits
 * Assigned: 2022-09-07
 * Due: 2022-09-18 23:59 pm
 *
 * Namgyu Park (namgyu.park@postech.ac.kr) is the lead person for this assignment.
 *
*/

/*
* Stu Name: Lee Ju Hyeon
* Stu Num: 20210479
*/


#if 0
LAB L1 INSTRUCTIONS:
#endif

/*
 *   #Homework1-1 
 *   bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */

int bitNor(int x, int y)
{
	return ~(~(~x & ~y)); //using De Morgan's rule
}


/*
 *   #Homework1-2
 *   isZero - returns 1 if x == 0, and 0 otherwise 
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x)
{
	return !x; //every integer except for 0 is true
}


/* 
 *   #Homework1-3
 *   addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y)
{
	/* basic concept */
	//overflow occurs only on two situation written in below
	//situation 1: msbs of both int are 0, carry-in=1, and carry-out=0
	//situation 2: msbs of both int are 1, carrry-in=0, and carry-out=1

	/* how to solve */
	//first, we have to check if msbs of both int are same
	//second, we have to check carry-in^carry-out is 1 (that means, carry-in and carry-out have different value)
	//in this stage, it is much easier to use msb instead of carry-out.
	//we know that if there is an overflow, then carry-in^msb must be 1.
	//so we have to use negation in carry-in^msb term to get accurate result that we expect.
	return (((x ^ y) | ~(x ^ (x + y))) >> 31) & 1;
}


/*   
 *   #Homework1-4
 *   absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x)
{
	//To solve this task, we have to use the feature of bitwise shift (logical for positive numbers, and arithmetic for negative ones)
	//First term: doesn't affect positive ones, but act like not-operation for each bit of negative ones
	//Second term: doesn't add anything to positive ones, but add 1 to negative ones
	return (x ^ (x >> 31)) + (~(x >> 31) + 1);
}


/*   
 *   #Homework1-5
 *   logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int logicalShift(int x, int n)
{
	//First, we have to do the normal shift operation
	//If x is negative, additional xor operation is necessary. With that process, logical right shift would be completed.
	return (x >> n) ^ ((((x >> 31) << 31) >> n) << 1);
}

