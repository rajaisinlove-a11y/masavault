add_library(tgcloud_core STATIC src/core.cpp src/memory_store.cpp)
target_include_directories(tgcloud_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(tgcloud_core PUBLIC cxx_std_20)
