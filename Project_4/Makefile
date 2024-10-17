all: zap unzap

zap: zap.cc pqueue.h bstream.h huffman.h
	g++ -g -Wall -Werror -o $@ $< -std=c++11

unzap: unzap.cc pqueue.h bstream.h huffman.h
	g++ -Wall -Werror -o $@ $< -std=c++11

clean:
	-rm -f zap unzap
