#Build id3_tree
INCLUDE = include/
CPP = g++
CCFLAGS = -std=c++11 -I$(INCLUDE)
SRCDIR = src/

SOURCE = $(SRCDIR)id3_tree.cpp $(SRCDIR)node.cpp $(SRCDIR)main.cpp

id3_tree:
	$(CPP) -o $@ $(CCFLAGS) $(SOURCE)

.PHONY: clean

clean:
	rm -f *.o
