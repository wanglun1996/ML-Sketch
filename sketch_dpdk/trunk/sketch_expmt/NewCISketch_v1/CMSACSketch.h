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
	void PrintCounter(cuc* str, uint acc_val);

	void LoadPara(cuc *path = CMSACPATH);
	float Predict(uint *t);	
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

CMSACSketch::CMSACSketch(uint d, uint w):d(d), w(w){
	srand(time(0));
	sketch = new uchar*[d];
	for(uint i = 0; i < d; ++i){
		sketch[i] = new uchar[w];
		memset(sketch[i], 0, sizeof(sketch[i]));
	}
	hf = new HashFunction();
	r = new uint[8];
	for(uint i = 0; i < 8; ++i){
		r[i] = DEF_PRO[i];
	}
	th = new uint[8];
	for(uint i = 0; i < 8; ++i){
		th[i] = DEF_THRES[i];
	}
	base = new uint[8];
	base[0] = th[0];
	for(uint i = 1; i < 8; ++i){
		base[i] = base[i-1]+(th[i]-th[i-1])*r[i];
	}
	para = new float[d];
	mean = new float[d];
	scale = new float[d];
	t = new uint[d];
}

CMSACSketch::~CMSACSketch(){
	for(uint i = 0; i < d; ++i) delete [] sketch[i];
	delete [] sketch;
	delete hf;
	delete [] r;
	delete [] th;
	delete [] base;
	delete [] para;
	delete [] mean;
	delete [] scale;
	delete [] t;
}

void CMSACSketch::ConfigPro(uint *pro){
	for(uint i = 0; i < 8; ++i){
		r[i] = pro[i];
	}
}

void CMSACSketch::ConfigThres(uint *thres){
	for(uint i = 0; i < 8; ++i){
		th[i] = thres[i];
	}
	base[0] = th[0];
	for(uint i = 1; i < 8; ++i){
		base[i] = base[i]+(th[i]-th[i-1])*r[i];
	}
}

void CMSACSketch::Insert(cuc *str){
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		Add(i, cid);
	}
}

uint CMSACSketch::Query(cuc *str, bool ml){
	memset(t, 0, sizeof(t));
    uint Min = INF_SAC;
    for(uint i = 0; i < d; ++i){
        uint cid = hf->Str2Int(str, i)%w;
        t[i] = CQuery(i, cid);
        Min = min(Min, t[i]);
    }
	if (!ml || !need_analyze(t, d)) {
		return (uint)Min;
	}
	else{
		for(uint i = 0; i < d; ++i){
			uint cid = hf->Str2Int(str, i)%w;
			t[i] = CQuery(i, cid);
		}
		std::sort(t, t+d);
		//if you want a float;
		return Predict(t);
		//if you want a integer;
		//return (uint)Predict(t);
	}
}

void CMSACSketch::PrintCounter(cuc* str, uint acc_val){
	memset(t, 0, sizeof(t));
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = CQuery(i, cid);
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

uint CMSACSketch::level(uint x){
	uint res = 8;
	for(uint i = 0; i < 8; ++i){
 		if(x < th[i]){
			res = i;
			break;
		}
	}
	if(res==-1) printf("data size is too big, one of the counter flow up\n");
	return res;
}

void CMSACSketch::Add(uint rid, uint cid){
	uint l = level(sketch[rid][cid]);
	if (l == 8) {
		return;
	}
	uint dice = rand()%r[l];
	if(!dice) ++sketch[rid][cid];
}
uint CMSACSketch::CQuery(uint rid, uint cid){
	uint v = sketch[rid][cid];
	uint l = level(v);
	if(l==0) return v;
    if (l == 8) // TODO
        return base[7];
	return base[l-1]+(v-th[l-1])*r[l];
}

void CMSACSketch::LoadPara(cuc *path){
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

float CMSACSketch::Predict(uint *t){
	float res = 0;
	for(uint i = 0; i < d; ++i){
		res += para[i]*(t[i]-mean[i])/scale[i];
	}
	return res;
}

#endif