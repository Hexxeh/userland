set(EXEC hwcursor)
set(SRCS hwcursor.c)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/host_applications/linux/libs/bcm_host/include)
set(HELLO_PI_LIBS bcm_host vcos vchiq_arm)

include_directories(/lib/usr/)
add_definitions(-ggdb)
add_executable(${EXEC} ${SRCS})
target_link_libraries(${EXEC} ${HELLO_PI_LIBS} X11)

install(TARGETS ${EXEC}
        RUNTIME DESTINATION bin)
