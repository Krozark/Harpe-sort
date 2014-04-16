set(INCROOT ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}-learning)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}-learning)

#ROOT files
set(ROOT_SRC
)
set(ROOT_HEADER
)

set(ROOT_FILES
    ${ROOT_HEADER}
    ${ROOT_SRC}
)

#FILES
set(FILES_HEADER
    ${ROOT_HEADER}
)
set(FILES_SRC
    ${ROOT_SRC}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#libmgf libharpe-algo
set(LIBS ${LIBS})

add_executable(${CMAKE_PROJECT_NAME}-learning main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}-learning ${LIBS})
