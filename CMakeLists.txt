cmake_minimum_required(VERSION 3.19)
project(SquidGames)

set(CMAKE_CXX_STANDARD 14)

add_executable(SquidGames lib/main1.cpp src/utils/Auxiliaries.cpp src/Tree/TNode.hpp src/Tree/Tree.hpp
        src/PlayersManager/PlayersManager.cpp src/PlayersManager/PlayersManager.h src/Tree/Exceptions.h)