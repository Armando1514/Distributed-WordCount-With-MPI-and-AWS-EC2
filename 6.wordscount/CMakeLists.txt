project(wordcount C)

set(CMAKE_C_STANDARD 99)

# Set minimum required version for CMake
cmake_minimum_required(VERSION 3.00)


# Define C standard
set(CMAKE_C_STANDARD 99)

# Use Open MPI C wrapper compiler
set(CMAKE_C_COMPILER mpicc)

# Set minimum required version for CMake
cmake_minimum_required(VERSION 3.00)

# Define libraries
add_library(fileparser STATIC src/fileparser.c)
target_include_directories(fileparser PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(workerhelper STATIC src/workerhelper.c)
target_include_directories(workerhelper PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(hashmap STATIC src/hashmap.c)
target_include_directories(hashmap PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(mpihelper STATIC src/mpihelper.c)
target_include_directories(mpihelper PUBLIC ${PROJECT_SOURCE_DIR}/include)


# Define the (only) executable of the project
add_executable(${PROJECT_NAME} src/main.c)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE  workerhelper fileparser  hashmap mpihelper )
