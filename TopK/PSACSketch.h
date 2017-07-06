#ifndef PSACSACSKETCH_H
#define PSACSACSKETCH_H

#include "common.h"
 
struct PSACSketch:public Sketch{
public:
	PSACSketch(uint d, uint w);
	~PSACSketch();
	void ConfigPro(uint *pro);
	void ConfigThres(uint *thres);
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str);

	void LoadPara(cuc *path = PSACPATH);
	bool Predict(uint *t);	
private:
	HashFunction *hf;
	uchar** sketch;
	float *para;
	float *mean;
	float *scale;
	uint *r;
	uint *th;
	uint *base;
	uint d;
	uint w;
	uint *t;

	uint level(uint x);
	void Add(uint rid, uint cid);
	uint CQuery(uint rid, uint cid);
};

#endif