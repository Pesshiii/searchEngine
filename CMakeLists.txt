cmake_minimum_required(VERSION 3.5)

project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#########################################################################
include(FetchContent)

FetchContent_Declare(
    json
    URL
    https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


##

FetchContent_Declare(
   googletest
   URL
   https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

##########################################################################


include_directories(include
                    tests)
file(GLOB SRC src/*.cpp)
add_executable(search_engine ${SRC})




#######################################


target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(search_engine PRIVATE gtest_main)

include(GoogleTest)
set(DISCOVERY_MODE PRE_TEST)
gtest_add_tests(TARGET search_engine)