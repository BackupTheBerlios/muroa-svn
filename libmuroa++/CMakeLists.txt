cmake_minimum_required (VERSION 2.6)
project (libmuroa++)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


add_definitions(  )


set( SRC           CItemBase.cpp
                   CMediaItem.cpp
                   CCategoryItem.cpp
                   libmain.cpp 
                   )
                  

set( HEADERS       CItemBase.h
                   CMediaItem.h
                   CCategoryItem.h
                   )
                   
include_directories(${CMAKE_SOURCE_DIR})

link_directories(  )

add_library( muroa++ SHARED ${SRC} ${HEADERS} )
target_link_libraries( muroa++     )
set_target_properties( muroa++  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   muroa++ DESTINATION  lib)
