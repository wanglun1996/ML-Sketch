#include "CMSketch.h"

CMSketch::CMSketch(uint d, uint w):d(d), w(w){
	sketch = new ushort*[d];
	for(uint i = 0; i < d; ++i){
		sketch[i] = new ushort[w];
		memset(sketch[i], 0, sizeof(sketch[i]));
	}
	hf = new HashFunction();
	para = new float[2*d-1];
	mean = new float[2*d-1];
	scale = new float[2*d-1];
	t = new uint[d];
}

CMSketch::~CMSketch(){
	for(uint i = 0; i < d; ++i) delete [] sketch[i];
	delete [] sketch;
	delete hf;
	delete [] para;
	delete [] mean;
	delete [] scale;
	delete [] t;
}

void CMSketch::Insert(cuc *str){
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		if(sketch[i][cid] < 65535) ++sketch[i][cid];
	}
}

uint CMSketch::Query(cuc *str, bool ml){
	memset(t, 0, sizeof(t));
	if(!ml){
		uint Min = INF_SHORT;
		for(uint i = 0; i < d; ++i){
			uint cid = hf->Str2Int(str, i)%w;
			t[i] = sketch[i][cid];
			Min = min(Min, t[i]);
		}
		return (uint)Min;
	}
	else{
		for(uint i = 0; i < d; ++i){
			uint cid = hf->Str2Int(str, i)%w;
			t[i] = sketch[i][cid];
		}
		std::sort(t, t+d);
		//if you want a float;
		return Predict(str);
		//if you want a integer;
		//return (uint)Predict(t);
	}
}

void CMSketch::PrintCounter(cuc* str){
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = sketch[i][cid];
	}	
	std::sort(t, t+d);
	printf("%u", t[0]);
	for(uint i = 1; i < d; ++i){
		printf(" %u", t[i]);
	}
	printf("\n");
}

void CMSketch::LoadPara(const char *path){
	FILE *file = fopen(path, "r");
	uint i;
	for(i = 0; i < 2*d-1; ++i){
		fscanf(file, "%f", mean+i);
	}
	for( i = 0; i < 2*d-1; ++i){
		fscanf(file, "%f", scale+i);
	}
	for( i = 0; i < 2*d-1; ++i){
		fscanf(file, "%f", para+i);
	}
}

bool CMSketch::Predict(cuc* str){
	for(uint i = 0; i < d; ++i){
		uint cid = hf->Str2Int(str, i)%w;
		t[i] = sketch[i][cid];
	}
	//printf("Predict1\n");
	std::sort(t, t+d);
	//printf("Predict2\n");
	float res = 0;
	uint *tt = new uint[2*d-1];
	uint i;
	for(i = 0; i < d-1; ++i){
		//printf("%u\n", i);
		tt[i] = t[i+1]/t[0];
	}
	//printf("Predict3\n");
	for(; i < 2*d-1; ++i){
		//printf("%u %u\n", i, i-d+1);
		tt[i] = t[i-d+1];
	}
	//printf("Predict4\n");
	for(i = 0; i < 2*d-1; ++i){
		res += para[i]*(tt[i]-mean[i])/scale[i];
	}
	printf("%f ", res);
	res = pow(2.71828, -res);printf("%f ", res);
	++res;printf("%f ", res);
	res = 1/res;
	printf("%f ", res);
	return (res>0.5);
}