#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <iostream>

#define FALSE 0
#define TRUE 1
#define STR_LEN 13
#define INF_SAC (40000)
#define INF_SHORT ((1<<16)-1)
#define min(x, y) (x<y?x:y)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint; 
typedef long long unsigned int ull;
typedef const unsigned char cuc;
typedef unsigned int(*funPtr)(const unsigned char*, unsigned int);

#define CMPATH ((cuc*)"./Para/CMSAC.para")
#define CUPATH ((cuc*)"./Para/CUSAC.para")
#define PPATH ((cuc*)"./Para/PSAC.para")
#define CMSACPATH ((cuc*)"./Para/CMSAC.para")
#define CUSACPATH ((cuc*)"./Para/CUSAC.para")
#define PSACPATH ((cuc*)"./Para/PSAC.para")

const uint DEF_PRO[8] = {1, 4, 16, 64, 128, 256, 512, 1024};
const uint DEF_THRES[8] = {128, 192, 224, 240, 248, 252, 254, 255};

uint BOB1(cuc *str, uint len);
uint BOB2(cuc *str, uint len);
uint BOB3(cuc *str, uint len);
uint BOB4(cuc *str, uint len);
uint RSHash(cuc *str, uint len);
uint JSHash(cuc *str, uint len);
uint PJWHash(cuc* str, uint len);
uint ELFHash(cuc *str, uint len);
uint BKDRHash(cuc *str, uint len);
uint SDBMHash(cuc*str, uint len);
uint DJBHash(cuc* str, uint len);
uint DEKHash(cuc *str, uint len);
uint APHash(cuc *str, uint len);
uint DJBHash(cuc *str, uint len);

struct HashFunction{
	HashFunction();
	uint Str2Int(cuc* str, uint hidx, uint len = STR_LEN);
	funPtr hash_function[14];
};

struct Sketch{
	virtual void Insert(cuc *str) = 0;
	virtual uint Query(cuc *str, bool ml = FALSE) = 0;
	virtual void PrintCounter(cuc *str) = 0;
};

#endif