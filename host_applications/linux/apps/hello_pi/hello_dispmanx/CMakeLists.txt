set(EXEC hello_dispmanx.bin)
set(SRCS dispmanx.c)

include_directories(/lib/usr/)
add_definitions(-ggdb)
add_executable(${EXEC} ${SRCS})
target_link_libraries(${EXEC} ${HELLO_PI_LIBS} X11)

install(TARGETS ${EXEC}
        RUNTIME DESTINATION bin)
