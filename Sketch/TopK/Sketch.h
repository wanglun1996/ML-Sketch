#ifndef SKETCH_H
#define SKETCH_H

#include "CMSketch.h"
#include "CUSketch.h"
#include "PSketch.h"
#include "CMSACSketch.h"
#include "CUSACSketch.h"
#include "PSACSketch.h"

void* CreateSketch(uint type, uint d, uint w);
void Insert(void *ptr, uint type, cuc* str);
uint Query(void *ptr, uint type, cuc* str);

#endif //SKETCH_H