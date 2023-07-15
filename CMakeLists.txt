#*******************************************************************************
#
#  Contributors:
#     Graunter - initial version
#*******************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT("RingBuf" C)

MESSAGE(STATUS "CMake version: " ${CMAKE_VERSION})
MESSAGE(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})

SET(CMAKE_SCRIPTS "${CMAKE_SOURCE_DIR}/cmake")
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## build settings
SET(RINGBUF_VERSION_MAJOR 0)
SET(RINGBUF_VERSION_MINOR 1)
SET(RINGBUF_VERSION_PATCH 0)
SET(RINGBUF_VERSION ${RINGBUF_VERSION_MAJOR}.${RINGBUF_VERSION_MINOR}.${RINGBUF_VERSION_PATCH})

STRING(TIMESTAMP BUILD_TIMESTAMP UTC)
MESSAGE(STATUS "Timestamp is ${BUILD_TIMESTAMP}")


ADD_SUBDIRECTORY(Src)

