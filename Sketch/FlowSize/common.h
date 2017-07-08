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
#include <algorithm>

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

//const uint DEF_PRO[8] = {1, 4, 16, 64, 128, 256, 512, 1024};
const uint DEF_PRO[8] = {1, 16, 128, 512, 1024, 2048, 2048, 2048};
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
	virtual void PrintCounter(cuc *str, uint acc_val) = 0;
    virtual void LoadPara(cuc *str) = 0;
    bool need_analyze(uint * arr, int num) {
        std::sort(arr, arr + num);
//        return true;
//        for (int i = 0; i < num - 1; ++i) {
//            if (arr[i] == arr[i + 1]) {
//                return false;
//            }
//        }
        return (arr[1] != arr[0]);
    }
};

HashFunction::HashFunction(){
	hash_function[0] = &BOB1;
	hash_function[1] = &BOB2;
	hash_function[2] = &BOB3;
	hash_function[3] = &BOB4;
	hash_function[4] = &RSHash;
	hash_function[5] = &JSHash;
	hash_function[6] = &PJWHash;
	hash_function[7] = &ELFHash;
	hash_function[8] = &BKDRHash;
	hash_function[9] = &SDBMHash;
	hash_function[10] = &DJBHash;
	hash_function[11] = &DEKHash;
	hash_function[12] = &APHash;
	hash_function[13] = &DJBHash;
}

uint HashFunction::Str2Int(cuc* str, uint hidx, uint len){
	return hash_function[hidx](str, len);
}

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

