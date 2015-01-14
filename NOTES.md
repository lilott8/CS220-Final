##NOTES

Fortune's is completely implemented.  I need to take the output and include it to alter the graph properly.

Almost have the entire fortune algorithm implemented.  Something is going berserk with my VEvent.  It is considered ambiguous and thus being a jerk.

Retooled my binary tree.  It was too difficult to incorporate a static structure, so I created a more dynamic structure which is housed in the fortunes class, instead of siloed in it's own class.  I am beginning to convert some of the generic fortune's algorithm into this framework.  The functions are declared, but not implemented.

The controller will be how we abstract the logic from the main and thus the entire framework.

The flow of the program will then be: main->controller->map & algorithm

kPins = sites (sorted PQ given x,y coordinates closest to the origin)

##TODO

  - Use the output from the voronoi to draw on the graph
  - Either add all the appropriate .o files to the make file or convince Brian to use CMake!
  - figure out why the map coords need to be switched in init!
  - Implement the nlogn Voronoi grapher *(Resolved)*
  - Create a datastructure to hold a Voronoi sector *(Resolved)*
  - derive a good compare function for the beachline rb-tree *(Resolved)*
  - why can't vedge and arch have pointers to each other?? *(Resolved)*