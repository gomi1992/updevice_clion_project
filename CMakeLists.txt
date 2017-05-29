project(updevice_clion_project C)
cmake_minimum_required(VERSION 3.6)

#set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE USER_SOURCES "UP_TestAll/*.c")
file(GLOB_RECURSE UPLIB_SOURCES "UP_TestAll/UpLib/*.c")

add_library(UPLIB ${UPLIB_SOURCES})

include_directories(UP_TestAll)
include_directories(UpLib)
include_directories(UpLib/STM32Lib)

add_definitions(-DSTM32F303xE)
add_definitions(-DSTM32F10X_HD)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DDEBUG_ON)
else()
    add_definitions(-DDEBUG_OFF)
endif()

add_executable(${PROJECT_NAME}.elf ${USER_SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf UPLIB)

set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")
