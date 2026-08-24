find_package(Threads REQUIRED)
add_executable(masavault-desktop
  main.cpp
  session.cpp
  http_server.cpp
  ui/transfer_center.cpp
  ui/navigation.cpp
  ui/setup.cpp
  ui/actions.cpp
  ui/settings.cpp)
target_link_libraries(masavault-desktop PRIVATE tgcloud_core Threads::Threads)
target_compile_features(masavault-desktop PRIVATE cxx_std_20)
target_include_directories(masavault-desktop PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/ui)
install(TARGETS masavault-desktop RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY www/ DESTINATION ${CMAKE_INSTALL_DATADIR}/masavault/www)
