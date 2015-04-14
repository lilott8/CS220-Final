CS220 Project Framework
===============

### Project Usage

`./final {location to problem object JSON file} {SPC thoroughness 1-4}`

`./final ../input/input.json 1`

### Project Summary

This project uses Steiner points to successfully connect all source/sink pairs using the minimum amount of channeling possible.  To do this the project utilizes several routines taken from different research efforts.  To read those papers please refer to the project specs.

This implementation works in several different phases.  Each phase is discrete and can be skipped or altered as they are completely self contained and independent of each other.  The general phases are as follows: 

 1. [Generate Steiner points from a Voronoi calculation](#step-one)
 2. [Generate a Hanan grid using other, calculated Steiner points (SPC phase)](#step-two)
 3. [Combine all vertice pairs into edges](#step-three)
 4. [Use Dijstra to calculate the shortest path through the graph](#step-four)
 5. [Use Kruskal to calculate the minimum connections needed to connect the graph](#step-five)

#### Step One:<a name="step-one"></a>
 
 Relying on Boost's Voronoi library, I generate a Voronoi graph and properly route around obstacles.  From there I pair Voronoi vertices to their closest sink/source point.
 
 **Input:** Set of sink/sources
 
 **Output:** Set of edges or vertices, The edge/vertice retrieval is a simple method call to the Voronoi class: `get_edges` or `get_vertices`
 
#### Step Two:<a name="step-two"></a>

 From here, the remainder of the SPC points are generated to help guarantee the shortest point path is found using Dijkstra's/Kruskal's algorithms.  The thoroughness is defined as follows:
 
 1. Naive, Midpoint approach.  This level will combine each element with only its successive element and use a manhattan midpoint calculation to determine Steiner point placement
    * Suppose elements 1,2,3,4 this SPC level will only calculate Steiner points for: 1-2, 2-3, 3-4
 2. Exhaustive, Midpoint approach.  This level will combine every element with every other element and use a manhattan midpoint calculation to determine Steiner point placement
 3. Naive, Geometric approach.  This level using the same combinatorial logic as step one, but instead of the manhattan midpoint it uses a complicated triangulation method to determine Steiner point placement\*
 4. Exhaustive, Geometric approach.  This level uses the combinatorial logic as step 2, but uses the same triangulation method as step 3 for Steiner point placement\*
 
 \* *Please note:* that these steps are extremely computationally expensive and can have inaccuracies caused by rounding. The geometric calculations are float based, while the problem space is defined in integers
 
 **Input:** Set of Vertices that need Steiner points generated
 
 **Output:** Set of Steiner points
 
##### Step Three:<a href="step-three"></a>

 Because the input for steps 4 and 5 are edges, we must take all the vertex pairs and transform them to edges but we must create a valid path between point a and point b.  To do so, we route each pair through Hadlock's algorithm to ensure a valid path exists and we then output that valid path as a series of edges.
 
 **Input:** Set of Source/Target nodes that need to be routed
 
 **Output:** Set of edges that route the Source/Target pairs
 
#### Step Four:<a href="step-four"></a>
 
 I leverage Boost's implementation of Dijkstra's algorithm.  This will output a .dot notated text to the std out.
 
 **Input:** Set of edges that comprise the graph
 
 **Output:** Set of edges that form the shortest path through the graph _and_ .dot notated text to std out
 
#### Step Five:<a href="step-five"></a>

 I leverage Boost's implementation of Kruskal's algorithm.  This will output a .dot notated text to the std out.
 
  **Input:** List of edges from [Step Four](#step-four)
  
  **Output:** .dot notated text to std out
 
---

### Project Output

 The output of this project are 2 dot compatible elements that are output through std out.  The first one is the state of the solution when Dijkstra's algorithm is applied.  The second is the proposed solution after Kruskal's algorithm is applied
 
### Project Problems

 As of writing there is only one known "bug".  Because of the way the Voronoi algorithm works, it is not easy to discern which 2 points the vertex resides between.  I properly identify one of the points but cannot adequately discern the second one.  The implications this has on the answer is that there are points in the Kruskal output that can end in Steiner points and not sink/source elements.  This can be easily solved in the output stage of Kruskal's algorithm.
 
 This isn't techincally a problem, but during some of the transformations, mainly for the Dijkstra and Kruskal usages, there is quite a bit of wasted computation.  This is a result of the esoteric, inconsistent implementation of the boost graph library.  I'm sure there are more efficient ways to do this, I'm just not smart enough to understand the boost documentation.

---
 
## Project Overview

You will use this framework for the following projects:

- Grid Routing
- Advanced Grid Routing
- Soukup Global Grid Routing
- Line Routing

The project will involve reading in JSON test files (located in the test directory) and parsing these files into a Problem Object. This step has already been done for you and is written at the beginning of main.cc to allow for command line arguments, **You should not change this code** as it will be used for testing. Please note that the ProblemObject gives you the width and height of the chip, which will be indexed from 0 to n-1.

You will then take this Problem Object, and pass it into your custom problem map object. The custom problem map object is the object you use to reprsent the problem space. In the example we give that object is the Grid object, however **you should not use the grid object** as it is not adequate for this project. You may, however, want to take advantage of the other classes we give you, such as the Node and Edge classes. While it is not required that you use any classes we give you (except for the ProblemObject and Path/Netlist) it is highly encouraged. You will also find that many of the classes we provide are too general for your use, in this case we advise you to extend the base class with a child class designed to fit your needs.

Once you have created your custom problem map object, you should make a call to a routing algorithm (the specific algorithm will depend on which submission you are working on). You should strive to be able to use the same custom problem object for mulitple routers, however it is not required that all routes work with the same custom problem object. Your routing algorithm **must return either a Path/vector of Paths or Netlist/vector of Netlists** depending on what the situation calls for.

- Path: A path is a set of straight line segments from one source to one sink
- Netlist: A netlist is a set of straight line segments from one souce to multiple sinks

Once you have a solution in the form of a Path/vector of Paths or Netlist/vector of Netlists, you should print your solution to std out for review.

Currently there is only one test case given, and it is not a good test case. It is given as an example so you can create your own test cases. **You should be creating robust test cases** that are capable of testing malformed input files, edge cases, and problems that are impossible to solve. With this **your program should error gracefully**, meaning if there is a bad input file or some type of error the program should not simply crash. It should give an explination as to the possible error and exit. We have given you the claim system for printing errors, please use it.

**Please Note:** If you have one of the projects listed at the top, then your project **MUST** exist within this framework.

## Running the Framework

In order to run the framework, you must first go to Utilities/JSON_parser and run "make". This will create a json_parse.so file that is need to parse the JSON test files that we give you. Next you will go back to the root directory and run "make". This will generate an executable called "grid_router", which you can run by calling "./grid_router <test_case>", where <test_case> is the path of a test case. In the framework we give you there is only one test case, located at "Tests/test_sample.json". This will run the example that we have written for the framework.

**Please Note:** some of you may get a library error when trying to run grid_router. If you recieve this library error, please copy the file "json_parser.so" from Utilities/JSON_parser to the root directory where the file "grid_router" exists and this should fix the problem. Note that you must have already called "make" in Utilities/JSON_parser for "json_parse.so" to exist.

## Bugs

As I have stated before, we expect there to be bugs in this framework. When you find a bug please [write a good bug report](http://www.chiark.greenend.org.uk/~sgtatham/bugs.html) and submit it to the GitHub bug page. Or better yet create a fix to the problem and [submit a pull request](https://help.github.com/articles/using-pull-requests/). You should also be **checking the bug tracking page** of the repository for known problems before emailing me or submitting a new bug report.

## Your Repository

For your submission, you must have a repository updated with the code you wish to submit by 8:00AM PST on the due date. Your repository must also adhere to the following rules.

- Your repository must be private
- I must be [added as a collaborator on your repository](https://help.github.com/articles/adding-collaborators-to-a-personal-repository/) (username: brrcrites)
- Your README.md must at minimum have the name(s) of your group members (and hopefully more useful information)

In addition to this, you **must email me with a link to your repsitory**. There are currently many repositories existing, and I have no idea who most of them belong to. **In order to have a private repository you must have a student developer pack**. You [get the student developer pack here](https://education.github.com/pack), and it comes with alot of other cool stuff. If you do not have the pack, and therefore cannot make your repository private, before the deadline for your proejct, do not worry. Continue to update your repository and make it private as soon as you can.

