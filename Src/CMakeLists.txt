#*******************************************************************************
#
#*******************************************************************************/


configure_file( 
       ${CMAKE_CURRENT_SOURCE_DIR}/../Inc/RingBufVersion.h.in 
       ${CMAKE_CURRENT_SOURCE_DIR}/../Inc/RingBufVersion.h 
)

SET(CMAKE_CXX_STANDARD 11)


ADD_LIBRARY( RingBuf
       RingBuf.c
)


TARGET_INCLUDE_DIRECTORIES( RingBuf
       PUBLIC
       .
       ${CMAKE_CURRENT_SOURCE_DIR}/../Inc/
)

