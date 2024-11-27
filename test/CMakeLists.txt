cmake_minimum_required(VERSION 3.10)
project(DBTest)

# Enable testing
enable_testing()

# Add Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(${PROJECT_NAME} SelectTest.cpp)

# Link against Google Test
target_link_libraries(${PROJECT_NAME} gtest_main memesql)

# Add test
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})