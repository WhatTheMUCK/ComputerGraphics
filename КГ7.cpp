#include <glfw3.h>
#include <cmath>
#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>

const double pi = acos(-1);
const double eps = 0.0001;

struct vec3
{
    double x, y, z;

    vec3(double _x = 0, double _y = 0, double _z = 0)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

vec3 operator+(vec3 a, vec3 b)
{
    vec3 res = a;
    res.x += b.x;
    res.y += b.y;
    res.z += b.z;
    return res;
}

vec3 operator-(vec3 a, vec3 b)
{
    vec3 res = a;
    res.x -= b.x;
    res.y -= b.y;
    res.z -= b.z;
    return res;
}

vec3 operator*(vec3 a, vec3 b)
{
    vec3 res = vec3(0, 0, 0);
    res.x = a.y * b.z - a.z * b.y;
    res.y = -a.x * b.z + a.z * b.x;
    res.z = a.x * b.y - a.y * b.x;
    return res;
}

vec3 operator*(double a, vec3 b)
{
    vec3 res = vec3(0, 0, 0);
    res.x = a * b.x;
    res.y = a * b.y;
    res.z = a * b.z;
    return res;
}

vec3 mult(float* m, vec3 v)
{
    vec3 res = vec3(0, 0, 0);
    res.x = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
    res.y = m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7];
    res.z = m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11];
    return res;
}

struct SplinePoint {
    vec3 position;
    vec3 tangent;
};

struct CubicSpline {
    SplinePoint p1, p2;
};

vec3 CalculateSplinePoint(const CubicSpline& spline, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    vec3 p = (2 * t3 - 3 * t2 + 1) * spline.p1.position
        + (t3 - 2 * t2 + t) * spline.p1.tangent
        + (-2 * t3 + 3 * t2) * spline.p2.position
        + (t3 - t2) * spline.p2.tangent;
    return p;
}

