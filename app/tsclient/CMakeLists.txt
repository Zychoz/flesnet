# Copyright 2013 Jan de Cuveland <cmail@cuveland.de>

file(GLOB APP_SOURCES *.cpp)
file(GLOB APP_HEADERS *.h)

include_directories("${PROJECT_SOURCE_DIR}/lib/fles_ipc"
                    "${PROJECT_SOURCE_DIR}/lib/fles_core"
                    "${PROJECT_SOURCE_DIR}/lib/logging"
                    "${PROJECT_SOURCE_DIR}/lib/crcutil")

add_executable(tsclient ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(tsclient fles_ipc fles_core logging crcutil ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(tsclient ${ZMQ_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(tsclient rt)
endif()
