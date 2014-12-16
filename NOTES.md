##NOTES

The controller will be how we abstract the logic from the main and thus the entire framework.

The flow of the program will then be: main->controller->map & algorithm

kPins = sites (sorted PQ given x,y coordinates closest to the origin)

##TODO

 - Either add all the appropriate .o files to the make file or convince Brian to use CMake!
 - derive a good compare function for the beachline rb-tree
 - figure out why the map coords need to be switched in init!
 - why can't vedge and arch have pointers to each other??
 
