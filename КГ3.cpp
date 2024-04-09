#pragma once
#include <glfw3.h>
#include <vector>
#include <cmath>
#include <string>
#include <Windows.h>

using namespace std;
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

bool pressed = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        pressed = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        pressed = false;
}

// Функция для проверки, находится ли точка внутри эллипса
bool is_inside_ellipse(vec3 light, double a, double b) {
    return (light.x * light.x) / (a * a) + (light.y * light.y) / (b * b) <= 1;
}

vec3 get_cylinder_point(double theta, double y, double a, double b)
{
    vec3 res;
    //res.x = a * cos(theta); // a и b - размеры полуосей эллипса
    //res.y = b * sin(theta);
    //res.z = z;

    res.x = b * sin(theta); // a и b - размеры полуосей эллипса
    res.y = y;
    res.z = a * cos(theta);
    return res;
}


vector <vector <vec3>> build_figure(double a, double b, double da, double height)
{
    vector <vector <vec3>> figure;
    for (double y = -height / 2; y < height / 2; y += da)
    {
        for (double theta = 0; theta < 2 * pi; theta += da)
        {
            vector <vec3> poly;
            vector <vec3> poly2;
            double next_theta = min(2 * pi, theta + da);
            double next_y = min(height / 2, y + da);
            poly.push_back(get_cylinder_point(theta, y, a, b));
            poly.push_back(get_cylinder_point(next_theta, y, a, b));
            poly.push_back(get_cylinder_point(next_theta, next_y, a, b));
            poly.push_back(get_cylinder_point(theta, next_y, a, b));
            reverse(poly.begin(), poly.end());
            figure.push_back(poly);

            poly2.push_back(get_cylinder_point(theta, next_y, a, b));
            poly2.push_back(get_cylinder_point(next_theta, next_y, a, b));
            poly2.push_back(get_cylinder_point(next_theta, y, a, b));
            poly2.push_back(get_cylinder_point(theta, y, a, b));
            reverse(poly2.begin(), poly2.end());
            figure.push_back(poly2);
        }
    }
    return figure;
}



