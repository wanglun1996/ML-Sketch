#include "CMSACSketch.h"

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
	para = new float[2*d-1];
	mean = new float[2*d-1];
	scale = new float[2*d-1];
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
	if(!ml){
		uint Min = INF_SAC;
		for(uint i = 0; i < d; ++i){
			uint cid = hf->Str2Int(str, i)%w;
			t[i] = CQuery(i, cid);
			Min = min(Min, t[i]);
		}
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

void CMSACSketch::PrintCounter(cuc* str){
	memset(t, 0, sizeof(t));
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = CQuery(i, cid);
	}	
	std::sort(t, t+d);
	printf("%u", t[0]);
	for(uint i = 1; i < d; ++i){
		printf(" %u", t[i]);
	}
	printf("\n");
	delete [] t;
}

uint CMSACSketch::level(uint x){
	uint res = -1;
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
	uint dice = rand()%r[l];
	if(!dice) ++sketch[rid][cid];
}
uint CMSACSketch::CQuery(uint rid, uint cid){
	uint v = sketch[rid][cid];
	uint l = level(v);
	if(l==0) return v;
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


bool CMSACSketch::Predict(uint *t){
	float res = 0;
	uint *tt = new uint[2*d-1];
	uint i;
	for(i = 0; i < d-1; ++i){
		tt[i] = t[i+1]/t[0];
	}
	for(; i < 2*d-1; ++i){
		tt[i] = t[i-d+1];
	}
	for(i = 0; i < 2*d-1; ++i){
		res += para[i]*(tt[i]-mean[i])/scale[i];
	}
	res = pow(2.71828, -res);
	++res;
	res = 1/res;
	return (res>0.5);
}