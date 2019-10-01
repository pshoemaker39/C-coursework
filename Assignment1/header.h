/*-------------------------------------------------------------------------*
 *---									---*
 *---		header.h						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#define		MAX_LINE	256

#define		RANGE_LOWEST	0

#define		RANGE_HIGHEST	32767

#define		MIN_NUM_NUMBERS	0

#define		MAX_NUM_NUMBERS	0x40000000

extern void countWithList(int numNums);
extern void countWithTree(int numNums);
extern int getNextNumber();
