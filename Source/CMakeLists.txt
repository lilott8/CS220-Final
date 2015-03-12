set(LIB_SOURCE
claim.cc
controller.cc
edge.cc
grid.cc
map.cc
netlist.cc
node.cc
path.cc
problem_object.cc
pathsegment.cc
prim.cc
segmentgroup.cc
steiner.cc
spc.cc
vedge.cc
vnode.cc
voronoi.cc
vpath.cc
)

add_library(SOURCE_LIB ${LIB_SOURCE} ../Headers/graph_alogs.h graph_alogs.cpp)

set(MAIN_SRC
    main.cc
)

add_executable(flow ${MAIN_SRC})
target_link_libraries(flow SOURCE_LIB)
#target_link_libraries(flow WYKOBI_LIB)
target_link_libraries(flow JSON_LIB)
