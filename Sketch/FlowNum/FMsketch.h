#ifndef _FMSKETCH_H
#define _FMSKETCH_H
#include "BOBHash.h"
#include "params.h"
#include <cstring>
#include <math.h>
#include <stdlib.h>
class FMsketch
{
private:
	int d;
	int w;
	int * counter;
	BOBHash * bobhash[MAX_PRIME];
public:
	FMsketch(int _d, int _w)
	{
		d = _d;
		w = _w;
		counter = new int[d];
		memset(counter, 0, d*sizeof(int));

		for(int i = 0; i < d; i++)
		{
			bobhash[i] = new BOBHash(i);
		}
	}
	//return how many 0 bit in the lower hashvalue;
	int MyHash(int identifier, char *str)
	{
		uint64 originHashValue = bobhash[identifier]->run(str, strlen(str));
		for(int i = 0; i < w - 1; i++)
		{
			if(((originHashValue >> i) & 1) == 1)
			{
				return i;
			}
		}
		return w - 1;
	}
	int DirectHash(int x){
		for(int i = 0; i < w - 1; i++){
			if(((x >> i) & 1) == 1){
				return i;
			}
		}
		return w - 1;		
	}
	void DirectInsert(int x){
		int hashNum = DirectHash(x);
		counter[0] |= (1<<hashNum);
	}
	void RandomInsert(char *str){
		int dice = rand()%COUNTER_NUM;
		_insert(dice, str);
	}
	void Insert(char *str){
        for(int j = 0; j < COUNTER_NUM; ++j){
            _insert(j, str);
        }
	}
	void _insert(int identifier, char *str)
	{
		int hashNum = MyHash(identifier, str);
		counter[identifier] |= (1 << hashNum);
	}
	int StatisticsSetCard()
	{
		double avelow = 0, avehigh = 0;
		for(int x = 0; x < d; x++)
		{
			int i, low, high;
			for(i = 0; i < w; i++)
			{
				if(((counter[x] >> i) & 1) == 0)
				{
					low = i;
					break;
				}
			}
			for(i = w - 1; i >= 0; i--)
			{
				if(((counter[x] >> i) & 1) == 1)
				{
					high = i;
					break;
				}
			}
			
			// low, high

			// printf("low = %d, high = %d\n", low, high);
			avelow += low;
			avehigh += high;
		}
		// return (int)pow(2.0, avehigh / d);
		//printf("avelow: %lf\n", avelow);
		return (int)(pow(2.0, (avelow / d) / 1) * 1.2928);
	}
	int QueryL(int idx){
		int i, low, high;
		for(i = 0; i < w; i++)
		{
			if(((counter[idx] >> i) & 1) == 0)
			{
				low = i;
				break;
			}
		}
		/*for(i = w - 1; i >= 0; i--)
		{
			if(((counter[idx] >> i) & 1) == 1)
			{
				high = i;
				break;
			}
		}*/
		return low;
	}
	int QueryH(int idx){
		int i, low, high = 0, shigh = 0;
		bool flag = 0;
		/*for(i = 0; i < w; i++)
		{
			if(((counter[idx] >> i) & 1) == 0)
			{
				low = i;
				break;
			}
		}*/
		for(i = w - 1; i >= 0; i--)
		{
			if(!flag){
				if(((counter[idx] >> i) & 1) == 1)
				{
					high = i;
					flag = 1;
				}
			}
			else{
				if(((counter[idx] >> i) & 1) == 1)
				{
					shigh = i;
					break;
				}	
			}
		}
		if(shigh < high-1) return shigh;
		return high;
	}
	void GetCounter(int* t){
		for(int i = 0; i < COUNTER_NUM; ++i){
			t[2*i] = QueryL(i);
			t[2*i+1] = QueryH(i);
		}
	}
	void PrintCounter(){
		for(int i = 0; i < COUNTER_NUM; ++i){
			printf(" %u %u", QueryL(i), QueryH(i));
		}
		printf("\n");
	}
	void Clear(){
		memset(counter, 0, d*sizeof(int));
	}

};

#endif