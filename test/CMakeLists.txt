find_package(BZip2 REQUIRED)
add_executable(testtool main.cpp)
target_compile_options(testtool PRIVATE /std:c++latest)
target_link_libraries(testtool PRIVATE BZip2::BZip2)
