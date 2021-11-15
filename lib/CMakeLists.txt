add_library(cachelib STATIC index.cpp index.h)
target_compile_options(cachelib PRIVATE /std:c++latest)
