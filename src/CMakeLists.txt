add_subdirectory(${CMAKE_PROJECT_NAME})
if(BUILD_GENERATOR)
    add_subdirectory(${CMAKE_PROJECT_NAME}-generator)
endif()
