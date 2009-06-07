
qt4_add_resources(res_files cradle.qrc)

set(headers
    Ball.h
    CubeMap.h
    Scene.h
    )

set(sources
    main.cpp
    Ball.cpp
    CubeMap.cpp
    Scene.cpp
    )

qt4_automoc(${sources})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${BULLET_INCLUDE_DIR}
    ${OPENEXR_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${CG_INCLUDE_DIR}
    )

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

add_executable(cradle ${sources} ${headers} ${ui_files} ${res_files})
target_link_libraries(cradle
    ${GLEW_LIBRARY}
    ${QT_LIBRARIES}
    ${CG_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OPENEXR_LIBRARIES}
    )
