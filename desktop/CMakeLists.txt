add_executable(masavault-desktop main.cpp)
target_link_libraries(masavault-desktop PRIVATE tgcloud_core)
target_compile_features(masavault-desktop PRIVATE cxx_std_20)
