#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window,int ancho, int alto);
void procesar_entrada(GLFWwindow* ventana);
std::string LoadShaderSource(const char* ShRuta);
std::string vertexCode = LoadShaderSource("shaders/vertex_shader.glsl");
std::string fragmentCode = LoadShaderSource("shaders/fragment_shader.glsl");

int main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,800,"experimentov.1",NULL,NULL);

      if(window == NULL) {
        std::cout << "error creando la ventana";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to initialize Glad " << std::endl;
        return -1;
    }

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    int sucess;
    char infolog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&sucess);

    if(!sucess) {
        glGetShaderInfoLog(vertexShader,512,NULL,infolog);
        std::cout << "ERROR_COMPILATION_SHADER" << infolog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5,-0.5, 0.0, // a
         0.5,-0.5, 0.0, // b
         0.0, 0.5, 0.0  // c
    };

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {

        //input
        procesar_entrada(window);

        //renderizar cosas aqui...
        glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);

        //verificar eventos y cambiar de buffers(font buffer y back buffer)
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

std::string LoadShaderSource(const char* ShRuta) {
    std::ifstream file(ShRuta);
    if(!file.is_open()) {
        std::cerr << "error, no se puede abrir el glsl" << ShRuta << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void framebuffer_size_callback(GLFWwindow* window,int ancho, int alto) {
    glViewport(0,0,ancho,alto);
}
void procesar_entrada(GLFWwindow* ventana) {
    if(glfwGetKey(ventana,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(ventana,true);
    }
}
