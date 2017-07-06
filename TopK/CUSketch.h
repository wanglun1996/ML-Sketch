#ifndef CUSKETCH_H 
#define CUSKETCH_H

#include "common.h"

struct CUSketch:public Sketch{
public:
	CUSketch(uint d, uint w);
	~CUSketch();
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str);

	void LoadPara(cuc *path = CUPATH);
	bool Predict(uint *t);
private:
	HashFunction *hf;
	ushort** sketch;
	float* para;
	float* mean;
	float* scale;
	uint d;
	uint w;
	uint *t;
};

#endif