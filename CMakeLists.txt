cmake_minimum_required(VERSION 3.5)

project(beanquick CXX)
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/CMake
  ${CMAKE_CURRENT_LIST_DIR}/CMake/copts
)

# include(AbseilInstallDirs)
include(CMakePackageConfigHelpers)
include(BeanquickHelper)

# include current path
list(APPEND BEANQUICK_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Enable CTest.
include(CTest)
enable_testing()

include(CMake/Googletest/DownloadGTest.cmake)
set(beanquick_gtest_src_dir ${CMAKE_BINARY_DIR}/googletest-src)
set(beanquick_gtest_build_dir ${CMAKE_BINARY_DIR}/googletest-build)

check_target(gtest)
check_target(gtest_main)
check_target(gmock)

list(APPEND BEANQUICK_TEST_COMMON_LIBRARIES
  gtest_main
  gtest
  gmock
  ${CMAKE_THREAD_LIBS_INIT}
)

add_subdirectory(third_party/libfixed)

# include abseil path
list(APPEND BEANQUICK_COMMON_INCLUDE_DIRS
     ${CMAKE_CURRENT_SOURCE_DIR}/third_party/abseil)
add_subdirectory(third_party/abseil)

add_subdirectory(beanquick)
