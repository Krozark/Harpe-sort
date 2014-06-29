set(INCROOT ${PROJECT_SOURCE_DIR}/include/GeneticEngine)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/GeneticEngine)

#ROOT files
set(ROOT_SRC
    ${SRCROOT}/GeneticThread.cpp
)
set(ROOT_HEADER
    ${INCROOT}/GeneticEngine.hpp
    ${INCROOT}/GeneticEngine.tpl
    ${INCROOT}/GeneticThread.hpp
    ${INCROOT}/GeneticThread.tpl
    ${INCROOT}/random.hpp
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

#SHARED lib
add_library(GeneticEngine SHARED ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(GeneticEngine
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
INSTALL(TARGETS GeneticEngine
    DESTINATION lib
)


#static lib
add_library(GeneticEngine-static STATIC ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(GeneticEngine-static
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
INSTALL(TARGETS GeneticEngine-static
    DESTINATION lib
)

#header
#INSTALL(FILES ${INCROOT}
#    DESTINATION include
#    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
#)

file(GLOB DEPLOY_FILES_AND_DIRS "${PROJECT_SOURCE_DIR}/include/GeneticEngine/*")
foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
   IF( IS_DIRECTORY "${ITEM}" )
      LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
   ELSE()
      LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
   ENDIF()
endforeach()
INSTALL( FILES ${FILES_TO_DEPLOY} ${BisonOutputInc} DESTINATION include/GeneticEngine )
INSTALL( DIRECTORY ${DIRS_TO_DEPLOY} DESTINATION include/GeneticEngine )