uint BOB1(cuc* str, uint len){
	//register ub4 a,b,c,len;
	uint a,b,c;
	uint initval = 2;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

	/*---------------------------------------- handle most of the key */
	while (len >= 12){
		a += (str[0] +((uint)str[1]<<8) +((uint)str[2]<<16) +((uint)str[3]<<24));
		b += (str[4] +((uint)str[5]<<8) +((uint)str[6]<<16) +((uint)str[7]<<24));
		c += (str[8] +((uint)str[9]<<8) +((uint)str[10]<<16)+((uint)str[11]<<24));
		mix(a,b,c);
		str += 12; len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch(len)              /* all the case statements fall through */{
		case 11: c+=((uint)str[10]<<24);
		case 10: c+=((uint)str[9]<<16);
		case 9 : c+=((uint)str[8]<<8);
		/* the first byte of c is reserved for the length */
		case 8 : b+=((uint)str[7]<<24);
		case 7 : b+=((uint)str[6]<<16);
		case 6 : b+=((uint)str[5]<<8);
		case 5 : b+=str[4];
		case 4 : a+=((uint)str[3]<<24);
		case 3 : a+=((uint)str[2]<<16);
		case 2 : a+=((uint)str[1]<<8);
		case 1 : a+=str[0];
		/* case 0: nothing left to add */
	}
	mix(a,b,c);
	/*-------------------------------------------- report the result */
	return c;
}
 
uint BOB2(cuc* str, uint len){
	//register ub4 a,b,c,len;
	uint a,b,c;
	uint initval = 31;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

	/*---------------------------------------- handle most of the key */
	while (len >= 12){
		a += (str[0] +((uint)str[1]<<8) +((uint)str[2]<<16) +((uint)str[3]<<24));
		b += (str[4] +((uint)str[5]<<8) +((uint)str[6]<<16) +((uint)str[7]<<24));
		c += (str[8] +((uint)str[9]<<8) +((uint)str[10]<<16)+((uint)str[11]<<24));
		mix(a,b,c);
		str += 12; len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch(len)              /* all the case statements fall through */{
		case 11: c+=((uint)str[10]<<24);
		case 10: c+=((uint)str[9]<<16);
		case 9 : c+=((uint)str[8]<<8);
		/* the first byte of c is reserved for the length */
		case 8 : b+=((uint)str[7]<<24);
		case 7 : b+=((uint)str[6]<<16);
		case 6 : b+=((uint)str[5]<<8);
		case 5 : b+=str[4];
		case 4 : a+=((uint)str[3]<<24);
		case 3 : a+=((uint)str[2]<<16);
		case 2 : a+=((uint)str[1]<<8);
		case 1 : a+=str[0];
		/* case 0: nothing left to add */
	}
	mix(a,b,c);
	/*-------------------------------------------- report the result */
	return c;
}

uint 
BOB3(cuc * str, uint len){
	//register ub4 a,b,c,len;
	uint a,b,c;
	uint initval = 73;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

	/*---------------------------------------- handle most of the key */
	while (len >= 12){
		a += (str[0] +((uint)str[1]<<8) +((uint)str[2]<<16) +((uint)str[3]<<24));
		b += (str[4] +((uint)str[5]<<8) +((uint)str[6]<<16) +((uint)str[7]<<24));
		c += (str[8] +((uint)str[9]<<8) +((uint)str[10]<<16)+((uint)str[11]<<24));
		mix(a,b,c);
		str += 12; len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch(len)              /* all the case statements fall through */{
		case 11: c+=((uint)str[10]<<24);
		case 10: c+=((uint)str[9]<<16);
		case 9 : c+=((uint)str[8]<<8);
		/* the first byte of c is reserved for the length */
		case 8 : b+=((uint)str[7]<<24);
		case 7 : b+=((uint)str[6]<<16);
		case 6 : b+=((uint)str[5]<<8);
		case 5 : b+=str[4];
		case 4 : a+=((uint)str[3]<<24);
		case 3 : a+=((uint)str[2]<<16);
		case 2 : a+=((uint)str[1]<<8);
		case 1 : a+=str[0];
		/* case 0: nothing left to add */
	}
	mix(a,b,c);
	/*-------------------------------------------- report the result */
	return c;
}
uint 
BOB4(cuc * str, uint len){
	//register ub4 a,b,c,len;
	uint a,b,c;
	uint initval = 127;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

	/*---------------------------------------- handle most of the key */
	while (len >= 12){
		a += (str[0] +((uint)str[1]<<8) +((uint)str[2]<<16) +((uint)str[3]<<24));
		b += (str[4] +((uint)str[5]<<8) +((uint)str[6]<<16) +((uint)str[7]<<24));
		c += (str[8] +((uint)str[9]<<8) +((uint)str[10]<<16)+((uint)str[11]<<24));
		mix(a,b,c);
		str += 12; len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch(len)              /* all the case statements fall through */{
		case 11: c+=((uint)str[10]<<24);
		case 10: c+=((uint)str[9]<<16);
		case 9 : c+=((uint)str[8]<<8);
		/* the first byte of c is reserved for the length */
		case 8 : b+=((uint)str[7]<<24);
		case 7 : b+=((uint)str[6]<<16);
		case 6 : b+=((uint)str[5]<<8);
		case 5 : b+=str[4];
		case 4 : a+=((uint)str[3]<<24);
		case 3 : a+=((uint)str[2]<<16);
		case 2 : a+=((uint)str[1]<<8);
		case 1 : a+=str[0];
		/* case 0: nothing left to add */
	}
	mix(a,b,c);
	/*-------------------------------------------- report the result */
	return c;
}

uint RSHash(cuc *str, uint len){
	uint b = 378551;
	uint a = 63689;
	ull hash = 0;
	for(uint i = 0; i < len; ++i){
		hash = hash*a+str[i];
		a = a*b;
	}
	return (uint)hash;
}

uint JSHash(cuc *str, uint len){
	ull hash = 1315423911;
	for(uint i = 0; i < len; ++i){
		hash ^= ((hash<<5)+str[i]+(hash>>2));
	}
	return (uint)hash;
}


uint PJWHash(cuc* str, uint len){  
	ull BitsInUnsignedInt = (ull)32;  
	ull ThreeQuarters = (ull)((BitsInUnsignedInt  * 3) / 4);  
	ull OneEighth = (ull)(BitsInUnsignedInt / 8);  
	ull HighBits = (ull)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);  
	ull hash = 0;  
	ull test = 0;  
	for(uint i = 0; i < len; i++){  
		hash = (hash << OneEighth) + str[i];  
		if((test = hash & HighBits)  != 0){ 
			hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));  
		}
	}  
	return (uint)hash;  
}  

uint ELFHash(cuc *str, uint len){
	ull hash = 0;
	ull x = 0;
	for(uint i = 0; i < len; ++i){
		hash = (hash<<4)+str[i];
		if((x=hash&0xF0000000L)!=0){
			hash ^= (x>>24);
		}
		hash &= ~x;
	}
	return (uint)hash;
}

uint BKDRHash(cuc *str, uint len){
	ull seed = 131;
	ull hash = 0;
	for(uint i = 0; i < len; ++i){
		hash = (hash *seed) + str[i];
	}
	return (uint)hash;
}

uint SDBMHash(cuc*str, uint len){
	ull hash = 0;
	for(uint i = 0; i < len; ++i){
		hash = str[i] + (hash<<6) + (hash<<16) - hash;
	}
	return (uint)hash;
}

uint DJBHash(cuc* str, uint len){
	ull hash = 5381;
	for(uint i = 0; i < len; ++i){
		hash = ((hash << 5) + hash) + str[i];
	}
	return (uint)hash;
}

uint DEKHash(cuc *str, uint len){
	ull hash = len;
	for(uint i = 0; i < len; ++i){
		hash = ((hash<<5)^(hash<<27))^str[i];
	}
	return (uint)hash;
}

uint APHash(cuc *str, uint len){
	ull hash = 0xAAAAAAAA;
	for(uint i = 0; i < len; ++i){
		if((i&1)==0){
			hash ^= ((hash<<7)^str[i]*(hash>>3));
		}
		else{
			hash ^= (~((hash<<11)+str[i]^(hash>>5)));
		}
	}
	return (uint)hash;
}

#endif