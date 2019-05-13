#pragma once

#include "Application.h"
#include "Shader.h"

class ShadowApplication : public Application {
public:
    ShadowApplication();
    ~ShadowApplication();
    virtual void update();

private:
    void render();
    void renderScene(Shader& shader);

    Shader depthShader, sceneShader;

    unsigned int VAOs[2], VBOs[2], FBO, depthMap;

    int projectionType = 0;
    float nearPlane = 1.0f, farPlane = 10.0f;
    float lightPos[3] = { 1.0f, 4.0f, -4.0f };
    float cameraPos[3] = { 0.0f, 6.0f, 6.0f };

    const unsigned int SHADOW_WIDTH = 10240, SHADOW_HEIGHT = 10240;
    const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

    const float planeVertices[36] = {
        // Positions           // Normals
         25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f,

         25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f,
         25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f
    };

    float vertices[216] = {
        -2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
         2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
         2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
         2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
        -2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
        -2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,

        -2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
         2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
         2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
         2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
        -2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
        -2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,

        -2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
        -2.0f,  2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
        -2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
        -2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
        -2.0f, -2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
        -2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,

         2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
         2.0f,  2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
         2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
         2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
         2.0f, -2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
         2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,

        -2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
         2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
         2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
         2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
        -2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
        -2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,

        -2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
         2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
         2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
         2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
        -2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
        -2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f
    };
};
