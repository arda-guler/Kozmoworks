cmake_minimum_required (VERSION 2.6)

project(Kozmoworks)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${PROJECT_SOURCE_DIR}/bin")

include_directories(src src/include/nlohmann)

add_executable(Kozmoworks
	src/Body.cpp
	src/constants.cpp
	src/main.cpp
	src/Maneuver.cpp
	src/RigidBody.cpp
	src/Solver.cpp
	src/Vector3.cpp
	src/Output.cpp
	src/Atmosphere.cpp
)
