#include "CUSketch.h"

CUSketch::CUSketch(uint d, uint w):d(d), w(w){
	sketch = new ushort*[d];
	for(uint i = 0; i < d; ++i){
		 sketch[i] = new ushort[w]();
		 
	}
	hf = new HashFunction();
	para = new float[2*d-1];
	mean = new float[2*d-1];
	scale = new float[2*d-1];
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
		return Predict(t);
		//if you want a integer;
		//return (uint)Predict(t);		
	}
}

void CUSketch::PrintCounter(cuc* str){
	 
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

bool CUSketch::Predict(uint *t){
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