void DrawSpline(const CubicSpline& spline) {
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1; t += 0.01) {
        vec3 point = CalculateSplinePoint(spline, t);
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

bool mousePressed = false;
double lastX = 0, lastY = 0;
double offsetX = 0, offsetY = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mousePressed = (action == GLFW_PRESS);
        if (mousePressed) {
            glfwGetCursorPos(window, &lastX, &lastY);
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        double dx = xpos - lastX;
        double dy = ypos - lastY;
        offsetX += dx / 100; // Скорость перемещения
        offsetY -= dy / 100; // Скорость перемещения

        lastX = xpos;
        lastY = ypos;
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "LR-7", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glClearColor(0, 0, 0, 1.0);

    HWND hwnd = GetForegroundWindow();
    HWND background = CreateWindow(L"Static", L"", WS_CHILD | WS_VISIBLE, 0, 0, 230, 160, hwnd, 0, 0, 0);
    HWND x1_label = CreateWindow(L"Static", L"x1 = ", WS_CHILD | WS_VISIBLE, 10, 10, 30, 20, hwnd, 0, 0, 0);
    HWND x1_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 40, 10, 60, 20, hwnd, 0, 0, 0);
    HWND y1_label = CreateWindow(L"Static", L"y1 = ", WS_CHILD | WS_VISIBLE,110, 10, 30, 20, hwnd, 0, 0, 0);
    HWND y1_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 10, 60, 20, hwnd, 0, 0, 0);

    HWND tangentx1_label = CreateWindow(L"Static", L"tangentx1 = ", WS_CHILD | WS_VISIBLE, 10, 35, 80, 20, hwnd, 0, 0, 0);
    HWND tangentx1_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 110, 35, 60, 20, hwnd, 0, 0, 0);
    HWND tangenty1_label = CreateWindow(L"Static", L"tangenty1 = ", WS_CHILD | WS_VISIBLE, 10, 60, 80, 20, hwnd, 0, 0, 0);
    HWND tangenty1_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 110, 60, 60, 20, hwnd, 0, 0, 0);

    HWND x2_label = CreateWindow(L"Static", L"x2 = ", WS_CHILD | WS_VISIBLE, 10, 85, 30, 20, hwnd, 0, 0, 0);
    HWND x2_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 40, 85, 60, 20, hwnd, 0, 0, 0);
    HWND y2_label = CreateWindow(L"Static", L"y2 = ", WS_CHILD | WS_VISIBLE, 110, 85, 30, 20, hwnd, 0, 0, 0);
    HWND y2_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 85, 60, 20, hwnd, 0, 0, 0);

    HWND tangentx2_label = CreateWindow(L"Static", L"tangentx2 = ", WS_CHILD | WS_VISIBLE, 10, 110, 80, 20, hwnd, 0, 0, 0);
    HWND tangentx2_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 110, 110, 60, 20, hwnd, 0, 0, 0);
    HWND tangenty2_label = CreateWindow(L"Static", L"tangenty2 = ", WS_CHILD | WS_VISIBLE, 10, 135, 80, 20, hwnd, 0, 0, 0);
    HWND tangenty2_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 110, 135, 60, 20, hwnd, 0, 0, 0);

    SetWindowText(x1_input, L"-0.5");
    SetWindowText(y1_input, L"0");
    SetWindowText(tangentx1_input, L"1");
    SetWindowText(tangenty1_input, L"1");
    SetWindowText(x2_input, L"0.5");
    SetWindowText(y2_input, L"0");
    SetWindowText(tangentx2_input, L"-1");
    SetWindowText(tangenty2_input, L"1");
    //CubicSpline spline = {
    //    {{-0.5, 0, 0}, {1, 1, 0}}, // p1: position and tangent
    //    {{0.5, 0, 0}, {-1, 1, 0}}  // p2: position and tangent
    //};
    CubicSpline spline;
    double x1 = 0, y1 = 0, tangentx1 = 0, tangenty1 = 0, x2 = 0, y2 = 0, tangentx2 = 0, tangenty2 = 0;

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-width / 200.0 + offsetX, width / 200.0 + offsetX, -height / 200.0 + offsetY, height / 200.0 + offsetY, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT);

        char str_x1[24], str_y1[24], str_tangentx1[24], str_tangenty1[24], str_x2[24], str_y2[24], str_tangentx2[24], str_tangenty2[24];
        GetWindowTextA(x1_input, (LPSTR)str_x1, 10);
        GetWindowTextA(y1_input, (LPSTR)str_y1, 10);
        GetWindowTextA(tangentx1_input, (LPSTR)str_tangentx1, 10);
        GetWindowTextA(tangenty1_input, (LPSTR)str_tangenty1, 10);
        GetWindowTextA(x2_input, (LPSTR)str_x2, 10);
        GetWindowTextA(y2_input, (LPSTR)str_y2, 10);
        GetWindowTextA(tangentx2_input, (LPSTR)str_tangentx2, 10);
        GetWindowTextA(tangenty2_input, (LPSTR)str_tangenty2, 10);
        double x1n = atof(str_x1);
        double y1n = atof(str_y1);
        double tangentx1n = atof(str_tangentx1);
        double tangenty1n = atof(str_tangenty1);
        double x2n = atof(str_x2);
        double y2n = atof(str_y2);
        double tangentx2n = atof(str_tangentx2);
        double tangenty2n = atof(str_tangenty2);
        if (abs(x1n - x1) > eps || abs(y1n - y1) || abs(tangentx1n - tangentx1) > eps || abs(tangenty1n - tangenty1) > eps ||
            abs(x2n - x2) > eps || abs(y2n - y2) || abs(tangentx2n - tangentx2) > eps || abs(tangenty2n - tangenty2) > eps)
        {
            x1 = x1n;
            y1 = y1n;
            tangentx1 = tangentx1n;
            tangenty1 = tangenty1n;
            x2 = x2n;
            y2 = y2n;
            tangentx2 = tangentx2n;
            tangenty2 = tangenty2n;
            spline = { {{x1, y1, 0}, {tangentx1, tangenty1, 0}}, {{x2, y2, 0}, {tangentx2, tangenty2, 0}} };
        }
        DrawSpline(spline);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
