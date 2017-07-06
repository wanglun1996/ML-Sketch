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

bool Find_in_str(string s, string* str, uint k)
{
	for(uint i = 0; i < k; i++)
	{
		if(s == str[i])
			return true;
	}
	return false;
}
uint Compare(string* str1, string* str2,uint k, TopK *topk)
{
	/*
	freopen("./MLData/wrong.txt", "w", stdout);
	FILE *in_file2 = fopen(in_path_4, "r");
	uint K = 2000;
	KV *kv = new KV[K];
	uint i = 0;
	for(itr = topk->heap->begin(); itr != topk->heap->end(); itr++) {
		kv[i].key = itr->first;
		kv[i].value = itr->second;
		i++;
	}
	sort(kv, kv+K);
	for(i = 0; i < K; i++)
	{
		float temp;
		fscanf(in_file2, "%f", &temp);
		kv[i].value = temp;
	}
	fclose(in_file2);
	*/
	uint cnt=0;
	for(uint i = 0;i < k;i++)
	{
		if(Find_in_str(str1[i], str2,k))
			cnt++;
		/*
		else
		{
			uint predict = 0;
			for(uint j = 0; j < k; j++)
			{
				if(kv[j].key == str1[i])
				{
					predict = kv[j].value;
					break;
				}
			}
			printf("%u ", record->find(str1[i])->second);
			topk->sketch->PrintCounter((cuc*)(str1[i]).c_str());
		}
		*/
	}
	return cnt;
	return float(cnt)/k;

}
string* Topk_r(uint k, TopK *topk)
{
	string* str = new string[k];
	/*
	uint c;
	uint i;
	uint m;
	string s;
	for( c = 0; c < k; c++)
	{
		m = 0;
		s = "";
		for(itr = record->begin(); itr!=record->end();itr++)
		{
			if(itr->second > m)
			{
				if(!Find_in_str(itr->first, str, c))
				{
					s = itr->first;
					m = itr->second;
				}
			}
		}
		str[c] = s;
	}
	*/
	
	std::unordered_map<std::string, uint> realTop;
	std::unordered_map<std::string, uint>::iterator MP;
	MP = realTop.end();
	uint min = 10000000;
	for(itr = record->begin(); itr != record->end(); itr++) {
		if(realTop.size() < k) {
			realTop.insert(std::make_pair(itr->first, itr->second));
			if(itr->second < min) {
				min = itr->second;
				MP = realTop.find(itr->first);
			}
		} else {
			if(itr->second > min) {
				realTop.erase(MP);
				realTop.insert(std::make_pair(itr->first, itr->second));
				min = 1000000;
				std::unordered_map<std::string, uint>::iterator it;
				for(it = realTop.begin(); it != realTop.end(); it++) {
					if(it->second < min) {
						min = it->second;
						MP = it;
					}
				}
			}
		}
	}
	
	KV *kv = new KV[k];
	uint i = 0;
	for(itr = realTop.begin(); itr != realTop.end(); itr ++) {
		kv[i].key = itr->first;
		kv[i].value = itr->second;
		i ++;
	}
	
	sort(kv, kv+k);
	//freopen("./MLData/right.txt", "w", stdout);
	
	for(i = 0; i < k; i++) {
		str[i] = kv[i].key;
		//printf("%u ", kv[i].value);
		//topk->sketch->PrintCounter((cuc*)(kv[i].key).c_str());
	}
	
	//printf("real min: %u\n", kv[k-1].value);
	return str;
}

bool mysort(const pair<std::string,uint> &x, const pair<std::string, uint> &y)
{
	return x.second > y.second;
}


