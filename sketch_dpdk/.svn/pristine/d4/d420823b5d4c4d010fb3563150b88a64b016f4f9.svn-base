#ifndef PSKETCH_H
#define PSKETCH_H

#include "common.h"

struct PSketch:public Sketch{
public:
	PSketch(uint d, uint w);
	~PSketch();
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	bool PrintCounter(cuc * str, uint acc_val, char * buffer);
	void LoadPara(cuc *path = PPATH);
	float Predict(uint *t);
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

PSketch::PSketch(uint d, uint w):d(d), w(w){
	srand(time(0));
	sketch = new ushort*[d];
	for(uint i = 0; i < d; ++i){
		sketch[i] = new ushort[w];
		memset(sketch[i], 0, sizeof(sketch[i]));
	}
	hf = new HashFunction();
	para = new float[d];
	mean = new float[d];
	scale = new float[d];
	t = new uint[d];
}

PSketch::~PSketch(){
	for(uint i = 0; i < d; ++i) delete [] sketch[i];
	delete [] sketch;
	delete hf;
	delete [] para;
	delete [] t;
}

void PSketch::Insert(cuc *str){
	uint i = rand()%d;
	uint cid = hf->Str2Int(str, i)%w;
	++sketch[i][cid];
}

uint PSketch::Query(cuc *str, bool ml){
	if(!ml){
		uint sum = 0;
		for(uint i = 0; i < d; ++i){
			uint cid = hf->Str2Int(str, i)%w;
			sum += sketch[i][cid];
		}
		return sum;
	}
	else{
		memset(t, 0, sizeof(t));
		for(uint i = 0; i < d; ++i){
			uint cid = hf->Str2Int(str, i)%w;
			t[i] = sketch[i][cid];
		}
		std::sort(t, t+d);
		//if you want a float;
		float result = Predict(t);
		result = result > 0 ? result : 0;
		return result;
		//if you want a integer;
		//return (uint)Predict(t);		
	}
}

bool PSketch::PrintCounter(cuc* str, uint acc_val, char * buffer) {
	memset(t, 0, sizeof(t));
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = sketch[i][cid];
	}
    std::sort(t, t + d);
	sprintf(buffer, "%u", acc_val);
	for (uint i = 0; i < d; ++i){
		sprintf(buffer + strlen(buffer), " %u", t[i]);
	}

	return true;
}

void PSketch::LoadPara(cuc *path){
	FILE *file = fopen((const char*)path, "r");
	for(uint i = 0; i < d; ++i){
		fscanf(file, "%f", mean+i);
	}
	for(uint i = 0; i < d; ++i){
		fscanf(file, "%f", scale+i);
	}
	for(uint i = 0; i < d; ++i){
		fscanf(file, "%f", para+i);
	}
}

float PSketch::Predict(uint *t){
	float res = 0;
	for(uint i = 0; i < d; ++i){
		res += para[i]*(t[i]-mean[i])/scale[i];
	}
	return res;
}

#endif