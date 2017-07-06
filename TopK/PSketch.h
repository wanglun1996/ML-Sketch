#ifndef PSKETCH_H
#define PSKETCH_H

#include "common.h"

struct PSketch:public Sketch{
public:
	PSketch(uint d, uint w);
	~PSketch();
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str);

	void LoadPara(cuc *path = PPATH);
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