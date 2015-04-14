
#include "../Headers/problem_object.h"
#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/vnode.h"
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
		cout << "Usage: ./flow <test_file> <spg>" << endl;
		cout << "Optional Arguments:" << endl;
		cout << "<test_file> {string}" << endl;
		cout << "<spg> | 1 | 2 | 3 | 4 (1 is the least computation intensive "
				<< "implementation and 4 is the most.) Default is 2" << endl;
		cout << "They differ in exhaustiveness and calculations.  "
				<< "1 | 2 are a O(n) | O(n^2) implementations that" << endl;
		cout << "utilize midpoint calculations to derive a steiner point.  "
				<< "While 3 | 4 are O(n) | O(n^2) implementations that" << endl;
		cout << "utilize triangulation to calculate the steiner point." << endl;

		//cout << "<Algorithm> fortune | spm" << endl;
		//cout << "<Kruskal's Optimization> d | h | u" << endl;
		exit(1);
	}

	//Controller* controller = new Controller(first_problem);

	Controller::AlgoType a_type = Controller::AlgoType::FORTUNE;
	Controller::Optimization  o_type = Controller::Optimization::DEFAULT;
	int steiner_calculation = 2;
	string file = "../Tests/debug_small.json";

	switch(argc) {
		case 4:
			o_type = resolve_optimization(argv[3]);
		case 3:
			steiner_calculation = atoi(argv[2]);
		default:
		case 2:
			file = argv[1];
	}

	Utilities::ProblemObject* first_problem = new Utilities::ProblemObject(file);
	Controller* controller = new Controller(first_problem, a_type, o_type);
	controller->set_steiner_calculator(steiner_calculation);

	VNode::kLastId = 0;

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
