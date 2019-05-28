#pragma once

#include "Application.h"
#include "Shader.h"

class CurveApplication : public Application {
public:
    CurveApplication();
    ~CurveApplication();
    virtual void update();
    virtual void inputMouse(GLFWwindow* window, int button, int action, int mods);
    virtual void inputMouse(GLFWwindow* window, float x, float y);

private:
    void render();

    void bezierCurve();
    float bernstein(int i, int n, float t);
    long long factorial[21];

    Shader shader;

    float xPos = 0, yPos = 0;
    float points[40] = {0};
    int pointSize = 0;
    float curvePoints[10001];
    int curvePointSize = 0;
    float dynamicPoints[40] = {0};
    int dynamicPointSize = 0;

    bool withLine = false;
    bool dynamicDraw = false;
    float time = 0.0f;
};
