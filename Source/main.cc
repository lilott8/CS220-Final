
#include "../Headers/grid.h"
#include "../Headers/problem_object.h"
#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/flow_algo.h"
#include "../Headers/claim.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

using namespace Flow;
using namespace Utilities;

FlowAlgorithm::AlgoType resolve_algorithm(string);
FlowAlgorithm::Optimization resolve_optimization(string);

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

	FlowAlgorithm::AlgoType a_type = FlowAlgorithm::AlgoType::FORTUNE;
	FlowAlgorithm::Optimization  o_type = FlowAlgorithm::Optimization::DEFAULT;
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

	controller->print_map();
	controller->start();

	delete controller;
	delete first_problem;

	return 0;
}

FlowAlgorithm::AlgoType resolve_algorithm(string s) {
	FlowAlgorithm::AlgoType result = FlowAlgorithm::AlgoType::FORTUNE;
	if(s.compare("fortune") == 0) {
		result = FlowAlgorithm::AlgoType::FORTUNE;
	} else if(s.compare("spm") == 0) {
		result = FlowAlgorithm::AlgoType::SPM;
	}
	return result;
}

FlowAlgorithm::Optimization resolve_optimization(string s) {
	FlowAlgorithm::Optimization result = FlowAlgorithm::Optimization::DEFAULT;
	if(s.compare("h") == 0) {
		result = FlowAlgorithm::Optimization::H_OPT;
	} else if(s.compare("u") == 0) {
		result = FlowAlgorithm::Optimization::U_OPT;
	}
	return result;
}
