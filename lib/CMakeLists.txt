find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)

add_library(cachelib STATIC "index.cpp" "index.h" "datafile.h" "datafile.cpp" "archive.h" "archive.cpp" "xtea.h" "xtea.cpp" "metadata.h" "metadata.cpp" "cachestore.h" "index_file.cpp")
target_compile_options(cachelib PRIVATE /std:c++latest)
target_link_libraries(cachelib PRIVATE BZip2::BZip2)
target_link_libraries(cachelib PRIVATE ZLIB::ZLIB)

