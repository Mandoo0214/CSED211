/*
 * CSED211 Lab12&13 Malloc lab
 *
 * STUDENT INFO: 20210479 Lee Juhyeon
 * LOGIN ID: lanthanum14
 * 
 * This c code file does dynamic allocation performance. The detailed descriptions of each function are written at the top of each function.
 * Every time we perform malloc, we should run mm_init first.
 * And then, mm_malloc or mm_realloc performs their own function.
 * Finally, mm_free will unallocate useless blocks.
 *
 * Subrutine extend_heap, coalesce, find_fit, place would help the functions of mm_init, mm_malloc, mm_realloc, and mm_free.
 * Descriptions of each subrutine are also written at the very beginning of each subrutine.
 *
 * In addition, this file contains plenty of preprocess statements. Each statements has a comment that tells what does this statement do.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* elements used in alignment or expension */
#define WSIZE 4 //word size, and also size of header and footer
#define DSIZE 8 //double word size
#define CHUNKSIZE (1<<12) //amount to increase heap

/* perform basic arithmetic operations */
#define MAX(x, y) ((x) > (y) ? (x) : (y)) //returns bigger one between entered two numbers
#define MIN(x, y) ((x) > (y) ? (y) : (x)) //returns smaller one between entered two numbers

/* return combination of size and alloc */
#define PACK(size, alloc) ((size) | (alloc))

/* perform reading/writing operations in address of p: using this functions, we can contain appropriate informations in header and footer */
#define GET(p) (*(unsigned int *)(p)) //reading
#define PUT(p, val) (*(unsigned int *)(p) = (val)) //writing

/* get informations from allocated field that is contained in address p */
#define GET_SIZE(p) (GET(p) & ~0x7) //get size information (remove 3bits in LSB)
#define GET_ALLOC(p) (GET(p) & 0x1) //get allocated fields (check the allocate bit)

/* get address of header or footer from given block ptr */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* get address of next of previous blocks from given block ptr */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* declartions of static functinos */
static char *heap_listp; //ptr used in mm_init and points to the very beginning of heap
static void *extend_heap(size_t words); //used in two situations: (1) initializing heap (2) failed to find a suitable block during mm_malloc operation
static void *coalesce(void *bp); //processing block merging
static void *find_fit(size_t asize); //finding proper block for requested size
static void place(void *bp, size_t asize); //assign data to the proper location

/* 
 * mm_init - creating & initailizing heap
 *
 * First, if statment tries to create and initialize empty heap.
 * If there is no problem, then mm_init just goes on.
 * If the return value of mem_sbrk is (void *)-1, then it means
 * some problem happened during creating heap. Thus then we should return -1.
 *
 * After that operation, PUT statements try to put alignment padding,
 * prologue header, prologue footer, and epilogue header. Detailed information of each ones
 * are explained in individual comments. And after putting them, heap start pointer starts to
 * refer to the very back of prologue footer.
 *
 * Finally, if statement tries to extend heap with a free block. The size of that block is
 * CHUNKSIZE bytes. And if there is any problem with extending heap, this function will return -1 and terminates.
 * If there is no problem, then mm_init returns 0 and terminates.
 *
 */
int mm_init(void)
{
    /* create and initialize empty heap */
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1) { return -1; }

    PUT(heap_listp, 0); //alignment padding (unused block at the very beginning of heap)
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); //prologue header (4bytes away from the start point, contain ALIGNMENT | 1)
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); //prologue footer (8bytes away from the start point, contain ALIGNMENT | 1)
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1)); //epilogue header (12bytes away from the start point of heap, contain 0 | 1)
    heap_listp += (2 * WSIZE); //now, heap start pointer refers to the very back of the prologue footer

    /* extend heap (extending extra size: CHUNKSIZE / WSIZE bytes) */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) { return -1; }
    
    /* return value 0 means that mm_init terminates with no problem (exceptions were dealt with at that time along this function) */
    return 0;
}

/* 
 * extend_heap - extend the size of heap
 * 
 * First, we have to calibrate word size to maintain alignment policy.
 * And then, bp will point to the very beginning of the heap.
 * If there are any problem with this opeation, then extend_heap will terminate with the return value 'NULL'.
 *
 * And similar to mm_init, we should initialize free block header, footer, and epilogue header.
 * Finally, we have to check if there are any free block near this block.
 * If there are some, then coalescing opeation should happen.
 * And then, extend_heap will terminate.
 *
 */
