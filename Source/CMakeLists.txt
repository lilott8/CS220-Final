set(LIB_SOURCE
claim.cc
controller.cc
edge.cc
hadlock.cc
map.cc
node.cc
problem_object.cc
steiner.cc
spc.cc
vedge.cc
vnode.cc
voronoi.cc
)

add_library(SOURCE_LIB ${LIB_SOURCE})

set(MAIN_SRC
    main.cc
)

add_executable(flow ${MAIN_SRC})
target_link_libraries(flow SOURCE_LIB)
#target_link_libraries(flow WYKOBI_LIB)
target_link_libraries(flow JSON_LIB)
