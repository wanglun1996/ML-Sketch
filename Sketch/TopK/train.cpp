#include "TopK.h"
#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;

#define DEP 4
#define NOR_WID 16384
#define SAC_WID 262144

const char* in_path = "./FlowData/sampleData";
const char* in_path_3 = "./FlowData/formatData";
const char* out_path1 = "./MLData/example.train";
const char* out_path2 = "./MLData/example.test";
const char* in_path_2 = "./Para/example.para";
const char* in_path_4 = "./MLData/example.result";

std::unordered_map<std::string, uint> *record = new std::unordered_map<std::string, uint>();
std::unordered_map<std::string, uint>::iterator itr;
std::unordered_map<std::string, NO>::iterator itt;
//TopK *topk = new TopK(0, DEP, NOR_WID, 1);

void GetTopK(const char* path, int k){
	FILE *in_file = fopen(path, "r");
	TopK *topk = new TopK(0, DEP, NOR_WID / 7, k);
	if(record != NULL) delete record;
	record = new std::unordered_map<std::string, uint>();
	char s[1024];
	int num = 0;
	//while(fscanf(in_file, "%s", s)!=EOF){
	std::unordered_map<std::string, uint> realTop;
	std::unordered_map<std::string, uint>::iterator it;
	while(fread(s, 1, 14, in_file) > 0){
		s[13] = '\0';
		num ++;
		//if(num > 900000) break;
		itr = record->find(std::string((const char*)s));
		//printf("\n");
		if(itr!=record->end()){
			//printf("yes\n");
			++itr->second;
			/*
			it = realTop.find(std::string((const char*)s));
			if(it != realTop.end()){
				++it->second;
			}else{
				if(realTop.size() < k){
					realTop.insert(std::make_pair(std::string((const char*)s), itr->second));
				}else{
					int MIN = 1000000;
					std::unordered_map<std::string, uint>::iterator MP;
					for(it = realTop.begin(); it != realTop.end(); it++){
						if(it->second < MIN){
							MP = it;
							MIN = it->second;
						}
					}
					if(MIN < itr->second){
						realTop.erase(MP);
						realTop.insert(std::make_pair(std::string((const char*)s), itr->second));
					}
				}
			}
			*/
			
		}
		else{
			record->insert(std::make_pair(std::string((const char*)s), 1));
		}		
		topk->Insert((cuc*)s);
	}
	/*
	KV *kv = new KV[k];
	uint i = 0;
	for(itr = realTop.begin(); itr != realTop.end(); itr ++)
	{
		kv[i].key = itr->first;
		kv[i].value = itr->second;
		i ++;
	}
	sort(kv, kv+k);
	*/
	freopen(out_path1, "a", stdout);

	//topk->sketch->LoadPara(in_path_2);

	uint right = 0;
	for(itt = topk->heap->begin(); itt != topk->heap->end(); itt++){
		//printf("%u ", right++);
		std::string s = itt->first;
		//if(realTop.find(s) != realTop.end()) {printf("0 "); right++;}
		
		uint v = (*record)[s];
		//printf("%d ", (int)v - (int)kv[k-1].value);
		printf("%u ", v);
		//if((float)itr->second / v > 2) printf("1 ");
		//else {printf("1 ");}
		//uint v = (*record)[s];
		//printf("%u ", v);
		//printf("%u %u ", itr->second, v);
		
		/*
		if(!topk->sketch->Predict((cuc*)(itr->first).c_str())) {
			printf("0 ");
		} else {
			printf("1 ");
		}
		*/
		//printf("%u ", v);
		//printf("%u %u %u ", (itt->second).oldValue, (itt->second).newValue, (itt->second).newValue - (itt->second).oldValue);
		topk->sketch->PrintCounter((cuc*)(itt->first).c_str());

	}


	//printf("%f\n", (float)right/k);
	/*
	std::vector<uint> v;
	for(itr = record->begin(); itr != record->end(); itr++){
		if(v.size() < k){
			bool ok = false;
			for(int i = 0; i < v.size(); i++){
				if(v[i] < itr->second){
					v.insert(v.begin()+i, itr->second);
					ok = true;
					break;
				}
			}
			if(!ok){
				v.push_back(itr->second);
			}
		} else {
			for(int i = 0; i < v.size(); i++){
				if(v[i] < itr->second){
					v.insert(v.begin()+i, itr->second);
					v.erase(v.begin()+(v.size()-1));
					break;
				}
			}
		}
	}

	for(int i = 0; i < v.size(); i++){
		printf("%u\n", v[i]);
	}
	*/
	/*
	for(itr = record->begin(); itr != record->end(); ++itr){
		//Inspiring Algorithm to judge if the flow is second kind fault;
		uint v = topk->sketch->Query((cuc*)(itr->first).c_str());
		char msg[128];
		sprintf(msg, "%u %u\n", v, itr->second);
		perror(msg);
		if(((float)v/itr->second)>2) printf("1 ");
		else printf("0 ");
		topk->sketch->PrintCounter((cuc*)(itr->first).c_str());
	}
	*/
	delete topk;
	return;
}


int main(int argc, char **argv){
	//printf("hello\n");
	/*
	char path[32];
	for(int i = 1; i <= 10; i++)
	{
		sprintf(path, "./FlowData/halfSample%d", i);
		GetTopK(path, 1000);
	}
	*/
	in_path = argv[1];
	//uint compress = atoi(argv[2]);
	GetTopK(in_path, 1000);
	//system("python TopK.py MLData/example.train Para/example.para");
	//freopen(out_path2,"w", stdout);
	//float a = Find_topk(100, NOR_WID, 0, DEP, false, 0);
	//printf("%f\n", a);
	//Find_topk(1000, NOR_WID, 0, DEP);
	//float a = Find_res(1000, NOR_WID, 0, DEP, true, 0);
	//printf("%f\n", a);
	return 0;
}