#ifndef COMMON_H
#define COMMIN_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unordered_map"
#include "string"
#include "iostream"
using namespace std;
 
const char* prefix1 = "./Data/";
const char* prefix2 = "./Sample/";
const char* train = "./TrainSet/";
const char* test = "./TestSet/";

void GetName(int idx, int idx2, int mode, char* t){
	char buff[10];
	char bufff[10];
	sprintf(buff, "%d", idx);
	sprintf(bufff, "%d", idx2);
	switch(mode){
		case 0:{
			strcpy(t, prefix1);
			strcat(t, buff);
			strcat(t, "_");
			strcat(t, bufff);
			break;
		}
		case 1:{
			strcpy(t, prefix2);
			strcat(t, buff);
			strcat(t, "_");
			strcat(t, bufff);
			break;
		}
		case 2:{
			strcpy(t, train);
			strcat(t, buff);
            strcat(t, "_");
			strcat(t, bufff);
			break;
		}
		case 3:{
			strcpy(t, test);
			strcat(t, buff);
            strcat(t, "_");
			strcat(t, bufff);
			break;
		}
	}
	printf("%s\n", t);
}

bool ReadLine(FILE* file, unsigned char* t){
	unsigned int a[4];
	if(fscanf(file, "%d.%d.%d.%d", a, a+1, a+2, a+3)==EOF){ 
		return 0;
	}
	for(int i = 0; i < 4; ++i)
		t[i] = a[i]==0?'a':a[i];
	fscanf(file, "%d", a); 
	t[4] = (a[0]>>8)==0?'a':(a[0]>>8);
	t[5] = a[0]==0?'a':a[0];
	fscanf(file, "%d.%d.%d.%d", a, a+1, a+2, a+3);
	for(int i = 0; i < 4; ++i)
		t[i+6] = a[i]==0?'a':a[i];
	fscanf(file, "%d", a); 
	t[10] = (a[0]>>8)==0?'a':(a[0]>>8);
	t[11] = a[0]==0?'a':a[0];
	fscanf(file, "%d", a);
	t[12] = a[0]==0?'a':a[0];
	return 1;
}

#endif
