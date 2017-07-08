#ifndef TOPK_H
#define TOPK_H

#include "Sketch.h"

// Initialize a sketch
// 0--CM
// 1--CU
// 2--P
// 3--CMSAC
// 4--CUSAC
// 5--PSAC

struct KV {
	std::string key;
	uint value;
	friend bool operator < (const KV &a, const KV &b) {
		return a.value > b.value;
	}
};

struct NO {
	uint oldValue;
	uint newValue;
	NO(uint o, uint n)
	{
		oldValue = o;
		newValue = n;
	}
	NO(uint n)
	{
		oldValue = n;
		newValue = n;
	}
	friend uint operator ++(NO &no)
	{
		no.newValue ++;
	}
	friend uint operator ++(NO &no, int a)
	{
		no.newValue ++;
	}
	friend bool operator <(NO &no, uint num)
	{
		return no.newValue < num;
	}
	operator uint()
	{
		return newValue;
	}
};

struct TopK{
public:
	TopK(uint type, uint d, uint w, uint k);
	~TopK();

	void Insert(cuc* str);
	void PrintTopK();
	CMSketch* sketch;
	
	uint k;
	//std::unordered_map<std::string, uint> *heap;
	//std::unordered_map<std::string, uint>::iterator MP;
	std::unordered_map<std::string, NO> *heap;
	std::unordered_map<std::string, NO>::iterator MP;
	uint min;
	//std::priority_queue<KV> *heap;
};


#endif //TOPK_H