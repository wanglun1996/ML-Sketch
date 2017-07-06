#include "CMSketch.h"
#include "CUSketch.h"
#include "PSketch.h"
#include "CMSACSketch.h"
#include "CUSACSketch.h"
#include "PSACSketch.h"

// Initialize a sketch
// 0--CM
// 1--CU
// 2--P
// 3--CMSAC
// 4--CUSAC
// 5--PSAC
void* CreateSketch(uint type, uint d, uint w){
	void* ptr = NULL;
	switch(type){
		case 0: {ptr = (void*)new CMSketch(d, w); break;} 
		case 1: {ptr = (void*)new CUSketch(d, w); break;}
		case 2: {ptr = (void*)new PSketch(d, w); break;}
		case 3: {ptr = (void*)new CMSACSketch(d, w); break;}
		case 4: {ptr = (void*)new CUSACSketch(d, w); break;}
		case 5: {ptr = (void*)new PSACSketch(d, w); break;}
		default: {printf("Wrong Parameter!\n"); break;}
	}
	return ptr;
}

//Insert str to sketch
void Insert(void *ptr, uint type, cuc* str){
	switch(type){
		case 0: {CMSketch *my_cm = (CMSketch*)ptr; my_cm->Insert(str); break; }
		case 1: {CUSketch *my_cu = (CUSketch*)ptr; my_cu->Insert(str); break;}
		case 2: {PSketch *my_p = (PSketch*)ptr; my_p->Insert(str); break;}
		case 3: {CMSACSketch *my_cmsac = (CMSACSketch*)ptr; my_cmsac->Insert(str); break;}
		case 4: {CUSACSketch *my_cusac = (CUSACSketch*)ptr; my_cusac->Insert(str); break;}
		case 5: {PSACSketch *my_psac = (PSACSketch*)ptr; my_psac->Insert(str); break;}
		default: {printf("Wrong Parameter!\n"); break;}
	}
}

//Query str in sketch
uint Query(void *ptr, uint type, cuc* str){
	uint res = 0;
	switch(type){
		case 0: {CMSketch *my_cm = (CMSketch*)ptr; res = my_cm->Query(str); break;}
		case 1: {CUSketch *my_cu = (CUSketch*)ptr; res = my_cu->Query(str); break;}
		case 2: {PSketch *my_p = (PSketch*)ptr; res = my_p->Query(str); break;}
		case 3: {CMSACSketch *my_cmsac = (CMSACSketch*)ptr; res = my_cmsac->Query(str); break;}
		case 4: {CUSACSketch *my_cusac = (CUSACSketch*)ptr; res = my_cusac->Query(str); break;}
		case 5: {PSACSketch *my_psac = (PSACSketch*)ptr; res = my_psac->Query(str); break;}
		default: {printf("Wrong Parameter!\n"); break;}
	}
	return res;
}