static void *extend_heap(size_t words)
{
    char *bp; //block ptr
    size_t size; //will contain aligned size of words

    /* calibrating word size to maintain alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

    /* extend heap with size bytes: if some problems happen, then extend_heap would return NULL and terminate */
    if ((long)(bp = mem_sbrk(size)) == -1) { return NULL; }

    /* initializing free block header, footer and epilogue header with informations of size */
    PUT(HDRP(bp), PACK(size, 0)); //free block header (find location from bp using HDRP)
    PUT(FTRP(bp), PACK(size, 0)); //free block footer (find location from bp using FTRP)
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); //epilogue header (find location from bp using NEXT_BLKP)

    /* before terminate extend_heap, we have to check if there are any free block near this block */
    return coalesce(bp);
}

/*
 * coalesce - managing free blocks in heap
 *
 * There are four cases where coalescing occurs.
 * In this function, they are separated by if-else statement.
 *
 * First case: no free block
 * Second case: next free block
 * Third case: previous free block
 * Fourth case: next & pervious free block
 *
 * The description of appropriate operation in each case was written in individual comments.
 * In every call of coalesce, local variables set by bp. And depending on their values, it leads to proper operations via if statement conditions.
 * At the end of this function, coalesce just returns bp and terminates.
 *
 */
static void *coalesce(void *bp)
{
    /* declartion of local variables */
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))); //bring the value of allocate check bit from previous block
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp))); //bring the value of alloacte check bit from next block
    size_t size = GET_SIZE(HDRP(bp)); //get size information from current block

    /* case 1: when there is no free block */
    if (prev_alloc && next_alloc) { return bp; } //just return bp and terminate

    /* case 2: when the next block is free */
    else if (prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp))); //size will be increased by the size of the next block

        PUT(HDRP(bp), PACK(size, 0)); //creating header for new combined block
        PUT(FTRP(bp), PACK(size, 0)); //creating footer for new combined block
    }

    /* case 3: when the previous block is free */
    else if (!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp))); //size will be increased by the size of the previous block

        PUT(FTRP(bp), PACK(size, 0)); //creating footer for new combined block
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0)); //creating header for new combined block

        bp = PREV_BLKP(bp); //now bp points to the previous block  
    }

    /* case 4: when both the next and the previous blocks are free */
    else
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp))); //size will be increased by the sum of the sizes of next and previous block

        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0)); //creating header for new combined block
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0)); //creating footer for new combined block

        bp = PREV_BLKP(bp); //now bp points to the front part of new combined block
    }

    return bp; //coalesce returns the address that bp contains when it terminates
}

/* 
 * mm_malloc - allocate a block, and heap expansion is also performed in some cases
 *
 * First, we have to set aligned values according to real size of data because we have to maintain alignment.
 * And then we search if there is appropriate block or not.
 * If there exists some proper block, mm_malloc allocate the data into there.
 *
 * But there are no appropriate block, mm_malloc call extend_heap and create new block that has aligned size.
 * And then mm_malloc assigns requested data into that block.
 *
 */
void *mm_malloc(size_t size)
{
    size_t asize; //aligned block size
    size_t extendsize; //amount to extend heap (used when if there are no appropriate free block)
    char *bp; //block ptr

    if (size == 0) { return NULL; } //check if it is fake call and ignore if it is

    /* calibrating block size according to the real requested size */
    if (size <= DSIZE) { asize = 2 * DSIZE; } //if size is smaller or same with 8 bytes, then size will adjusted as 16 bytes (real size will be up to 12 bytes)
    else { asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE); } //if size is bigger than 8 bytes, then size will be adjusted to multiple of 8

    /* search if there is appropriate fit block or not */
    if ((bp = find_fit(asize)) != NULL) //if there are proper fit block
    {
        place(bp, asize); //allocate requested data to there
        return bp; //returns the address of bp and terminates
    }

    /* 
     * if function operates these statements, then it means that there is no proper fit block for requested data
     * Thus, we have to extend heap and put data in there.
     */
    extendsize = MAX(asize, CHUNKSIZE); //set extendsize that can fit all of requested data

    /* call extend_heap and try to extend heap */
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL) { return NULL; } //if there are any problems in extending heap, then mm_malloc will terminate with return value of NULL

    place(bp, asize); //if extending operation ended with no problems, then allocate requested data into heap
    
    return bp; //returns the address of bp and terminates
}

