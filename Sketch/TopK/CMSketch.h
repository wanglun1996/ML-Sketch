#ifndef CMSKETCH_H
#define CMSKETCH_H

#include "common.h"

struct CMSketch:public Sketch{
public:
	CMSketch(uint d, uint w);
	~CMSketch();
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str);

	void LoadPara(const char *path);
	bool Predict(cuc *str);
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