cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(experiment LANGUAGES C CXX)

# Usar C++ (ajusta la versión si quieres)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Rutas (equivalentes a las variables del BAT)
set(GLAD_INCLUDE ../include)
set(GLFW_INCLUDE C:/glfw-3.4/glfw-3.4/include)
set(GLFW_LIB C:/glfw-3.4/glfw-3.4/build/src)

# Archivos fuente
add_executable(experiment
    main.cpp
    glad.c
)

# Includes
target_include_directories(experiment PRIVATE
    ${GLAD_INCLUDE}
    ${GLFW_INCLUDE}
)

# Librerías
target_link_directories(experiment PRIVATE
    ${GLFW_LIB}
)

target_link_libraries(experiment
    glfw3
    opengl32
    gdi32
)

# En Windows mostrar consola (equivalente a -mconsole)
set_target_properties(experiment PROPERTIES
    WIN32_EXECUTABLE OFF
)
