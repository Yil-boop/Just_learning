#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//cargando funciones necesarias (llamadas de teclado, transformaciones etc)
void framebuffer_size_callback(GLFWwindow* window,int ancho, int alto);
void procesar_entrada(GLFWwindow* ventana);
std::string LoadShaderSource(const char* ShRuta);

int main() {
    
    //iniciar glfw, especificamos la version (3.3) con solo las funciones utiles
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //creamos la ventana, que sera el contexto en el que trabajaremos
    GLFWwindow* window = glfwCreateWindow(800,800,"experimentov.1",NULL,NULL);
    //esta funcion devuelve un puntero con el que glfw trabaja internamente
    
    //hacemos algo de debug, comprobando si glfw esta inicializando
    if(window == NULL) {
        std::cout << "error creando la ventana";
        glfwTerminate();
        return -1;
    }
    
    //aqui definimos el contexto en el que trabajaremos que es la ventana anteriormente creada
    glfwMakeContextCurrent(window);
    //cada vez que la pantalla cambia de tamaño, se supone que esta funcion hace la transformacion
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    //cargamos las funciones de glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to initialize Glad " << std::endl;
        return -1;
    }

    std::string vertexCode = LoadShaderSource("C:/Hello_Triangle/Hello_Triangle/shaders/vertex_shader.glsl");
    std::string fragmentCode = LoadShaderSource("C:/Hello_Triangle/Hello_Triangle/shaders/fragment_shader.glsl");

    //declaramos donde esta el source code de los shaders
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();
    
    //creamos los shaders (ahora si)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    int sucess;
    char infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);

    if (!sucess) {
    glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
    std::cout << "VERTEX SHADER COMPILATION ERROR:\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&sucess);

    if (!sucess) {
    glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
    std::cout << "FRAGMENT SHADER COMPILATION ERROR:\n" << infoLog << std::endl;
   }

    //creamos el programa que usara la pipeline
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
    if (!sucess){
    glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
    std::cout << "SHADER PROGRAM LINK ERROR:\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //el triangulo que usaremos
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   //top left
    };
    
    //indices que usaremos
    unsigned int indices[] = {
        1, 2, 0,
        3, 0, 2
    };
    
    //creamos el VERTEX ARRAY OBJECT, guarda las instrucciones que queremos darle a la gpu
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    //el Vertex Buffer Object, contiene los datos que usara la gpu
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    //el EBO guarda los indices de los vertices que creamos
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //definimos como interpretar los datos del VBO al la gpu
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    //el array buffer manda datos al indice 0
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
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        //verificar eventos y cambiar de buffers(font buffer y back buffer)
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

//funcion para cargar los shaders
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
//funcion para transformar la pantalla en cada render
void framebuffer_size_callback(GLFWwindow* window,int ancho, int alto) {
    glViewport(0,0,ancho,alto);
}
//procesar entrada.
void procesar_entrada(GLFWwindow* ventana) {
    if(glfwGetKey(ventana,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(ventana,true);
    }
}
