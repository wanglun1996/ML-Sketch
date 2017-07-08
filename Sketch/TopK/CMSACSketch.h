#ifndef CMSACSACSKETCH_H
#define CMSACSACSKETCH_H

#include "common.h"
 
struct CMSACSketch:public Sketch{
public:
	CMSACSketch(uint d, uint w);
	~CMSACSketch();
	void ConfigPro(uint *pro);
	void ConfigThres(uint *thres);
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str);

	void LoadPara(cuc *path = CMSACPATH);
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