set(headers
    Scene.h
    )

set(sources
    main.cpp
    Scene.cpp
    )

qt4_automoc(${sources})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(cradle ${sources} ${headers} ${ui_files} ${res_files})
target_link_libraries(cradle
    ${QT_LIBRARIES}
    )
