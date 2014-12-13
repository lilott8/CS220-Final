set(LIB_SOURCE
claim.cc
edge.cc
grid.cc
netlist.cc
node.cc
path.cc
pathsegment.cc
segmentgroup.cc
problem_object.cc
vnode.cc
vpath.cc
map.cc
algorithm.cc
controller.cc
fortune.cc
kruskal.cc
spm.cc
rb.c
)

add_library(SOURCE_LIB ${LIB_SOURCE})

set(MAIN_SRC
    main.cc
)

add_executable(flow ${MAIN_SRC})
target_link_libraries(flow SOURCE_LIB)
target_link_libraries(flow JSON_LIB)