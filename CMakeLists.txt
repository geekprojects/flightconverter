cmake_minimum_required(VERSION 3.29)
project(flightconverter)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libufc REQUIRED libufc)
pkg_check_modules(nlohmann_json REQUIRED nlohmann_json)

SET(FLAGS_COMMON "-Wall -Werror")
SET(CMAKE_CXX_FLAGS_DEBUG "${FLAGS_COMMON} -O0 -g -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer")
SET(CMAKE_CXX_FLAGS_RELEASE  "${FLAGS_COMMON} -O3")

add_definitions(
        -I/usr/local/include
        ${libufc_CFLAGS}
        ${nlohmann_json_CFLAGS}
)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(flightconverter src/main.cpp
        src/utils.cpp
        src/flightconverter.cpp
        src/flightconverter.h
        src/flightplan.h
        src/formats/msfsiniformat.cpp
        src/formats/msfsiniformat.h
        src/formats/xplanefmsformat.cpp
        src/formats/xplanefmsformat.h
        src/flightplan.cpp
        src/commands/command.h
        src/commands/load.cpp
        src/commands/load.h
        src/commands/split.cpp
        src/commands/split.h
        src/commands/write.cpp
        src/commands/write.h
        src/commands/civa.cpp
        src/commands/civa.h
        src/commands/update.cpp
        src/commands/update.h
        src/commands/simplify.cpp
        src/commands/simplify.h
        src/formats/fileformat.h
        src/formats/fileformat.cpp
        src/commands/circuit.cpp
        src/commands/circuit.h
        src/commands/simbrief.cpp
        src/commands/simbrief.h
        src/formats/SimbriefJson.cpp
        src/formats/SimbriefJson.h)

target_link_libraries(flightconverter
        -Wl,-rpath -Wl,/usr/local/lib
        -L/usr/local/lib
        -lcurl
        ${libufc_LDFLAGS}
        ${nlohmann_json_LDFLAGS}
)

