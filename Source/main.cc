
#include "../Headers/grid.h"
#include "../Headers/problem_object.h"
#include "../Headers/map.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

using namespace Flow;

int main(int argc,char* argv[]) {

	// DO NOT CHANGE THIS SECTION OF CODE
	if(argc < 2) { 
		cout << "Usage: ./grid_router <test_file>" << endl; 
		exit(1);
	}
	Utilities::ProblemObject* first_problem = new Utilities::ProblemObject(std::string(argv[1]));

	Map *map = new Map(first_problem);
	map->print_map();
	// EDIT FROM HERE DOWN

	delete first_problem;

	return 0;
}
