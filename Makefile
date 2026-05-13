
a.out: main.c.o pairwise-sum.c.o direct.cc.o pairwise-sum.cc.o
	g++ main.o pairwise-sum.c.o pairwise-sum.cc.o

pairwise-sum.c.o: src/pairwise-sum.c
	g++ -x c -Iinclude src/pairwise-sum.c -c -o $@

pairwise-sum.cc.o: src/pairwise-sum.cc
	g++ -Iinclude src/pairwise-sum.cc -c -o $@

direct.cc.o: src/direct.cc
	g++ -Iinclude src/direct.cc -c -o $@

main.c.o: main.c
	g++ -Iinclude main.c -c -o $@

clean:
	rm -rf main.o pairwise-sum.o a.out
