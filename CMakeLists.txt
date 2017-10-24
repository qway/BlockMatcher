cmake_minimum_required (VERSION 3.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project (BlockMatcher)
find_package( OpenCV REQUIRED )


add_executable(BlockMatcher src/main.cpp)
target_link_libraries( BlockMatcher ${OpenCV_LIBS} )

#Change startup project to get debugging working
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT BlockMatcher)
