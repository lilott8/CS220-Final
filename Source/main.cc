
#include "../Headers/grid.h"
#include "../Headers/problem_object.h"
#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/algorithm.h"
#include "../Headers/claim.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

using namespace Flow;
using namespace Utilities;

Algorithm::AlgoType resolve_algorithm(string);
Algorithm::Optimization resolve_optimization(string);

int main(int argc,char* argv[]) {

	// DO NOT CHANGE THIS SECTION OF CODE
	if(argc < 2) { 
		cout << "Usage: ./flow <test_file> <Algorithm> <Kruskal's optimization>" << endl;
		cout << "Optional Arguments:" << endl;
		cout << "<test_file> {string}" << endl;
		cout << "<Algorithm> fortune | spm" << endl;
		cout << "<Kruskal's Optimization> d | h | u" << endl;
		exit(1);
	}

	//Controller* controller = new Controller(first_problem);

	Algorithm::AlgoType a_type = Algorithm::AlgoType::FORTUNE;
	Algorithm::Optimization  o_type = Algorithm::Optimization::DEFAULT;
	string file = "../Tests/debug_small.json";

	switch(argc) {
		case 4:
			o_type = resolve_optimization(argv[3]);
		case 3:
			a_type = resolve_algorithm(argv[2]);
		default:
		case 2:
			file = argv[1];
	}

	Utilities::ProblemObject* first_problem = new Utilities::ProblemObject(file);
	Controller* controller = new Controller(first_problem, a_type, o_type);

	controller->start();

	delete controller;
	delete first_problem;

	return 0;
}

Algorithm::AlgoType resolve_algorithm(string s) {
	Algorithm::AlgoType result = Algorithm::AlgoType::FORTUNE;
	if(s.compare("fortune") == 0) {
		result = Algorithm::AlgoType::FORTUNE;
	} else if(s.compare("spm") == 0) {
		result = Algorithm::AlgoType::SPM;
	}
	return result;
}

Algorithm::Optimization resolve_optimization(string s) {
	Algorithm::Optimization result = Algorithm::Optimization::DEFAULT;
	if(s.compare("h") == 0) {
		result = Algorithm::Optimization::H_OPT;
	} else if(s.compare("u") == 0) {
		result = Algorithm::Optimization::U_OPT;
	}
	return result;
}
