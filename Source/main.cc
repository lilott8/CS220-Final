
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

		//cout << "<Kruskal's Optimization> d | h | u" << endl;
		exit(1);
	}

	// Set the optimization we will be using
	Controller::Optimization  o_type = Controller::Optimization::DEFAULT;
	// determine which steiner calculation method we will use
	int steiner_calculation = 2;
	// default location of the problem object
	string file = "../Tests/debug_small.json";

	// Parse the args
	switch(argc) {
		case 4:
			o_type = resolve_optimization(argv[3]);
		case 3:
			steiner_calculation = atoi(argv[2]);
		default:
		case 2:
			file = argv[1];
	}

	// Create the problem object
	Utilities::ProblemObject* first_problem = new Utilities::ProblemObject(file);
	// Create the controller
	Controller* controller = new Controller(first_problem, o_type);
	// Set the steiner point generation
	controller->set_steiner_calculator(steiner_calculation);

	// Set the id of the last vnode
	VNode::kLastId = 0;

	// Begin execution of the program!
	controller->start();

	// Delete all the things!
	delete controller;
	delete first_problem;

	return 0;
}

/**
 * Determine the type of Kruskal optimization you want to use
 * Deprecated but still here due to project scope alterations
 */
Controller::Optimization resolve_optimization(string s) {
	Controller::Optimization result = Controller::Optimization::DEFAULT;
	if(s.compare("h") == 0) {
		result = Controller::Optimization::H_OPT;
	} else if(s.compare("u") == 0) {
		result = Controller::Optimization::U_OPT;
	}
	return result;
}
