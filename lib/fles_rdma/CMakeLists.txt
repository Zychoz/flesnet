# Copyright 2015 Jan de Cuveland <cmail@cuveland.de>

file(GLOB LIB_SOURCES !(._)*.cpp)
file(GLOB LIB_HEADERS !(._)*.h)

include_directories("${PROJECT_SOURCE_DIR}/lib/fles_ipc"
                    "${PROJECT_SOURCE_DIR}/lib/fles_core"
                    "${PROJECT_SOURCE_DIR}/lib/logging"
		    "${PROJECT_SOURCE_DIR}/lib/httpClient")

add_library(fles_rdma ${LIB_SOURCES} ${LIB_HEADERS})
