#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application {
public:
    virtual void update() = 0;

    virtual void input(GLFWwindow* window) {}
    virtual void input(GLFWwindow* window, float xOffset, float yOffset) {}
    virtual void input(GLFWwindow* window, float zOffset) {}
    virtual void inputMouse(GLFWwindow* window, int button, int action, int mods) {}
    virtual void inputMouse(GLFWwindow* window, float x, float y) {}
};
