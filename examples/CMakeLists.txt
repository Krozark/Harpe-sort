## EXAMPLES
if(BUILD_EXAMPLES)
    #add_executable(${CMAKE_PROJECT_NAME}-test main.cpp ${FILES_HEADER} ${FILES_SRC})
	add_executable(${CMAKE_PROJECT_NAME}-test main.cpp)
	target_link_libraries(${CMAKE_PROJECT_NAME}-test $ ${CMAKE_PROJECT_NAME})
endif()
