#pragma once
#include <iostream>
#include <glfw3.h>
#include <vector>
#include <cmath>
#include <string>
#include <Windows.h>

using namespace std;
const double pi = acos(-1);

struct vec3
{
    double x, y, z;

    vec3(double _x, double _y, double _z)
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

bool pressed = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        pressed = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        pressed = false;
}


vector<vec3> build_figure(double a, double scale) {
    vector<vec3> figure;

    // вычисление высоты от центра до вершины
    double height = sqrt(2.0 / 3.0) * a * scale;

    // верхняя и нижняя вершины
    vec3 top(0, 0, height);
    vec3 bottom(0, 0, -height);

    // четыре вершины квадрата
    for (int i = 0; i < 4; ++i) {
        double ang = pi / 2 * i;
        vec3 v(a * scale * cos(ang), a * scale * sin(ang), 0);
        figure.push_back(v);
    }

    // добавление верхней и нижней вершин
    figure.push_back(top);
    figure.push_back(bottom);

    return figure;
}

void draw_figure(vector <vec3>& figure) {
    int n = figure.size();
    glColor3d(1.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(3);

    // Отрисовка треугольников
    for (int i = 0; i < n - 2; ++i) {
        glBegin(GL_POLYGON);
        // Верхний треугольник
        glVertex3f(figure[i].x, figure[i].y, figure[i].z);
        glVertex3f(figure[n - 2].x, figure[n - 2].y, figure[n - 2].z);
        glVertex3f(figure[(i + 1) % (n - 2)].x, figure[(i + 1) % (n - 2)].y, figure[(i + 1) % (n - 2)].z);
        glEnd();

        glBegin(GL_POLYGON);
        // Нижний треугольник
        glVertex3f(figure[i].x, figure[i].y, figure[i].z);
        glVertex3f(figure[(i + 1) % (n - 2)].x, figure[(i + 1) % (n - 2)].y, figure[(i + 1) % (n - 2)].z);
        glVertex3f(figure[n - 1].x, figure[n - 1].y, figure[n - 1].z);
        glEnd();
    }
}


int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(640, 480, "LR2", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glClearColor(0.94, 0.94, 0.94, 1.0);

    vector <vec3> figure;
    double a = 0, h = 0, scale = 0;

    HWND hwnd = GetForegroundWindow();
    HWND a_label = CreateWindow(L"Static", L"A=", WS_CHILD | WS_VISIBLE, 0, 0, 25, 25, hwnd, 0, 0, 0);
    HWND a_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 25, 0, 60, 20, hwnd, 0, 0, 0);

    glEnable(GL_CULL_FACE);
    double posx, posy;
    vec3 axis1 = vec3(1, 0, 0);
    vec3 axis2 = vec3(0, -1, 0);
    vec3 rot_axis = vec3(1, 0, 0);
    while (!glfwWindowShouldClose(window))
    {
        int W, H;
        glfwGetFramebufferSize(window, &W, &H);
        glViewport(0, 0, W, H);
        double ratio = (double)W / H;

        char str_a[24], str_h[24];
        GetWindowTextA(a_input, (LPSTR)str_a, 10);
        double na = atof(str_a);
        if (abs(na - a) > 0.0001)
        {
            a = na;
            //scale = 2 / sqrt(4 * a * a);
            scale = (double) (1) / 20;
            figure = build_figure(a, scale);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (ratio > 1.0)
            glOrtho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0);
        else
            glOrtho(-1.0, 1.0, -1.0 / ratio, 1.0 / ratio, 1.0, -1.0);

        double alpha = 0;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (pressed)
        {
            if (posx < 0)
            {
                alpha = 0;
                glfwGetCursorPos(window, &posx, &posy);
            }
            else
            {
                double nposx, nposy;
                glfwGetCursorPos(window, &nposx, &nposy);
                double dx = (nposx - posx) / W;
                double dy = (nposy - posy) / H;
                rot_axis = dx * axis2 - dy * axis1;
                alpha = 100 * sqrt(dx * dx + dy * dy);
                posx = nposx;
                posy = nposy;
            }
        }
        else
        {
            posx = -1;
            posy = -1;
        }
        glRotatef(alpha, rot_axis.x, rot_axis.y, rot_axis.z);
        draw_figure(figure);
        float m[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        for (int i = 0; i < figure.size(); i++)
            figure[i] = mult(m, figure[i]);

        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(10);
    }

    glfwTerminate();
    return 0;
}
