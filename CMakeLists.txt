cmake_minimum_required(VERSION 3.12)

PROJECT(ustblogind)

add_definitions("-Wall -O2")

INCLUDE_DIRECTORIES(
    include
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
    /usr/include/gdk-pixbuf-2.0
)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(ustblogind
${DIR_SRCS}
)

ADD_EXECUTABLE(${PROJECT_NAME} ${ustblogind})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} readline curl)