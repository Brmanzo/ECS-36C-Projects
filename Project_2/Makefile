all: luggage_handling test_deque

luggage_handling:  luggage_handling.cc queue.h stack.h
	g++	-std=c++11	-Wall	-Werror	-o	luggage_handling	luggage_handling.cc

test_deque: test_deque.cc deque.h
	g++	-std=c++11	-Wall	-Werror	-o	test_deque test_deque.cc -pthread -lgtest
 
clean:
	rm	-f	*.o	luggage_handling	test_deque

lint: luggage_handling.cc
	/home/cs36cjp/public/cpplint/cpplint	luggage_handling.cc	deque.h

