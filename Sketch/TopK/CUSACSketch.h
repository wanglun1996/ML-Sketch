#ifndef CUSACSACSKETCH_H
#define CUSACSACSKETCH_H

#include "common.h"
 
struct CUSACSketch:public Sketch{
public:
	CUSACSketch(uint d, uint w);
	~CUSACSketch();
	void ConfigPro(uint *pro);
	void ConfigThres(uint *thres);
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str);

	void LoadPara(cuc *path = CUSACPATH);
	bool Predict(uint *t);	
private:
	HashFunction *hf;
	uchar** sketch;
	float *para;
	float *mean;
	float *scale;
	uint *r;
	float *qr;
	uint *th;
	uint *base;
	uint d;
	uint w;
	uint *t;


	uint level(uint x);
	uint C(uint m, uint n);
	float getrate(uint x, uint index);
	void Add(uint rid, uint cid);
	uint CQuery(uint rid, uint cid);
};

#endif