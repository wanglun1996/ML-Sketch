#include "Sketch.h"

int main(){
	PSketch *my_p = (PSketch*)CreateSketch(2, 4, 4);
	for(int i = 1; i <= 500; ++i){
		Insert((void*)my_p, 2, (cuc*)"ABCDEF");
		uint v = Query((void*)my_p, 2, (cuc*)"ABCDEF");
		printf("%d %d\n", i, v);
	}
	return 0;
}