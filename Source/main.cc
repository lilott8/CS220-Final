
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

Controller::AlgoType resolve_algorithm(string);
Controller::Optimization resolve_optimization(string);

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

	Controller::AlgoType a_type = Controller::AlgoType::FORTUNE;
	Controller::Optimization  o_type = Controller::Optimization::DEFAULT;
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

	//controller->print_map();
	controller->start();

	delete controller;
	delete first_problem;

	return 0;
}

Controller::AlgoType resolve_algorithm(string s) {
	Controller::AlgoType result = Controller::AlgoType::FORTUNE;
	if(s.compare("fortune") == 0) {
		result = Controller::AlgoType::FORTUNE;
	} else if(s.compare("spm") == 0) {
		result = Controller::AlgoType::SPM;
	}
	return result;
}

Controller::Optimization resolve_optimization(string s) {
	Controller::Optimization result = Controller::Optimization::DEFAULT;
	if(s.compare("h") == 0) {
		result = Controller::Optimization::H_OPT;
	} else if(s.compare("u") == 0) {
		result = Controller::Optimization::U_OPT;
	}
	return result;
}
