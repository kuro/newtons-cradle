macro(fetch file)
    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
        message(STATUS "Downloading ${file}")
        file(DOWNLOAD
            http://mentaldistortion.com/media/${file}
            ${CMAKE_CURRENT_SOURCE_DIR}/${file}
            )
    endif()
endmacro()

fetch(cubemaps/SwedishRoyalCastle.exr)
fetch(textures/flame.jpg)
