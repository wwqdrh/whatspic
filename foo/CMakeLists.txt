cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(libtorch_examples)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(Torch_DIR /usr/local/lib/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
include_directories( ${Torch_INCLUDE_DIRS} )
add_library(TorchDemo SHARED "")
target_sources(TorchDemo PRIVATE
    src/foo.cpp
)
set_property(TARGET TorchDemo PROPERTY CXX_STANDARD 17)
target_link_libraries(TorchDemo ${TORCH_LIBRARIES} )

set_target_properties(TorchDemo PROPERTIES PUBLIC_HEADER src/foo.h)
include(GNUInstallDirs)
install(TARGETS TorchDemo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)