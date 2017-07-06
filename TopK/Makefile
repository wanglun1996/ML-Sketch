CC = g++
OPTION = -std=c++11

main : main.o TopK.o Sketch.o CMSACSketch.o CMSketch.o CUSACSketch.o CUSketch.o PSACSketch.o PSketch.o common.o
	$(CC) -o main main.o TopK.o Sketch.o CMSACSketch.o CMSketch.o CUSACSketch.o CUSketch.o PSACSketch.o PSketch.o common.o $(OPTION)

train : train.o TopK.o Sketch.o CMSACSketch.o CMSketch.o CUSACSketch.o CUSketch.o PSACSketch.o PSketch.o common.o
	$(CC) -o train train.o TopK.o Sketch.o CMSACSketch.o CMSketch.o CUSACSketch.o CUSketch.o PSACSketch.o PSketch.o common.o

res : res.o TopK.o Sketch.o CMSACSketch.o CMSketch.o CUSACSketch.o CUSketch.o PSACSketch.o PSketch.o common.o
	$(CC) -o res res.o TopK.o Sketch.o CMSACSketch.o CMSketch.o CUSACSketch.o CUSketch.o PSACSketch.o PSketch.o common.o 

train.o : train.cpp TopK.h
	$(CC) -c train.cpp $(OPTION)

res.o : res.cpp TopK.h
	$(CC) -c res.cpp $(OPTION)

main.o : main.cpp TopK.h
	$(CC) -c main.cpp $(OPTION)

TopK.o : TopK.cpp TopK.h Sketch.h
	$(CC) -c TopK.cpp $(OPTION)

Sketch.o : Sketch.cpp Sketch.h CMSACSketch.h CMSketch.h CUSACSketch.h CUSketch.h PSACSketch.h PSketch.h
	$(CC) -c Sketch.cpp $(OPTION)

CMSACSketch.o : CMSACSketch.cpp CMSACSketch.h common.h
	$(CC) -c CMSACSketch.cpp $(OPTION)

CMSketch.o : CMSketch.cpp CMSketch.h common.h
	$(CC) -c CMSketch.cpp $(OPTION)

CUSACSketch.o : CUSACSketch.cpp CUSACSketch.h common.h
	$(CC) -c CUSACSketch.cpp $(OPTION)

CUSketch.o : CUSketch.cpp CUSketch.h common.h
	$(CC) -c CUSketch.cpp $(OPTION)

PSACSketch.o : PSACSketch.cpp PSACSketch.h common.h
	$(CC) -c PSACSketch.cpp $(OPTION)

PSketch.o : PSketch.cpp PSketch.h common.h
	$(CC) -c PSketch.cpp $(OPTION)

common.o : common.cpp common.h
	$(CC) -c common.cpp $(OPTION)

predict : 
	python TopK.py MLData/example.train MLData/example.result "MLData/example.test"

cld : 
	rm ./MLData/example.train

clean : 
	rm *.o main train res