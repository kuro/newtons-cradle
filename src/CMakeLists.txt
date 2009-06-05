
find_path(BULLET_INCLUDE_DIR
    NAMES btBulletDynamicsCommon.h
    PATHS
    /usr/include/bullet
    )

find_library(BULLET_DYNAMICS_LIBRARY NAMES bulletdynamics)
find_library(BULLET_COLLISION_LIBRARY NAMES bulletcollision)
find_library(BULLET_MATH_LIBRARY NAMES bulletmath LinearMath)

if (BULLET_DYNAMICS_LIBRARY
        AND BULLET_COLLISION_LIBRARY
        AND BULLET_MATH_LIBRARY
        )
    set(BULLET_LIBRARIES
        ${BULLET_DYNAMICS_LIBRARY}
        ${BULLET_COLLISION_LIBRARY}
        ${BULLET_MATH_LIBRARY}
        )
else ()
    message(FATAL_ERROR "Bullet not found")
endif ()

set(headers
    Ball.h
    Scene.h
    )

set(sources
    main.cpp
    Ball.cpp
    Scene.cpp
    )

qt4_automoc(${sources})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${BULLET_INCLUDE_DIR}
    )

add_executable(cradle ${sources} ${headers} ${ui_files} ${res_files})
target_link_libraries(cradle
    ${QT_LIBRARIES}
    ${BULLET_LIBRARIES}
    )
