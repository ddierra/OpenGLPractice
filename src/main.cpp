#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"}\n\0";

const char *fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";
    
int main() {
    //Initialize GLFW
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   #ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   #endif

   //Window creation
   GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Practice", NULL, NULL);
   if(window == NULL){
    std::cout <<"Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
   }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Load GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //build and compile our shader program
    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //fragment shader
    unsigned int fragmentShaderO = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderY = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderO, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderO);
    glShaderSource(fragmentShaderY, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderY);
    
    //link shaders
    unsigned int shaderProgramB = glCreateProgram();
    unsigned int shaderProgramY = glCreateProgram();
    glAttachShader(shaderProgramB, vertexShader);
    glAttachShader(shaderProgramB, fragmentShaderO);
    glLinkProgram(shaderProgramB);
    glAttachShader(shaderProgramY, vertexShader);
    glAttachShader(shaderProgramY, fragmentShaderY);
    glLinkProgram(shaderProgramY);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderO);
    glDeleteShader(fragmentShaderY);

    //set up vertex data (and buffer(s)) and configure vertex attributes
    float leftTriangle[] = {
        //left triangle
        -0.5f, 0.5f, 0.0f, //top
        -1.0f, -0.5f, 0.0f, //bottom left
         0.0f, -0.5f, 0.0f, //bottom right
    };
    float rightTriangle[] = {
         //right triangle
         0.5f, 0.5f, 0.0f, //top
         0.0f, -0.5f, 0.0f, //bottom left
         1.0f, -0.5f, 0.0f  //bottom right
    };

    unsigned int indices[] = {  //note that we start from 0!
        0, 1, 3,   //first triangle
        1, 2, 3    //second triangle
    };
    
    unsigned int VBO[2], VAO[2], EBO;
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangle), leftTriangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangle), rightTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //Render loop
    while(!glfwWindowShouldClose(window)){
        //Input
        processInput(window);

        //Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw our first triangle
        glUseProgram(shaderProgramB);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramY);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //glfw swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgramB);
    glDeleteProgram(shaderProgramY);

    //terminate GLFW
    glfwTerminate();
    return 0;
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    }
    