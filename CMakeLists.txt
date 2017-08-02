cmake_minimum_required (VERSION 2.6)
project (rl2048 CXX)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-Wall -g")

include_directories (include)
set (SOURCES_LEARN src/learn.cpp src/board.cpp src/td_afterstate_agent.cpp)

add_executable (learn ${SOURCES_LEARN})
