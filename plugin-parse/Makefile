
SRC_DIR = ./src
TR_DIR = ./traces

CC=g++
CPPFLAGS=-Wall -Wextra -std=c++0x

test: parseTasks.o parseDAG.o test.o
	$(CC) $^ -g -o $@ -lboost_graph

parse: parseTasks.o parseDAG.o parser.o
	$(CC) $^ -g -o $@ -lboost_graph

parser.o: ${SRC_DIR}/parser.cpp
	g++ -Wall -g -c -o parser.o ${SRC_DIR}/parser.cpp

parseDAG.o: ${SRC_DIR}/parseDAG.cpp
	g++ -Wall -g -c -o parseDAG.o ${SRC_DIR}/parseDAG.cpp

parseTasks.o: ${SRC_DIR}/parseTasks.cpp
	g++ -Wall -g -c -o parseTasks.o ${SRC_DIR}/parseTasks.cpp

test.o: ${SRC_DIR}/test.cpp
	g++ -Wall -g -c -o test.o ${SRC_DIR}/test.cpp

execute: 
	./parse ${TR_DIR}/dag.dot ${TR_DIR}/tasks.rec 

execute1: 
	./parse ${TR_DIR}/dag1.dot ${TR_DIR}/tasks1.rec 

draw_original:
	dot -Tpng ${TR_DIR}/dag.dot > original_DAG.png

draw_new:
	dot -Tpng ${TR_DIR}/CriticalPath_DAG.dot > criticalPath_DAG.png

clean: 
	rm -f *.o parse test ${TR_DIR}/CriticalPath_DAG.dot *.png