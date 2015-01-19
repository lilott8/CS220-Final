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
  
##Compare Against:

GOT Line #0 (4.500000,0.000000)->(4.500000,0.000000)
GOT Line #1 (10.000000,4.500000)->(10.000000,4.500000)
GOT Line #2 (4.500000,10.000000)->(4.500000,10.000000)
GOT Line #3 (0.000000,4.500000)->(0.000000,4.500000)
GOT Line #4 (5.928571,8.928572)->(5.750000,10.000000)
GOT Line #5 (0.625000,5.875000)->(2.000000,10.000000)
GOT Line #6 (8.166666,5.944445)->(5.928571,8.928572)
GOT Line #7 (2.833333,5.833333)->(5.928571,8.928572)
GOT Line #8 (6.875000,0.750001)->(10.000000,2.833334)
GOT Line #9 (8.166667,5.944445)->(10.000000,5.333333)
GOT Line #10 (0.000000,5.666667)->(0.625000,5.875000)
GOT Line #11 (0.000000,2.166667)->(2.772727,0.318182)  // this!!!!!!!
GOT Line #12 (5.785715,4.357143)->(8.166667,5.944445)
GOT Line #13 (0.625000,5.875000)->(1.944444,5.611111)
GOT Line #14 (1.944444,5.611111)->(2.833333,5.833333)
GOT Line #15 (3.500000,2.500000)->(1.944444,5.611111)
GOT Line #16 (2.833333,5.833333)->(5.785715,4.357142)
GOT Line #17 (5.500000,3.500000)->(5.785715,4.357143)
GOT Line #18 (6.875000,0.750000)->(5.500000,3.500000)
GOT Line #19 (3.500000,2.500000)->(5.500000,3.500000)
GOT Line #20 (2.772727,0.318182)->(3.500000,2.500000)
GOT Line #21 (2.900000,0.000000)->(2.772727,0.318182)
GOT Line #22 (6.500000,0.000000)->(6.875000,0.750000)