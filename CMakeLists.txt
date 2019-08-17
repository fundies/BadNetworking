cmake_minimum_required(VERSION 3.0)

project(GServer VERSION 0.1.0 LANGUAGES CXX)
set(API_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(BUILD_VERSION "${PROJECT_VERSION}")

set(SSOURCES
  "server.cpp"
)

set(CSOURCES
  "client.cpp"
)

add_executable("GServer" "${SSOURCES}")
add_executable("GClient" "${CSOURCES}")

target_link_libraries("GServer" "enet")
target_link_libraries("GClient" "enet")
