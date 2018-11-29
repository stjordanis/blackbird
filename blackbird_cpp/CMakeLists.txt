cmake_minimum_required(VERSION 2.8.9)
project(blackbird_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)


#Bring the headers
include_directories(/home/josh/.local/include)
include_directories(/home/josh/.local/include/antlr4-runtime)

#GDB debug flag
set(CMAKE_BUILD_TYPE RelWithDebInfo)

#Can manually add the sources using the set command as follows:
set(SOURCES main_antlr.cpp Visitor.cpp blackbirdBaseListener.cpp blackbirdLexer.cpp blackbirdParser.cpp)
##set(SOURCES main_antlr.cpp)

#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "AsyncSerial/MeComAPI/*.cpp")

add_executable(antlr_cpp ${SOURCES})

find_library(ANTLR4_RUNTIME antlr4-runtime HINTS /home/josh/.local/lib)
#add_subdirectory(Chat_Client)

## Boost Link libraries
##set(BOOST_LIBS thread date_time system)
##find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

##target_link_libraries(chipcontrol ${Boost_LIBRARIES})
#find_package(Threads REQUIRED)
target_link_libraries(antlr_cpp -O ${ANTLR4_RUNTIME})