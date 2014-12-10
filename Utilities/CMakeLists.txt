file(GLOB JSON_SOURCE
    JSON_parser/cc_h/*.*
    JSON_parser/Source/*.*
)

set(OTHERS
JSON_parser/json_parser_bison.cc
JSON_parser/json_parser_lex.cc
)

#Add the necessary files
include_directories(JSON_parser/Headers)
include_directories(JSON_parser)

add_library(JSON_LIB ${JSON_SOURCE} ${OTHERS})