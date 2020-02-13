# Copyright 2017 The Abseil Authors.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#    https://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(CMakeParseArguments)
include(ConfigCopts)
# include(BytemeInstallDirs)

# The IDE folder for Qian that will be used if Qian is included in a CMake
# project that sets
#    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# For example, Visual Studio supports folders.
# set(BEANQUICK_IDE_FOLDER beanquick)

# beanquick_cc_library()
# CMake function to imitate Bazel's cc_library rule.
# Parameters:
# NAME: name of target (see Note)
# HDRS: List of public header files for the library
# SRCS: List of source files for the library
# DEPS: List of other libraries to be linked in to the binary targets
# COPTS: List of private compile options
# DEFINES: List of public defines
# LINKOPTS: List of link options
# PUBLIC: Add this so that this library will be exported under beanquick::
# Also in IDE, target will appear in Qian folder while non PUBLIC will be in Qian/internal.
# TESTONLY: When added, this target will only be built if user passes -DBEANQUICK_RUN_TESTS=ON to CMake.
# Note:
# By default, beanquick_cc_library will always create a library named beanquick_${NAME},
# and alias target beanquick::${NAME}.  The beanquick:: form should always be used.
# This is to reduce namespace pollution.
# beanquick_cc_library(
#   NAME
#     awesome
#   HDRS
#     "a.h"
#   SRCS
#     "a.cc"
# )
# beanquick_cc_library(
#   NAME
#     fantastic_lib
#   SRCS
#     "b.cc"
#   DEPS
#     beanquick::awesome # not "awesome" !
#   PUBLIC
# )
# beanquick_cc_library(
#   NAME
#     main_lib
#   ...
#   DEPS
#     beanquick::fantastic_lib
# )
# TODO: Implement "ALWAYSLINK"
function(bean_cc_library)
  cmake_parse_arguments(BEANQUICK_CC_LIB
    "DISABLE_INSTALL;PUBLIC;TESTONLY"
    "NAME"
    "HDRS;SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  if(NOT BEANQUICK_CC_LIB_TESTONLY OR BEANQUICK_RUN_TESTS)
    if(BEANQUICK_ENABLE_INSTALL)
      set(_NAME "${BEANQUICK_CC_LIB_NAME}")
    else()
      set(_NAME "bean_${BEANQUICK_CC_LIB_NAME}")
    endif()

    # Check if this is a header-only library
    # Note that as of February 2019, many popular OS's (for example, Ubuntu
    # 16.04 LTS) only come with cmake 3.5 by default.  For this reason, we can't
    # use list(FILTER...)
    set(BEANQUICK_CC_SRCS "${BEANQUICK_CC_LIB_SRCS}")
    foreach(src_file IN LISTS BEANQUICK_CC_SRCS)
      if(${src_file} MATCHES ".*\\.(h|inc)")
        list(REMOVE_ITEM BEANQUICK_CC_SRCS "${src_file}")
      endif()
    endforeach()
    if("${BEANQUICK_CC_SRCS}" STREQUAL "")
      set(BEANQUICK_CC_LIB_IS_INTERFACE 1)
    else()
      set(BEANQUICK_CC_LIB_IS_INTERFACE 0)
    endif()

    if(NOT BEANQUICK_CC_LIB_IS_INTERFACE)
      add_library(${_NAME} STATIC "")
      target_sources(${_NAME} PRIVATE ${BEANQUICK_CC_LIB_SRCS} ${BEANQUICK_CC_LIB_HDRS})
      target_include_directories(${_NAME}
        PUBLIC
          "$<BUILD_INTERFACE:${BEANQUICK_COMMON_INCLUDE_DIRS}>"
          $<INSTALL_INTERFACE:${BEANQUICK_INSTALL_INCLUDEDIR}>
      )
      target_compile_options(${_NAME}
        PRIVATE ${BEANQUICK_CC_LIB_COPTS})
      target_link_libraries(${_NAME}
        PUBLIC ${BEANQUICK_CC_LIB_DEPS}
        PRIVATE
          ${BEANQUICK_CC_LIB_LINKOPTS}
          ${BEANQUICK_DEFAULT_LINKOPTS}
      )
      target_compile_definitions(${_NAME} PUBLIC ${BEANQUICK_CC_LIB_DEFINES})

      # Add all Beanquick targets to a a folder in the IDE for organization.
      if(BEANQUICK_CC_LIB_PUBLIC)
        set_property(TARGET ${_NAME} PROPERTY FOLDER ${BEANQUICK_IDE_FOLDER})
      elseif(BEANQUICK_CC_LIB_TESTONLY)
        set_property(TARGET ${_NAME} PROPERTY FOLDER ${BEANQUICK_IDE_FOLDER}/test)
      else()
        set_property(TARGET ${_NAME} PROPERTY FOLDER ${BEANQUICK_IDE_FOLDER}/internal)
      endif()

      # INTERFACE libraries can't have the CXX_STANDARD property set
      set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD ${BEANQUICK_CXX_STANDARD})
      set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

      # When being installed, we lose the beanquick_ prefix.  We want to put it back
      # to have properly named lib files.  This is a no-op when we are not being
      # installed.
      set_target_properties(${_NAME} PROPERTIES
        OUTPUT_NAME "beanquick_${_NAME}"
      )
    else()
      # Generating header-only library
      add_library(${_NAME} INTERFACE)
      target_include_directories(${_NAME}
        INTERFACE
          "$<BUILD_INTERFACE:${BEANQUICK_COMMON_INCLUDE_DIRS}>"
          $<INSTALL_INTERFACE:${BEANQUICK_INSTALL_INCLUDEDIR}>
        )
      target_link_libraries(${_NAME}
        INTERFACE
          ${BEANQUICK_CC_LIB_DEPS}
          ${BEANQUICK_CC_LIB_LINKOPTS}
          ${BEANQUICK_DEFAULT_LINKOPTS}
      )
      target_compile_definitions(${_NAME} INTERFACE ${BEANQUICK_CC_LIB_DEFINES})
    endif()

    # TODO currently we don't install googletest alongside abseil sources, so
    # installed abseil can't be tested.
    if(NOT BEANQUICK_CC_LIB_TESTONLY AND BEANQUICK_ENABLE_INSTALL)
      install(TARGETS ${_NAME} EXPORT ${PROJECT_NAME}Targets
            RUNTIME DESTINATION ${BEANQUICK_INSTALL_BINDIR}
            LIBRARY DESTINATION ${BEANQUICK_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${BEANQUICK_INSTALL_LIBDIR}
      )
    endif()

    add_library(bean::${BEANQUICK_CC_LIB_NAME} ALIAS ${_NAME})
  endif()
endfunction()

# beanquick_cc_test()
# CMake function to imitate Bazel's cc_test rule.
# Parameters:
# NAME: name of target (see Usage below)
# SRCS: List of source files for the binary
# DEPS: List of other libraries to be linked in to the binary targets
# COPTS: List of private compile options
# DEFINES: List of public defines
# LINKOPTS: List of link options
# Note:
# By default, beanquick_cc_test will always create a binary named beanquick_${NAME}.
# This will also add it to ctest list as beanquick_${NAME}.
# Usage:
# beanquick_cc_library(
#   NAME
#     awesome
#   HDRS
#     "a.h"
#   SRCS
#     "a.cc"
#   PUBLIC
# )
# beanquick_cc_test(
#   NAME
#     awesome_test
#   SRCS
#     "awesome_test.cc"
#   DEPS
#     beanquick::awesome
#     gmock
#     gtest_main
# )
function(bean_cc_test)
  # if(NOT BEANQUICK_RUN_TESTS)
  #   return()
  # endif()

  cmake_parse_arguments(BEANQUICK_CC_TEST
    ""
    "NAME"
    "SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  set(_NAME "beanquick_${BEANQUICK_CC_TEST_NAME}")
  add_executable(${_NAME} "")
  target_sources(${_NAME} PRIVATE ${BEANQUICK_CC_TEST_SRCS})
  target_include_directories(${_NAME}
    PUBLIC ${BEANQUICK_COMMON_INCLUDE_DIRS}
    PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS}
  )
  target_compile_definitions(${_NAME}
    PUBLIC ${BEANQUICK_CC_TEST_DEFINES}
  )
  target_compile_options(${_NAME}
    PRIVATE ${BEANQUICK_CC_TEST_COPTS}
  )
  target_link_libraries(${_NAME}
    PUBLIC ${BEANQUICK_CC_TEST_DEPS}
    PRIVATE ${BEANQUICK_CC_TEST_LINKOPTS}
  )
  # Add all Qian targets to a a folder in the IDE for organization.
  set_property(TARGET ${_NAME} PROPERTY FOLDER ${BEANQUICK_IDE_FOLDER}/test)

  set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD ${BEANQUICK_CXX_STANDARD})
  set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

  add_test(NAME ${_NAME} COMMAND ${_NAME})
endfunction()


function(check_target my_target)
  if(NOT TARGET ${my_target})
    message(FATAL_ERROR " BEANQUICK: compiling beanquick requires a ${my_target} CMake
                   target in your project, see CMake/README.md for more details")
  endif(NOT TARGET ${my_target})
endfunction()
