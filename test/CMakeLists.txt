cmake_minimum_required(VERSION 2.6)

# We need thread support
find_package(Threads REQUIRED)
 
# Locate GTest
find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
)


# Link runTests with what we want to test and the GTest and pthread library
file(GLOB SOURCE_FILES *.cpp)
add_executable(runTests ${SOURCE_FILES})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
