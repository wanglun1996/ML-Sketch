#ifndef CUSKETCH_H 
#define CUSKETCH_H

#include "common.h"

struct CUSketch:public Sketch{
public:
	CUSketch(uint d, uint w);
	~CUSketch();
	void Insert(cuc *str);
	uint Query(cuc *str, bool ml = FALSE);
	void PrintCounter(cuc* str, uint acc_val);

	void LoadPara(cuc *path = CUPATH);
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

CUSketch::CUSketch(uint d, uint w):d(d), w(w){
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

CUSketch::~CUSketch(){
	for(uint i = 0; i < d; ++i) delete [] sketch[i];
	delete [] sketch;
	delete hf;
	delete [] para;
	delete [] mean;
	delete [] scale;
	delete [] t;
}

void CUSketch::Insert(cuc *str){
	memset(t, 0, sizeof(t));
	uint Min = INF_SHORT;
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = sketch[i][cid];
		Min = min(Min, t[i]);
	}
	for(uint i = 0; i < d; ++i){
		if(t[i]==Min){
			uint cid = hf->Str2Int(str, i)%w;
			++sketch[i][cid];
		}
	}
}

uint CUSketch::Query(cuc *str, bool ml){
	memset(t, 0, sizeof(t));
    uint Min = INF_SHORT;
    for(uint i = 0; i < d; ++i){
        uint cid = hf->Str2Int(str, i)%w;
        t[i] = sketch[i][cid];
        Min = min(Min, t[i]);
    }
	if (!ml || !need_analyze(t, d)) {
		return (uint)Min;
	}
	else {
		std::sort(t, t+d);
		//if you want a float;
		return Predict(t);
		//if you want a integer;
		//return (uint)Predict(t);		
	}
}

void CUSketch::PrintCounter(cuc* str, uint acc_val){
	memset(t, 0, sizeof(t));

	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = sketch[i][cid];
	}

    if (need_analyze(t, d)) {
        std::sort(t, t+d);
        printf("%u", acc_val);
        for(uint i = 0; i < d; ++i){
            printf(" %u", t[i]);
        }
        printf("\n");
    }
}

void CUSketch::LoadPara(cuc *path){
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

float CUSketch::Predict(uint *t){
	float res = 0;
	for(uint i = 0; i < d; ++i){
		res += para[i]*(t[i]-mean[i])/scale[i];
	}
	return res;
}

#endif