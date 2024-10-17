all:	test_treemap	anitaborg_donations

test_treemap:	test_treemap.cc	treemap.h
	g++	-std=c++11	-Wall	-Werror	-o	test_treemap	test_treemap.cc	-pthread	-lgtest

anitaborg_donations: anitaborg_donations.cc treemap.h
	g++	-std=c++11	-Wall	-Werror	-o	anitaborg_donations	anitaborg_donations.cc
 
clean:
	rm	-f	*.o	test_treemap	anitaborg_donations

lint:
	/home/cs36cjp/public/cpplint/cpplint	test_treemap.cc	treemap.h	anitaborg_donations.cc

