# Copyright (c) 2013, 2016, Oracle and/or its affiliates. All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

INCLUDE_DIRECTORIES(
  ${GTEST_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/sql
  ${CMAKE_SOURCE_DIR}/sql/auth
  ${CMAKE_SOURCE_DIR}/storage/innobase/include
  ${CMAKE_SOURCE_DIR}/unittest/gunit
)

INCLUDE(${CMAKE_SOURCE_DIR}/storage/innobase/innodb.cmake)

SET(TESTS
  #example
  ha_innodb
  mem0mem
  ut0crc32
  ut0lock_free_hash
  ut0mem
  ut0new
)

SET(ALL_INNODB_TESTS)
FOREACH(test ${TESTS})
  LIST(APPEND ALL_INNODB_TESTS ${test}-t.cc)
ENDFOREACH()

IF (MERGE_UNITTESTS)
  IF(WIN32)
    LIST(APPEND ALL_INNODB_TESTS ../../../sql/nt_servc.cc)
  ENDIF()
  ADD_EXECUTABLE(merge_innodb_tests-t ${ALL_INNODB_TESTS})

  TARGET_LINK_LIBRARIES(merge_innodb_tests-t
    sql_main binlog rpl master slave sql_main sql_gis)
  TARGET_LINK_LIBRARIES(merge_innodb_tests-t
    gunit_large strings dbug regex mysys)
  TARGET_LINK_LIBRARIES(merge_innodb_tests-t
    sql_main binlog rpl master slave sql_main sql_gis sql_main)

  # Download TBB from https://www.threadingbuildingblocks.org
  #SET(TBB_INCDIR /path/to/tbb43_20150611oss/include)
  #SET(TBB_LIBDIR /path/to/tbb43_20150611oss/build/linux_intel64_gcc_cc4.9.1_libc2.12_kernel2.6.39_release)
  #TARGET_LINK_LIBRARIES(merge_innodb_tests-t tbb -L${TBB_LIBDIR} -Wl,-rpath=${TBB_LIBDIR})
  #INCLUDE_DIRECTORIES(${TBB_INCDIR})

  IF(WITH_PERFSCHEMA_STORAGE_ENGINE)
    TARGET_LINK_LIBRARIES(merge_innodb_tests-t perfschema)
  ENDIF()

  ADD_DEPENDENCIES(merge_innodb_tests-t GenError)

  ADD_TEST(merge_innodb_tests merge_innodb_tests-t)

ENDIF(MERGE_UNITTESTS)

FOREACH(test ${TESTS})
  SET(SRC_FILES ${test}-t.cc)
  IF(WIN32)
    LIST(APPEND SRC_FILES ../../../sql/nt_servc.cc)
  ENDIF()
  ADD_EXECUTABLE(${test}-t ${SRC_FILES})

  TARGET_LINK_LIBRARIES(${test}-t
    sql_main binlog rpl master slave sql_main sql_gis)
  TARGET_LINK_LIBRARIES(${test}-t gunit_large strings dbug regex mysys)
  TARGET_LINK_LIBRARIES(${test}-t
    sql_main binlog rpl master slave sql_main sql_gis sql_main)

  IF(WITH_PERFSCHEMA_STORAGE_ENGINE)
    TARGET_LINK_LIBRARIES(${test}-t perfschema)
  ENDIF()

  ADD_DEPENDENCIES(${test}-t GenError)

  IF(MERGE_UNITTESTS)
    SET_PROPERTY(TARGET ${test}-t PROPERTY EXCLUDE_FROM_ALL TRUE)
    IF(WIN32)
      SET_PROPERTY(TARGET ${test}-t PROPERTY EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    ENDIF()
  ELSE()
    ADD_TEST(${test} ${test}-t)
  ENDIF()

ENDFOREACH()
