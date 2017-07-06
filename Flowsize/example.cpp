#include "Sketch.h"

char str[1024] = "haha";
int main(){
	//printf("%d\n", strcmp(str, (std::string(str)).c_str()));
	Sketch *my_cm = (Sketch*)CreateSketch(0, 4, 100000);
	for(int i = 1; i <= 500; ++i){
		my_cm->Insert((cuc*)"ABCDEF");
		uint v = my_cm->Query((cuc*)"ABCDEF");
		printf("%d %d\n", i, v);
		// Insert((void*)my_cm, 0, (cuc*)"ABCDEF");
		// uint v = Query((void*)my_cm, 0, (cuc*)"ABCDEF");
		// printf("%d %d\n", i, v);
	}
	return 0;
}