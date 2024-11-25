cmake_minimum_required(VERSION 3.29)
project(flightconverter)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libufc REQUIRED libufc)

SET(FLAGS_COMMON "-Wall -Werror")
SET(CMAKE_CXX_FLAGS_DEBUG "${FLAGS_COMMON} -O0 -g -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer")
SET(CMAKE_CXX_FLAGS_RELEASE  "${FLAGS_COMMON} -O3")

add_definitions( -I/usr/local/include ${libufc_CFLAGS} )

add_executable(flightconverter main.cpp
        utils.cpp
        flightconverter.cpp
        flightconverter.h
        flightplan.h
        msfsformat.cpp
        msfsformat.h
        xplanefmsformat.cpp
        xplanefmsformat.h
        flightplan.cpp)

target_link_libraries(flightconverter
        -Wl,-rpath -Wl,/usr/local/lib
        -L/usr/local/lib
        ${libufc_LDFLAGS}
)

