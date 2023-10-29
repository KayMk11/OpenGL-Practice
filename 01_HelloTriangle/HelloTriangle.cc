#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout(location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 fragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   fragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                   "}\0";

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    // glfw window creation
    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

    // if window creation fails then exit
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // make window the current opengl context
    glfwMakeContextCurrent(window);

    // tell glad to load opengl
    gladLoadGL();

    // set window dimensions as view port
    glViewport(0, 0, 640, 480);

    unsigned int vertexShader, fragmentShader;

    // create shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "RROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // create fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "RROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // create shader program
    unsigned int shaderProgram = glCreateProgram();

    // attach vertex and fragment shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // link shader
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete shaders once linking is done
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    unsigned int VAO, VBO;
    // Generate VAO
    glGenVertexArrays(1, &VAO);
    // Generate VBO
    glGenBuffers(1, &VBO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Assign data to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Tell opengl how to read the VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // once opengl knows how to read VBO, enable it so that opengl uses it
    glEnableVertexAttribArray(0);

    // unbind VBO and VAO so that we dont mistakenly make changes to it;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // tell opengl to use our shader program
        glUseProgram(shaderProgram);

        // bind VAO for drawing
        glBindVertexArray(VAO);

        // tell opengl to draw bound VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}