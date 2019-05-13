#include "ShadowApplication.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

ShadowApplication::ShadowApplication()
    : depthShader("Shader/Vec3PosVec3ColorWithUniProj.vs.glsl", "Shader/Empty.fs.glsl"),
    sceneShader("Shader/Shadow.vs.glsl", "Shader/Shadow.fs.glsl") {

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowApplication::~ShadowApplication() {
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteFramebuffers(1, &FBO);
}

void ShadowApplication::update() {
    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::RadioButton("Orthographic Projection", &projectionType, 0);
    ImGui::RadioButton("Perspective Projection", &projectionType, 1);
    ImGui::SliderFloat("Near Plane", &nearPlane, 0.1f, 20.0f);
    ImGui::SliderFloat("Far Plane", &farPlane, 0.1f, 20.0f);
    ImGui::SliderFloat3("Camera Position", cameraPos, -10.0f, 10.0f);
    ImGui::SliderFloat3("Light Position", lightPos, -10.0f, 10.0f);
    ImGui::End();
    render();
}

void ShadowApplication::render() {
    glEnable(GL_DEPTH_TEST);

    glm::vec3 lightPos3 = glm::vec3(lightPos[0], lightPos[1], lightPos[2]);
    glm::vec3 cameraPos3 = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]);

    glm::mat4 projection;
    if (projectionType == 0) {
        projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    } else if (projectionType == 1) {
        projection = glm::perspective((float) glm::radians(45.0f), 1.0f, nearPlane, farPlane);
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos3);
    glm::mat4 view = glm::lookAt(lightPos3, glm::vec3(0.0f), glm::vec3(1.0f));
    glm::mat4 lightSpaceMatrix = projection * view;
    
    // Render depth map
    depthShader.use();
    depthShader.setGlmMat4("view", view);
    depthShader.setGlmMat4("projection", projection);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    renderScene(depthShader);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Render scene
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sceneShader.use();
    projection = glm::perspective((float) glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(cameraPos3, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    sceneShader.setGlmMat4("view", view);
    sceneShader.setGlmMat4("projection", projection);
    sceneShader.setGlmVec3("lightPos", lightPos3);
    sceneShader.setGlmVec3("viewPos", cameraPos3);
    sceneShader.setGlmMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    sceneShader.setInt("shadowMap", 0);
    renderScene(sceneShader);

    glDisable(GL_DEPTH_TEST);
}

void ShadowApplication::renderScene(Shader& shader) {
    // Plane
    glm::mat4 model = glm::mat4(1.0f);
    shader.setGlmMat4("model", model);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // Cube
    model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
    shader.setGlmMat4("model", model);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
