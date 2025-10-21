//
// Created by xuyz on 2018/11/7.
//
#include <stdio.h>
#define GLEW_STATIC
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//*  @param[in] window The window that received the event.
//*  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
//*  @param[in] scancode The system-specific scancode of the key.
//*  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
//*  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
//        *  held down.
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

GLint Width = 600, Height = 400;

auto vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main(){\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1);\n"
        "}";
auto fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main(){\n"
        "color=vec4(1.0, 0.5, 0.2, 1.0);\n"
        "}";

auto fragmentShaderSource2 =
        "#version 330 core \n"
        "out vec4 color;\n"
        "void main(){\n"
        "color = vec4(0.2, 1.0, 0.5, 1.0);\n"
        "}";

GLuint createShader(const GLenum shaderType, const GLchar *source) {
    const GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    GLchar info[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
        printf("create shader info log: %s\n", info);
        return 0;
    }
    return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    GLchar info[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(info), nullptr, info);
        printf("createShaderProgram error: %s\n", info);
        return 0;
    }
    return program;
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(Width, Height, "hello-triangle", nullptr, nullptr);
    if (window == nullptr) {
        const char *str;
        glfwGetError(&str);
        printf("glfwCreateWindow failed:%s\n", str);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    const GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("glew init error:%s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    GLuint uiVertexArrayObject;
    glGenVertexArrays(1, &uiVertexArrayObject);
    std::cout << "uiVertexArrayObject = " << uiVertexArrayObject << std::endl;

    // GLFWwindow *window = glfwCreateWindow(Width, Height, "hello-triangle", nullptr, nullptr);
    // if (window == nullptr) {
    //     const char *str;
    //     glfwGetError(&str);
    //     printf("glfwCreateWindow failed:%s\n", str);
    //     glfwTerminate();
    //     return -1;
    // }
    // glfwMakeContextCurrent(window);

    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(window, key_callback);

    GLint success;
    GLchar info[512];
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint fragmentShader2 = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource2);
    GLuint shaderProgram1 = createShaderProgram(vertexShader, fragmentShader);
    GLuint shaderProgram2 = createShaderProgram(vertexShader, fragmentShader2);
    //    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //    glShaderSource(vertexShader,1,&vertexShaderSource, nullptr);
    //    glCompileShader(vertexShader);
    //    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //    if(!success){
    //        glGetShaderInfoLog(vertexShader, sizeof(info), nullptr, info);
    //        printf("GL_VERTEX_SHADER shader info log:%s\n", info);
    //        return -1;
    //    }

    //    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //    glShaderSource(fragmentShader,1,&fragmentShaderSource, nullptr);
    //    glCompileShader(fragmentShader);
    //    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //    if(!success){
    //        glGetShaderInfoLog(fragmentShader, sizeof(info), nullptr, info);
    //        printf("GL_FRAGMENT_SHADER shader info log:%s\n", info);
    //        return -1;
    //    }

    //    // 着色器程序
    //    GLuint shaderProgram=glCreateProgram();
    //    glAttachShader(shaderProgram, vertexShader);
    //    glAttachShader(shaderProgram, fragmentShader2);
    //    glLinkProgram(shaderProgram);
    //    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //    if (!success){
    //        glGetProgramInfoLog(shaderProgram, sizeof(info), nullptr, info);
    //        printf("GL_LINK_STATUS: %s\n", info);
    //        return -1;
    //    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //vertexes
    GLfloat vertexes[] = {
        0.5, 0.5, 1.0,
        -0.5, 0.5, 1.0,
        -0.5, -0.5, 1.0,
        0.5, -0.5, 1.0,
        //            0,0.5,0.9
    };
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // VAO{VBO}
    GLuint VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO);
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLuint *) 0) + 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, const int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else {
        GLuint uiVertexArrayObject;
        glGenVertexArrays(1, &uiVertexArrayObject);
        std::cout << "uiVertexArrayObject = " << uiVertexArrayObject << std::endl;
        // glDeleteVertexArrays(1, &uiVertexArrayObject);
    }
}
