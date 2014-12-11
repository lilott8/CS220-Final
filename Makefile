
SRC=$(filter-out %main.cc, Source/*)
OBJ=claim.o edge.o grid.o netlist.o node.o path.o pathsegment.o problem_object.o segmentgroup.o 

vpath %.cc Source/

all: $(OBJ) main.cc
	g++ -o flow $^ Utilities/JSON_parser/json_parser.so

test: all
	./flow Tests/test_sample.json
	
%.o: %.cc
	g++ -c $^

cleanup:
	rm -f *.o

clean: cleanup
	rm -f flow