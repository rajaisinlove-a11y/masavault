cmake_minimum_required(VERSION 3.16)
project(masavault VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

enable_testing()
add_subdirectory(core)
add_subdirectory(desktop)
add_subdirectory(tests)

install(FILES README.md LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY docs/ DESTINATION ${CMAKE_INSTALL_DOCDIR})

set(CPACK_PACKAGE_NAME "masavault")
set(CPACK_PACKAGE_VENDOR "MasaVault")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TG Cloud Desktop — Linux core for the Android TG Cloud ecosystem")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/\\\\.git/" "/build/" "/\\\\.cache/")
include(CPack)
