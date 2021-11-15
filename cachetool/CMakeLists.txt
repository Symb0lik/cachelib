find_package(BZip2 REQUIRED)
add_executable(cachetool main.cpp main.h)
target_compile_options(cachetool PRIVATE /std:c++latest)

target_include_directories(cachetool PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(cachetool PRIVATE BZip2::BZip2)
target_link_libraries(cachetool PRIVATE cachelib)