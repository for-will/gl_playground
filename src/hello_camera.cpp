#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <ostream>

#include "shader_s.h"
#include "texture_s.h"


// stores how much we're seeing of either texture
float mixValue = 0.2f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window);

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f


constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // B1
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // B2
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // B3
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // B4

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // T1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // T2
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // T3
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, // T4

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // T1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // T2
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // B2
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // B1

    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // T2
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // T3
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // B3
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // B2

    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // T4
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // T3
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // B3
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // B4

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // T1
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // T4
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // B4
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // B1

};
constexpr GLubyte indices[] = {
    0, 1, 2, // first triangle
    0, 2, 3, // second triangle

    4, 5, 6,
    4, 6, 7,

    8, 9, 10,
    8, 10, 11,

    12, 13, 14,
    12, 14, 15,

    16, 17, 18,
    16, 18, 19,

    20, 21, 22,
    20, 22, 23,
};

// world space positions of our cubes
constexpr glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
};


int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const auto ourShader = Shader("../shader/projection.vert", "../shader/texture.frag");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // constexpr float vertices[] = {
    //     // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //     0.5f, 0.5f, 0.0f,/*****/1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
    //     0.5f, -0.5f, 0.0f,/****/0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
    //     -0.5f, -0.5f, 0.0f,/***/0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
    //     -0.5f, 0.5f, 0.0f,/****/1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 左上
    // };


    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // // 颜色属性
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // 创建纹理
    loadTexture("../assets/container.jpg",GL_RGB,GL_TEXTURE1);
    loadTexture("../assets/awesomeface.png", GL_RGBA,GL_TEXTURE2);

    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 1); // 手动设置
    ourShader.setInt("texture2", 2); // 或者使用着色器类设置

    // auto model = glm::rotate(glm::mat4(), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // const auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    const auto projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
    // ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    // const auto modelLoc = glGetUniformLocation(ourShader.ID, "model");
    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // const auto viewLoc = glGetUniformLocation(ourShader.ID, "view");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // const auto projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        // 输入
        processInput(window);

        // 渲染
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 记得激活着色器
        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);

        // model = glm::rotate(glm::mat4(), (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 0.5f, 0.0f));
        // auto modelLoc = glGetUniformLocation(ourShader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //
        //
        // // 绘制三角形
        // glBindVertexArray(VAO);
        // // glBindTexture(GL_TEXTURE_2D, texture2);
        // // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        constexpr float radius = 10.0f;
        float eyeX = sin(glfwGetTime()) * radius;
        float eyeZ = cos(glfwGetTime()) * radius;
        auto view = glm::lookAt(glm::vec3(eyeX, 0.0f, eyeZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("view", view);

        for (auto i = 0; i < std::size(cubePositions); i++) {
            const auto &ps = cubePositions[i];
            auto model = glm::translate(glm::mat4(), ps);
            float angle = 20.0f * i;
            if (i % 3 == 0) {
                angle += glfwGetTime();
            }

            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            // model = glm::translate(model, ps);
            ourShader.setMat4("model", model);
            glBindVertexArray(VAO);
            // glDrawArrays(GL_TRIANGLES, 0, 36);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, static_cast<const void *>(nullptr));
        }

        // 交换缓冲并查询IO事件
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        // glfwSetWindowShouldClose(window, true);
        mixValue += 0.001f;
        if (mixValue > 1.0f) {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        // glfwSetWindowShouldClose(window, true);
        mixValue -= 0.001f;
        if (mixValue < 0.0f) {
            mixValue = 0.0f;
        }
    }
}
