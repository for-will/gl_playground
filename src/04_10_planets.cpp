#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "camera.h"
#include "model.h"

#include <iostream>
#include <map>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

unsigned int loadTexture(const char *path);

unsigned int loadCubemap(const std::vector<std::string> &faces);

void setShaderLight(const Shader &shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)
};

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float) SCR_WIDTH / 2.0;
float lastY = (float) SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    glDepthFunc(GL_LESS);

    // glEnable(GL_STENCIL_TEST);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);

    // build and compile shaders
    // -------------------------
    Shader modelShader("../shader/4.10.planets.vert",
                       "../shader/4.10.planets.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // load models
    // -----------
    Model ourModel("../assets/nanosuit_reflection/nanosuit.obj");

    // load textures
    // -------------
    // unsigned int cubeTexture = loadTexture("../assets/container2.png");
    // unsigned int cubeTexture = loadTexture("../assets/ss.jpg");
    // unsigned int floorTexture = loadTexture("../assets/metal.png");
    // unsigned int grassTexture = loadTexture("../assets/grass.png");
    //
    // std::vector<std::string> faces{
    //     "right.jpg",
    //     "left.jpg",
    //     "top.jpg",
    //     "bottom.jpg",
    //     "front.jpg",
    //     "back.jpg"
    // };
    // unsigned int cubemapTexture = loadCubemap(faces);


    // shader configuration
    // --------------------
    // shader.use();
    // shader.setInt("texture1", 0);

    // std::vector<glm::vec3> windows;
    // windows.emplace_back(-1.5f, 0.0f, -0.48f);
    // windows.emplace_back(1.5f, 0.0f, 0.51f);
    // windows.emplace_back(0.0f, 0.0f, 0.7f);
    // windows.emplace_back(-0.3f, 0.0f, -2.3f);
    // windows.emplace_back(0.5f, 0.0f, -0.6f);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // // skybox
        // glDepthMask(GL_FALSE);
        // glDisable(GL_CULL_FACE);
        // skyboxShader.use();
        // glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
        //                                         (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        // skyboxShader.setMat4("view", view);
        // skyboxShader.setMat4("projection", projection);
        // skyboxShader.setInt("skyboxTexture", 2);
        // glBindVertexArray(skyboxVAO);
        // glActiveTexture(GL_TEXTURE2);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDepthMask(GL_TRUE);
        //
        // shader.use();
        // view = camera.GetViewMatrix();
        // projection = glm::perspective(glm::radians(camera.Zoom),
        //                               (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        // shader.setMat4("view", view);
        // shader.setMat4("projection", projection);

        // floor
        // glEnable(GL_CULL_FACE);
        // // glStencilMask(0x00);
        // glBindVertexArray(planeVAO);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, floorTexture);
        // shader.setInt("texture1", 1);
        // shader.setMat4("model", glm::mat4(1.0f));
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0);

        // cubes
        // reflectShader.use();
        // reflectShader.setMat4("view", view);
        // reflectShader.setMat4("projection", projection);
        // reflectShader.setVec3("cameraPos", camera.Position);
        // glDisable(GL_CULL_FACE);
        // glBindVertexArray(cubeVAO);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // reflectShader.setInt("skybox", 1);
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        // reflectShader.setMat4("model", model);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        // reflectShader.setMat4("model", model);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // std::map<float, glm::vec3> sorted;
        // for (unsigned int i = 0; i < windows.size(); i++) {
        //     float distance = glm::length(camera.Position - windows[i]);
        //     sorted[distance] = windows[i];
        // }
        // glDisable(GL_CULL_FACE);
        // glBindVertexArray(vegetationVAO);
        // glBindTexture(GL_TEXTURE_2D, grassTexture);
        // for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, it->second);
        //     shader.setMat4("model", model);
        //     glDrawArrays(GL_TRIANGLES, 0, 6);
        // }


        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glClearColor(0.2, 0.3, 0.3, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT);
        // glDisable(GL_DEPTH_TEST);

        // render the loaded model
        modelShader.use();
        setShaderLight(modelShader);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                      (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);
        // modelShader.setFloat("time", glfwGetTime());
        ourModel.Draw(modelShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &cubeVAO);
    // glDeleteVertexArrays(1, &planeVAO);
    // glDeleteBuffers(1, &cubeVBO);
    // glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = GL_STENCIL;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);


        if (format == GL_RGBA) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(const std::vector<std::string> &faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(("../assets/skybox/" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

unsigned int createFrameBuffer(GLFWwindow *window) {
    // 渲染到纹理
    // ---------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    int width = SCR_WIDTH;
    int height = SCR_HEIGHT;
    glfwGetFramebufferSize(window, &width, &height);

    // 生成纹理
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 将它附加到当前绑定的帧缓冲对象
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // 创建一个渲染缓冲对象用于深度和模板测试
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // 解绑

    // 将渲染缓冲对象附加到帧缓冲的深度和模板附件上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // 检查帧缓冲是否是完整的
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return framebuffer;
}

void setShaderLight(const Shader &shader) {
    // be sure to activate shader when setting uniforms/drawing objects
    shader.use();
    shader.setVec3("viewPos", camera.Position);
    // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)

    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
    shader.setVec3("dirLight.diffuse", glm::vec3(0.2f));
    shader.setVec3("dirLight.specular", glm::vec3(0.2f));

    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setFloat("spotLight.cutOff", cos(glm::radians(10.5f)));
    shader.setFloat("spotLight.outerCutOff", cos(glm::radians(12.5f)));
    shader.setVec3("spotLight.ambient", glm::vec3(0.0f));
    shader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
    shader.setVec3("spotLight.specular", glm::vec3(1.0f));
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.045f);
    shader.setFloat("spotLight.quadratic", 0.0075f);

    for (int i = 0; i < std::size(pointLightPositions); i++) {
        char name[128];
        snprintf(name, sizeof(name), "pointLights[%d]", i);
        // point light 4
        shader.setVec3(std::string(name) + ".position", pointLightPositions[i]);
        shader.setVec3(std::string(name) + ".ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3(std::string(name) + ".diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3(std::string(name) + ".specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat(std::string(name) + ".constant", 1.0f);
        shader.setFloat(std::string(name) + ".linear", 0.09f);
        shader.setFloat(std::string(name) + ".quadratic", 0.032f);
    }
}
