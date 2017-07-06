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

void Find_topk(uint k, uint w, uint type, uint d) {
	string *str = new string[k];
	uint K;
	K = k;
	TopK* topk= new TopK(type, d, w, K);
	FILE *in_file = fopen(in_path_3, "r");
	char s[1024];
	//while(fscanf(in_file, "%s", s)!=EOF){
	printf("inserting...\n");
	while(fread(s, 1, 14, in_file) > 0){
		s[13] = '\0';
		itr = record->find(std::string((const char*)s));
		if(itr!=record->end()){
			++itr->second;
		}
		else{
			record->insert(std::make_pair(std::string((const char*)s), 1));
		}
		topk->Insert((cuc*)s);
	}
	KV *kv = new KV[K];
	uint i = 0;
	for(itt = topk->heap->begin(); itt != topk->heap->end(); itt++) {
		kv[i].key = itt->first;
		kv[i].value = itt->second;
		//printf("%u %u\n", topk->sketch->Query((cuc*)(itr->first).c_str()), itr->second);
		i++;
	}
	sort(kv, kv+K);
	freopen(out_path2, "w", stdout);
	for(i = 0; i < K; i++) {
		printf("%u ", kv[i].value);
		//itt = topk->heap->find(kv[i].key);
		//printf("%u %u %u ", (itt->second).oldValue, (itt->second).newValue, (itt->second).newValue - (itt->second).oldValue);
		topk->sketch->PrintCounter((cuc*)((kv[i].key).c_str()));
	}
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
	//GetTopK(1000);
	//system("python TopK.py MLData/example.train Para/example.para");
	//freopen(out_path2,"w", stdout);
	//float a = Find_topk(100, NOR_WID, 0, DEP, false, 0);
	//printf("%f\n", a);
	in_path_3 = argv[1];
	Find_topk(1000, NOR_WID, 0, DEP);
	//float a = Find_res(1000, NOR_WID, 0, DEP, true, 0);
	//printf("%f\n", a);
	return 0;
}