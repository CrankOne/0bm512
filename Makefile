
CCFLAGS+=-Wall -Iinclude
CFLAGS+=-Wall -Iinclude

# First rule (build by default)
all: tests mc-app

# Auxiliary application for various tests
tests: main.tests.c.o lib0bm512.a
	g++ $^ -o $@

# Special rule for Geant4 app
mc-app: main-g4.cc.o lib0bm512.a
	g++ $^ $(shell geant4-config --libs) -o $@

# Rule to produce .o files from .c (pure C) files in src/
build/%.c.o: src/%.c
	g++ -c -x c $(CFLAGS) $^ -o $@

# Rule to produce .o files from .cc (C++) files in src/
build/%.cc.o: src/%.cc
	g++ -c $(CCFLAGS) $^ -o $@

# Rule to produce .o files from g4/*.cc (C++) files in src/
build/g4-%.o: src/g4/%.cc
	g++ -c $(shell geant4-config --cflags) $(CFLAGS) $< -o $@

# Static library target:
lib0bm512.a: build/pairwise-sum.c.o \
			 build/pairwise-sum.cc.o \
			 build/direct.cc.o \
			 build/histogram.c.o \
			 build/g4-ActionInitialization.o \
			 build/g4-DetectorConstruction.o \
			 build/g4-PrimaryGeneratorAction.o \
			 build/g4-SensitiveDetector.o
	ar -crs $@ $^

# testing fixture entry point (not MC, plain C)
main.tests.c.o: main.tests.c
	g++ -Iinclude main.tests.c -c -o $@

# Geant4 app entry point
main-g4.cc.o: main-g4.cc
	g++ -c $(shell geant4-config --cflags) $(CFLAGS) $< -o $@

clean:
	@rm -vrf build/*.o
	@rm -vf tests

.PHONY: all clean
