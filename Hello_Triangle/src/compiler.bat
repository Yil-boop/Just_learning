@echo off
REM Cambia esto si tus headers o librerías están en otra ruta
set GLAD_INCLUDE=../include
set GLFW_INCLUDE=C:/glfw-3.4/glfw-3.4/include
set GLFW_LIB=C:/glfw-3.4/glfw-3.4/build/src

REM Archivo a compilar (argumento del BAT)
set FILE=%1

IF "%FILE%"=="" (
    echo Usa: build.BAT nombre_del_archivo.cpp
    exit /b
)

REM Nombre del ejecutable
set EXE=%~n1.exe

REM Compilar con g++
g++ %FILE% glad.c -I%GLAD_INCLUDE% -I%GLFW_INCLUDE% -L%GLFW_LIB% -lglfw3 -lopengl32 -lgdi32 -mconsole -o %EXE%

IF %ERRORLEVEL% NEQ 0 (
    echo Error de compilacion.
    pause
    exit /b
)

REM Ejecutar el programa
%EXE%
pause

