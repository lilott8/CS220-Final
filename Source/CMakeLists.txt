set(LIB_SOURCE
claim.cc
controller.cc
edge.cc
flow_algo.cc
fortune.cc
grid.cc
kruskal.cc
map.cc
netlist.cc
node.cc
path.cc
problem_object.cc
pathsegment.cc
segmentgroup.cc
spm.cc
vedge.cc
vnode.cc
vpath.cc
vparabola.cc
vevent.cc
)

add_library(SOURCE_LIB ${LIB_SOURCE})

set(MAIN_SRC
    main.cc
)

add_executable(flow ${MAIN_SRC})
target_link_libraries(flow SOURCE_LIB)
target_link_libraries(flow JSON_LIB)