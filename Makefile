CC=g++
FLAGS=-lgdi32 -mconsole 
CFLAGS=-fpermissive 

all: mkdirs generate predict train

train: obj/train.o obj/genann.o obj/dist.o
	$(CC) -o bin/$@ $^ $(FLAGS)

predict: obj/predict.o obj/genann.o obj/dist.o
	$(CC) -o bin/$@ $^ $(FLAGS)

generate: obj/gen.o obj/genann.o obj/dist.o
	$(CC) -o bin/$@ $^ $(FLAGS)

obj/train.o: src/train.cpp
	$(CC) -c $^ -o $@

obj/predict.o: src/predict.cpp
	$(CC) -c $^ -o $@

obj/gen.o: src/gen.cpp
	$(CC) -c $^ -o $@

obj/genann.o: src/genann.c
	-$(CC) -c $^ $(CFLAGS) -w -o $@

obj/dist.o: src/dist.cpp
	$(CC) -c $^ -o $@

mkdirs:
	@-mkdir obj bin


clean:
	rm -f obj/*.o bin/*.exe