/*
 * mm_free - unallocating blocks
 *
 * First, we should set local variable 'size' with the size of block that will be unallocated.
 * And then, mm_free will modify informations in header and footer of that block.
 * Finally, mm_free call coalesce and deal with a possible block merging.
 *
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr)); //contain the size of block that will be unallocate in this function

    PUT(HDRP(ptr), PACK(size, 0)); //set header as 'unallocated'
    PUT(FTRP(ptr), PACK(size, 0)); //set footer as 'unallocated'

    coalesce(ptr); //search if there are free blocks near this block, and process merging them if there are some
}

/*
 * mm_realloc - reallocating with new size
 *
 * First, mm_realloc() deals with two exceptions: null ptr and zero size.
 * NULL ptr means mm_malloc(), so in that situation, mm_realloc() will terminate just after call mm_malloc().
 * And zero size means free that block. So in this situation, mm_realloc will just call mm_free() and returns with the NULL value.
 *
 * If the parameters are normal, then mm_realloc declares 'newp' pointer and tries to allocate new block with entered variable 'size'.
 * And if there are no problems with that allocation, memcpy() copies original data to new block.
 * When this copying operation happens, copysize will be the minimum value between old block size and new size.
 *
 * Then, mm_realloc() frees original block and terminates.
 *
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *newp; //points to newly allocated block
    size_t csize; //contains the size for data copy to occur

    /* if ptr is NULL, then it is same with call mm_malloc with size */
    if (ptr == NULL) { return mm_malloc(size); }
    
    /* if size is 0, then it has same meaning with free that block (Cf: mininum value of size is 0 since the type of size is size_t) */
    else if (size == 0) { mm_free(ptr); return NULL; }

    /* if ptr is not NULL, then perform realloc operation */
    if ((newp = mm_malloc(size)) == NULL) { return NULL; }

    csize = MIN(GET_SIZE(HDRP(ptr)), size); //csize will be the minimum value between old block size and new size

    memcpy(newp, ptr, csize); //copying operation: from original data to new block
    mm_free(ptr); //freeing original block

    return newp;
}

/*
 * find_fit - search for proper fit block
 *
 * In this function, bp will search block that fits for requested size.
 * When there are free block that fits, find_fit tests that block.
 * If that block meets the condition(unallocated state, enough size) find_fit returns the address of that block
 * If there is no block that meets the condition, then find_fit returns NULL and terminates.
 *
 */
static void *find_fit(size_t asize)
{
    void *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) //find_fit searches all blocks in heap until they reach the end or find proper one
    {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) { return bp; } //if they find appropriate one and if it meets the condition, then find_fit terminates with return value of address of that block
    }

    return NULL; //if there are no blocks that meet condition, then find_fit would return NULL and terminate
}

/*
 * place - perform actual allocation
 *
 * This function calculates the difference between free block size and real necessary size.
 * And if the difference is bigger than 2 * ALIGNMENT, i.e., 16 bytes, then place perform separation.
 *
 * If difference is less than 16 bytes, then place just does setting header and footer: allocation.
 *
 * And then place terminates.
 *
 */
static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp)); //store size of the block that will be used for allocating requested data

    /* case 1: difference between free block size and real necessary size is bigger than 2 * ALIGNMENT */
    if ((csize - asize) >= (2 * DSIZE))
    {
        PUT(HDRP(bp), PACK(asize, 1)); //set header for the brand-new allocated block
        PUT(FTRP(bp), PACK(asize, 1)); //set footer for the brand-new allocated block
        
        bp = NEXT_BLKP(bp); //change the point that bp directs

        /* separate empty part as a new free block */
        PUT(HDRP(bp), PACK(csize - asize, 0)); //created new header
        PUT(FTRP(bp), PACK(csize - asize, 0)); //created new footer
    }

    /* case 2: difference is smaller than 2 * ALIGNMENT */
    else
    {
        PUT(HDRP(bp), PACK(csize, 1)); //set header for the brand-new allocated block
        PUT(FTRP(bp), PACK(csize, 1)); //set footer for the brand-new allocated block
    }
}