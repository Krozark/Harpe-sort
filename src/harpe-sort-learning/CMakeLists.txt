set(INCROOT ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}-learning)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}-learning)

set(ROOT_SRC
    ${SRCROOT}/Entity.cpp
    ${SRCROOT}/Sequence.cpp
    ${SRCROOT}/Spectrum.cpp
    ${SRCROOT}/std.cpp
)
set(ROOT_HEADER
    ${INCROOT}/Entity.hpp
    ${INCROOT}/Sequence.hpp
    ${INCROOT}/Spectrum.hpp
    ${INCROOT}/std.hpp
    ${INCROOT}/std.tpl
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

#find_library(MONITORING_LIB Monitoring)
#set(LIBS ${LIBS} ${MGF_LIB} ${MONITORING_LIB} ${HARPE_ALGO_LIB})

find_library(MGF_LIB mgf)
find_library(HARPE_ALGO_LIB harpe-algo)
set(LIBS ${LIBS} ${MGF_LIB} ${HARPE_ALGO_LIB})

add_executable(${CMAKE_PROJECT_NAME}-learning main.cpp ${FILES_SRC} ${FILES_HEADER})
target_link_libraries(${CMAKE_PROJECT_NAME}-learning ${LIBS})
