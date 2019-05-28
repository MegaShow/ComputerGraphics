#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "CurveApplication.h"

CurveApplication::CurveApplication()
    : shader("Shader/Vec2PosWithUniColor.vs.glsl", "Shader/Vec3Color.fs.glsl") {
    
    factorial[0] = 1;
    for (int i = 1; i < 21; i++) {
        factorial[i] = factorial[i - 1] * i;
    }
}

CurveApplication::~CurveApplication() {}

void CurveApplication::update() {
    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Point Size: %d (Max 20)", pointSize);
    ImGui::Text("Curve Point Size: %d", curvePointSize);
    ImGui::Checkbox("With Line (Non-Dynamic Draw)", &withLine);
    ImGui::Checkbox("Dynamic Draw", &dynamicDraw);
    ImGui::End();
    render();
}

void CurveApplication::inputMouse(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (pointSize < 20) {
            points[pointSize * 2] = xPos;
            points[pointSize * 2 + 1] = yPos;
            pointSize++;
            bezierCurve();
        }
    }
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (pointSize > 0) {
            pointSize--;
            bezierCurve();
        }
    }
}

void CurveApplication::inputMouse(GLFWwindow* window, float x, float y) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    xPos = x / width * 2 - 1.0f;
    yPos = 1.0f - y / height * 2;
}

void CurveApplication::render() {
    shader.use();
    // Curve
    if (curvePointSize >= 1) {
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
        shader.setGlmVec3("uniColor", color);
        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * curvePointSize * 2, curvePoints, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, ( void*) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glPointSize(5);
        glDrawArrays(GL_POINTS, 0, curvePointSize);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    // Line
    if (pointSize > 2 && dynamicDraw) {
        time += 0.01f;
        if (time >= 1.0f) {
            time = 0.0f;
        }
        dynamicPointSize = pointSize;
        for (int i = 0; i < pointSize * 2; i++) {
            dynamicPoints[i] = points[i];
        }
        for (bool flag = true; flag; ) {
            int prev = dynamicPointSize;
            dynamicPointSize = 0;
            for (int i = 1; i < prev; i++) {
                dynamicPoints[dynamicPointSize * 2] = dynamicPoints[i * 2 - 2] * (1 - time) + dynamicPoints[i * 2] * time;
                dynamicPoints[dynamicPointSize * 2 + 1] = dynamicPoints[i * 2 - 1] * (1 - time) + dynamicPoints[i * 2 + 1] * time;
                dynamicPointSize++;
            }
            if (dynamicPointSize == 2) {
                dynamicPoints[dynamicPointSize * 2] = dynamicPoints[0] * (1 - time) + dynamicPoints[2] * time;
                dynamicPoints[dynamicPointSize * 2 + 1] = dynamicPoints[1] * (1 - time) + dynamicPoints[3] * time;
                dynamicPointSize++;
                flag = false;
            }
            unsigned int VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dynamicPointSize * 2, dynamicPoints, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, ( void*) 0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glPointSize(6);
            glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f);
            shader.setGlmVec3("uniColor", color);
            if (flag) {
                glDrawArrays(GL_POINTS, 0, dynamicPointSize);
            } else {
                glDrawArrays(GL_POINTS, 0, dynamicPointSize - 1);
                glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);
                shader.setGlmVec3("uniColor", color);
                glPointSize(8);
                glDrawArrays(GL_POINTS, dynamicPointSize - 1, 1);
            }
            if (pointSize > 2) {
                glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
                shader.setGlmVec3("uniColor", color);
                glDrawArrays(GL_LINE_STRIP, 0, dynamicPointSize);
            }
            glBindVertexArray(0);

            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    }
    // Point
    if (pointSize >= 1) {
        glm::vec3 color(1.0f, 0.0f, 0.0f);
        shader.setGlmVec3("uniColor", color);
        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointSize * 2, points, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, ( void*) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glPointSize(8);
        glDrawArrays(GL_POINTS, 0, pointSize);
        if (pointSize > 2 && (withLine || dynamicDraw)) {
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader.setGlmVec3("uniColor", color);
            glDrawArrays(GL_LINE_STRIP, 0, pointSize);
        }
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void CurveApplication::bezierCurve() {
    curvePointSize = 0;
    if (pointSize <= 1) {
        return;
    }
    float step = 0.0f;
    for (int i = 1; i < pointSize; i++) {
        step += std::sqrt(std::pow(points[i * 2] - points[i * 2 - 2], 2) + std::pow(points[i * 2 + 1] - points[i * 2 - 1], 2));
    }
    step = 1.0f / (step * 500);
    if (step < 0.0001f) {
        step = 0.0001f;
    }
    for (float t = 0.0f; t < 1.0f; t += step) {
        float x = 0.0f, y = 0.0f;
        for (int i = 0; i < pointSize; i++) {
            float b = bernstein(i, pointSize - 1, t);
            x += b * points[i * 2];
            y += b * points[i * 2 + 1];
        }
        curvePoints[curvePointSize * 2] = x;
        curvePoints[curvePointSize * 2 + 1] = y;
        curvePointSize++;
    }
}

float CurveApplication::bernstein(int i, int n, float t) {
    return factorial[n] / factorial[i] / factorial[n - i] * std::pow(t, i) * std::pow(1 - t, n - i);
}
