#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <ostream>

#include "camera.h"
#include "shader_s.h"
#include "texture_s.h"


// stores how much we're seeing of either texture
float mixValue = 0.2f;

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间


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

// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


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
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const auto lightingShader = Shader("../shader/projection.vert", "../shader/light_object.frag");
    const auto lightShader = Shader("../shader/projection.vert", "../shader/lamp.frag");


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

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);


    // 创建纹理
    loadTexture("../assets/container.jpg",GL_RGB,GL_TEXTURE1);
    loadTexture("../assets/awesomeface.png", GL_RGBA,GL_TEXTURE2);

    lightingShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(lightingShader.ID, "texture1"), 1); // 手动设置
    lightingShader.setInt("texture2", 2); // 或者使用着色器类设置

    const auto projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
    lightingShader.setMat4("projection", projection);

    constexpr glm::vec3 lightPos(1.2f, 1.0f, 0.0f);
    auto model = glm::mat4();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", camera.GetViewMatrix());
    lightShader.setInt("texture1", 1);
    lightShader.setInt("texture2", 2);


    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        const float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 输入
        processInput(window);

        // 渲染
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 记得激活着色器
        lightingShader.use();
        lightingShader.setFloat("mixValue", mixValue);

        // model = glm::rotate(glm::mat4(), (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 0.5f, 0.0f));
        // auto modelLoc = glGetUniformLocation(ourShader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //
        //

        // auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        lightingShader.setMat4("view", camera.GetViewMatrix());
        const auto projection = glm::perspective(glm::radians(camera.Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // 绘制三角形
        glBindVertexArray(VAO);
        const auto &ps = glm::vec3(0.0f, 0.2f, 0.0f);
        auto model = glm::translate(glm::mat4(), ps);
        // float angle = 20.0f;
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, static_cast<const void *>(nullptr));
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        lightShader.use();
        // lightShader.setFloat("mixValue", mixValue);
        lightShader.setMat4("view", camera.GetViewMatrix());
        lightShader.setMat4("projection", projection);
        // constexpr glm::vec3 lightPos(1.2f, 0.6f, 0.0f);
        // auto lightModel = glm::mat4();
        // lightModel = glm::translate(lightModel, lightPos);
        // lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        // lightShader.setMat4("model", lightModel);
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, static_cast<const void *>(nullptr));

        // ourShader.use();
        // ourShader.setFloat("mixValue", mixValue);
        // ourShader.setMat4("model", model);
        // ourShader.setMat4("view", camera.GetViewMatrix());
        // ourShader.setMat4("projection", projection);
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, static_cast<const void *>(nullptr));

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

    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // cameraPos += cameraSpeed * cameraFront;
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    static bool firstMouse = true;
    static double lastX = 400, lastY = 300;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // float sensitivity = 0.05;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;
    camera.ProcessMouseMovement(xoffset, yoffset);

    // yaw += xoffset;
    // pitch += yoffset;
    //
    // if (pitch > 89.0f)
    //     pitch = 89.0f;
    // if (pitch < -89.0f)
    //     pitch = -89.0f;

    // glm::vec3 front;
    // front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // front.y = sin(glm::radians(pitch));
    // front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // fov -= static_cast<float>(yoffset);
    // if (fov < 1.0f)
    //     fov = 1.0f;
    // if (fov > 65.0f)
    //     fov = 65.0f;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