float Find_res(uint k, uint w, uint type, uint d, bool l, uint an)
{
	//string *str = new string[k];
	if(record != NULL) delete record;
	record = new std::unordered_map<std::string, uint>();
	record->clear();
	KV *kv = new KV[k];
	memset(kv, 0, sizeof(KV)*k);
	uint K;
	if(l)
	K = k;
	else
	K = k;
	TopK* topk= new TopK(type, d, w, K);
	FILE *in_file = fopen(in_path_3, "r");
	char s[1024];
	//while(fscanf(in_file, "%s", s)!=EOF){
	//printf("inserting...\n");
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
	//return 0;
	if(l)
	{
		FILE *in_file2 = fopen(in_path_4, "r");
		//KV *kv = new KV[K];
		uint i = 0;
		for(itt = topk->heap->begin(); itt != topk->heap->end(); itt++) {
			kv[i].key = itt->first;
			kv[i].value = itt->second;
			i++;
		}
		sort(kv, kv+K);
		
		for(i = 0; i < K; i++)
		{
			float temp;
			fscanf(in_file2, "%f", &temp);
			if(temp < kv[i].value) kv[i].value = temp;
			if(temp < 0) kv[i].value = 1;
		}
		sort(kv, kv+K);
		
	    uint j;
		//uint min = kv[K-1].value;
		float min = 100000;
		//printf("min: %u\n", min);
		string ans;
		//topk->sketch->LoadPara(in_path_2);
		/*
		for(int i = 0; i < k; i++)
		{
			float temp;
			fscanf(in_file2, "%f", &temp);
			min = (min > temp) ? temp : min;
		}
		*/
		//printf("min : %f\n", min);
		fclose(in_file2);

		in_file2 = fopen(in_path_4, "r");
		uint count = 0;
		uint wrong = 0;
		uint right = 0;
		uint inlet = 0;
		uint accuracy = 0;
		string* str3 = Topk_r(k, topk);
		for(j = 0; j < k; j++){
			//str[j] = kv[j].key;
			//printf("%u\n", kv[j].value);
		}
		/*
		for(j = 0; j < K; j++)
		{
			float temp;
			fscanf(in_file2, "%f", &temp);
			if(Find_in_str(kv[j].key, str3, k))
			{
				printf("1 ");
			}
			else
			{
				printf("0 ");
			}
			printf("%f %u ", temp, record->find(kv[j].key)->second);
			topk->sketch->PrintCounter((cuc*)(kv[j].key).c_str());
		}
		*/
		//freopen("./MLData/kick.txt", "w", stdout);
		//perror("debug");
		//std::vector<KV> restore;
		/*
		for(j = 0; j < k; j++)
		{
			if(count >= K) break;
			float r;
			fscanf(in_file2, "%f", &r);
			//if(r > kv[count].value) r = kv[count].value;
			while(r > 0.5) {
				if(Find_in_str(kv[count].key, str3, k)) {
					wrong++;
					itt = topk->heap->find(kv[count].key);
					printf("0 %u %u %u %u ", (itt->second).oldValue, (itt->second).newValue, (itt->second).newValue - (itt->second).oldValue, record->find(kv[count].key)->second);
					//perror("wrong");
					topk->sketch->PrintCounter((cuc*)(kv[count].key).c_str());
				}
				else {
					right++;
					itt = topk->heap->find(kv[count].key);
					printf("1 %u %u %u %u ", (itt->second).oldValue, (itt->second).newValue, (itt->second).newValue - (itt->second).oldValue, record->find(kv[count].key)->second);
					//perror("right");
					topk->sketch->PrintCounter((cuc*)(kv[count].key).c_str());
				}
				//KV temp = kv[count];
				//temp.value = r;
				//restore.push_back(temp);
				count ++;
				if(count >= K) break;
				fscanf(in_file2, "%f", &r);
			}
			if(count >= K) break;
			str[j] = kv[count].key; //inlet ++;
			uint v = record->find(kv[count].key)->second;
			if(Find_in_str(kv[count].key, str3, k)) {
				itt = topk->heap->find(kv[count].key);
				printf("2 %u %u %u %u ", (itt->second).oldValue, (itt->second).newValue, (itt->second).newValue - (itt->second).oldValue, record->find(kv[count].key)->second);
				topk->sketch->PrintCounter((cuc*)(kv[count].key).c_str());
			} else {
				itt = topk->heap->find(kv[count].key);
				printf("3 %u %u %u %u ", (itt->second).oldValue, (itt->second).newValue, (itt->second).newValue - (itt->second).oldValue, record->find(kv[count].key)->second);
				topk->sketch->PrintCounter((cuc*)(kv[count].key).c_str());
			}
			if((float)kv[count].value / v <= 2) accuracy++;
			count ++;
			//perror("k");
		}
		*/
		//uint size = restore.size() * 2;
		/*
		for(j = 0.95*k; j < K; j++)
		{
			float r;
			fscanf(in_file2, "%f", &r);
			KV temp = kv[j];
			temp.value = r;
			restore.push_back(temp);
		}
		sort(restore.begin(), restore.end());

		count = 0;
		for(j = inlet; j < k; j++)
		{
			str[j] = restore[count].key;
			count ++;
		}
		*/
		//printf("get answer: %d\n", j);
		//printf("wrong kick: %u\n", wrong);
		//printf("right kick: %u\n", right);
		//printf("accuracy: %u\n", accuracy);
		//perror("before");
		fclose(in_file2);
		//perror("close");
		/*std::vector<pair<std::string, uint>>v;
		//std::vector<pair<std::string,uint>>::iterator it;
		for(itr = topk->heap->begin(); itr!= topk->heap->end(); itr++)
		{
			v.push_back(std::make_pair(itr->first, itr->second));
		}
		sort(v.begin(), v.end(),mysort);
		uint i=0;
		uint j=0;
		topk->sketch->LoadPara(in_path_2);
		while((i < k))
		{
			if(!topk->sketch->Predict((const unsigned char*)v[j].first.c_str()))
			{
			str[i] = v[i].first;
			i++;
			}
			j++;
		}*/
	}
	else
	{
		//for(itr = topk->heap->begin(); itr!= topk->heap->end();itr++)
		//	;//cout<<itr->first<<" "<<itr->second<<endl;
		//cout<<topk->sketch->Query((cuc*)("6432672575375"))<<endl;
		itt = topk->heap->begin();
		for(uint i = 0 ;i < k; i++)
		{
			//str[i] = itt->first;
			kv[i].key = itt->first;
			kv[i].value = itt->second;
			itt ++;
		}
	}
	
	if(an == 0)
	{
		string* str2 = Topk_r(k, topk);
		//for(uint i = 0;i < k; i++)
		//{
		//	;//cout<<str[i]<<' '<<str2[i]<<endl;
		//}
		return 0;
		//return Compare(str, str2, k, topk);
	}
	else
	{
		float sum = 0;
		int kick = 0;
		for(uint i = 0; i < k; i++)
		{
			uint a1, a2, o, n;
			//itt = topk->heap->find(str[i]);
			itt = topk->heap->find(kv[i].key);
			//a1 = itt->second;
			a1 = kv[i].value;
			o = (itt->second).oldValue;
			n = (itt->second).newValue;
			if(l && n - o < 8) {
				kick ++;
				//itr = record->find(kv[i].key);
				//a2 = itr->second;
				//sum += abs((int)a1 - (int)a2) / (float)a2;
				continue;
			}
			//itr = record->find(str[i]);
			itr = record->find(kv[i].key);
			a2 = itr->second;
			//sum += float(a1-a2)/a2;
			//printf("%u %u %u\n", a1, a2, abs((int)a1 - (int)a2));
			sum += abs((int)a1 - (int)a2);
		}
		//printf("kick: %u\n", kick);
		return kick;
		return (float)sum / (k - kick);

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
	//GetTopK(in_path, 1000);
	//system("python TopK.py MLData/example.train Para/example.para");
	//freopen(out_path2,"w", stdout);
	//float a = Find_topk(100, NOR_WID, 0, DEP, false, 0);
	//printf("%f\n", a);
	//Find_topk(1000, NOR_WID, 0, DEP);
	in_path_3 = argv[1];
	float a = Find_res(1000, NOR_WID, 0, DEP, true, 1);
	printf("%f\n", a);
	//a = Find_res(1000, NOR_WID, 0, DEP, false, 1);
	//printf("%f\n", a);
	return 0;
}