void draw_figure(vector <vector <vec3>>& figure, bool inside)
{
    glColor3d(0.6, 0.6, 0.6);
    for (int i = 0; i < figure.size(); i++)
    {
        glBegin(GL_POLYGON);
        vec3 norm = (figure[i][1] - figure[i][0]) * (figure[i][2] - figure[i][0]);
        if (inside) {
            glNormal3f(-norm.x, -norm.y, norm.z);
        }
        else {
            glNormal3f(norm.x, norm.y, norm.z);
        }
        int n = figure[i].size();
        for (int j = 0; j < n; j++)
            glVertex3f(figure[i][j].x, figure[i][j].y, figure[i][j].z);
        glEnd();
    }
}

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(900, 500, "LR4-5", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glClearColor(0, 0, 0, 1.0);

    HWND hwnd = GetForegroundWindow();
    HWND background = CreateWindow(L"Static", L"", WS_CHILD | WS_VISIBLE, 0, 0, 170, 170, hwnd, 0, 0, 0);
    HWND a1_label = CreateWindow(L"Static", L"a = ", WS_CHILD | WS_VISIBLE, 10, 10, 30, 20, hwnd, 0, 0, 0);
    HWND a1_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 40, 10, 60, 20, hwnd, 0, 0, 0);
    HWND a2_label = CreateWindow(L"Static", L"b = ", WS_CHILD | WS_VISIBLE, 10, 35, 30, 20, hwnd, 0, 0, 0);
    HWND a2_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 40, 35, 60, 20, hwnd, 0, 0, 0);
    HWND st_label = CreateWindow(L"Static", L"Step = ", WS_CHILD | WS_VISIBLE, 10, 60, 60, 20, hwnd, 0, 0, 0);
    HWND st_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 70, 60, 60, 20, hwnd, 0, 0, 0);
    HWND x_label = CreateWindow(L"Static", L"Light X =", WS_CHILD | WS_VISIBLE, 10, 85, 70, 20, hwnd, 0, 0, 0);
    HWND x_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 80, 85, 60, 20, hwnd, 0, 0, 0);
    HWND z_label = CreateWindow(L"Static", L"Light Y =", WS_CHILD | WS_VISIBLE, 10, 110, 70, 20, hwnd, 0, 0, 0);
    HWND z_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 80, 110, 60, 20, hwnd, 0, 0, 0);
    HWND y_label = CreateWindow(L"Static", L"Light Z =", WS_CHILD | WS_VISIBLE, 10, 135, 70, 20, hwnd, 0, 0, 0);
    HWND y_input = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 80, 135, 60, 20, hwnd, 0, 0, 0);

    SetWindowText(a1_input, L"0.3");
    SetWindowText(a2_input, L"0.4");
    SetWindowText(st_input, L"0.1");
    SetWindowText(x_input, L"1");
    SetWindowText(y_input, L"0");
    SetWindowText(z_input, L"1");

    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    double posx = 0, posy = 0;
    double step = 1;
    double a = 0, b = 0;
    vec3 axis1 = vec3(-1, 0, 0);
    vec3 axis2 = vec3(0, 1, 0);
    vec3 rot_axis = vec3(1, 0, 0);
    vec3 s_light = vec3(0, 0, 0);
    vec3 light = vec3(0, 0, 0);
    vector <vector <vec3>> figure;

    while (!glfwWindowShouldClose(window))
    {
        int W, H;
        glfwGetFramebufferSize(window, &W, &H);
        glViewport(0, 0, W, H);
        double ratio = (double)W / H;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (ratio > 1.0)
            glOrtho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0);
        else
            glOrtho(-1.0, 1.0, -1.0 / ratio, 1.0 / ratio, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        char str_a1[24], str_a2[24], str_st[24], str_x[24], str_y[24], str_z[24];
        GetWindowTextA(a1_input, (LPSTR)str_a1, 10);
        GetWindowTextA(a2_input, (LPSTR)str_a2, 10);
        GetWindowTextA(st_input, (LPSTR)str_st, 10);
        GetWindowTextA(x_input, (LPSTR)str_x, 10);
        GetWindowTextA(y_input, (LPSTR)str_y, 10);
        GetWindowTextA(z_input, (LPSTR)str_z, 10);
        double a1 = atof(str_a1);
        double a2 = atof(str_a2);
        double n_step = max(pi / 100, atof(str_st));
        double light_x = atof(str_x);
        double light_y = -1 * atof(str_y);
        double light_z = atof(str_z);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        if (abs(a1 - a) > eps || abs(a2 - b) || abs(n_step - step) > eps ||
            abs(light_x - s_light.x) > eps || abs(light_y - s_light.y) || abs(light_z - s_light.z))
        {
            double len = sqrt(light_x * light_x + light_y * light_y + light_z * light_z);
            //double R = min(1.0/ len, 1.0);
            double R = 1.0;
            figure = build_figure(a1, a2, n_step, R);
            a = a1;
            b = a2;
            step = n_step;
            s_light = vec3(light_x, light_y, light_z);
            light = R * s_light;
        }

        double alpha = 0;
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
        glPushMatrix();
        glRotatef(alpha, rot_axis.x, rot_axis.y, rot_axis.z);
        float m[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        glPopMatrix();

        for (int i = 0; i < figure.size(); i++)
        {
            for (int j = 0; j < figure[i].size(); j++)
                figure[i][j] = mult(m, figure[i][j]);
        }
        light = mult(m, light);

        glEnable(GL_LIGHTING);
        glShadeModel(GL_FLAT);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        GLfloat position[] = { light.x, light.y, light.z, 1 };
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        draw_figure(figure, is_inside_ellipse(light, a, b));
        glDisable(GL_LIGHTING);

        glColor3d(1, 1, 0);
        glPointSize(10);
        glEnable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        glVertex3f(-light.x, -light.y, -light.z);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(10);
    }

    glfwTerminate();
    return 0;
}
