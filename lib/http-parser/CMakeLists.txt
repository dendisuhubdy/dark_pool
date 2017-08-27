cmake_minimum_required(VERSION 2.8)

project(http_parser)

add_library(
  http_parser
  http_parser.c
)
set_target_properties(
  http_parser
  PROPERTIES COMPILE_FLAGS
  "-DHTTP_PARSER_STRICT=0 -Wall -Wextra -O0 -g -ftrapv -DDEBUG -D_DEBUG"
)

#add_library(
#  http_parser_strict
#  http_parser.c
#)
#set_target_properties(
#  http_parser_strict
#  PROPERTIES COMPILE_FLAGS
#  "-DHTTP_PARSER_STRICT=1 -Wall -Wextra -O0 -g -ftrapv -DDEBUG -D_DEBUG"
#)
