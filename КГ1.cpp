#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <cmath>
#include <freeglut.h>
#include <string>

#define GLUT_BITMAP_HELVETICA_18 (void *)0x0008
#define GLUT_BITMAP_HELVETICA_12 (void *)0x0007

using namespace std;

double a; // Константа a
double pow2C; //Уравнение Лемнискаты Бернулли: (x^2 + y^2)^2 = 2c^2(x^2 - y^2)

double scale = 1.0; // Переменная масштабирования
double centerX = 0.0; // Центр по X
double centerY = 0.0; // Центр по Y

void textCallback(GLFWwindow* window, unsigned int codepoint) {
    static string input;
    if (codepoint == '\b') {
        if (!input.empty()) {
            input.pop_back();
        }
    }
    else if (codepoint == '\r' || codepoint == '\n') {
        try {
            a = stod(input);
        }
        catch (const std::invalid_argument& e) {
            cout << "Неверный формат числа" << endl;
        }
        input.clear();
    }
    else {
        input += static_cast<char>(codepoint);
    }
}

void drawAxes() {
    glColor3f(1, 1, 1); // Цвет осей

    // Ось X
    glBegin(GL_LINES);
    glVertex2d(-2.0 * a * scale, 0.0);
    glVertex2d(2.0 * a * scale, 0.0);
    glEnd();

    // Ось Y
    glBegin(GL_LINES);
    glVertex2d(0.0, -2.0 * a * scale);
    glVertex2d(0.0, 2.0 * a * scale);
    glEnd();

    // Масштабные линии и подписи
    glBegin(GL_LINES);
    for (double i = -2.0 * a; i < 2.0 * a - 0.1; i += 0.2) {
        glVertex2d(i * scale, 0.01 * scale);
        glVertex2d(i * scale, -0.01 * scale);

        glVertex2d(0.01 * scale, i * scale);
        glVertex2d(-0.01 * scale, i * scale);

        // Подписи на шкалах осей
        string label = "X";
        glColor3f(1.0, 1.0, 1.0); // Цвет текста
        glRasterPos2d(i * scale - 0.2 * scale, -0.1 * scale);
        for (char c : label) {
            glutBitmapCharacter((void*)0x0008, c);
        }

        glRasterPos2d(-0.2 * scale, i * scale - 0.1 * scale);
        for (char c : label) {
            glutBitmapCharacter((void*)0x0008, c);
        }
    }
    glEnd();

    // Подписи на шкалах осей
    for (double i = -2.0 * a; i < 2.0 * a - 0.1; i += 0.2) {
        string label = to_string(i);
        if (i > 0) {
            label = label.substr(0, 3);
        }
        else if (i != 0) {
            label = label.substr(0, 4);
        }
        else {
            label = label.substr(1, 4);
        }
        glColor3f(1.0, 1.0, 1.0); // Цвет текста
        if (label != "-0.0") {
            glRasterPos2d(i * scale - 0.05 * scale, -0.1 * scale);
            for (char c : label) {
                glutBitmapCharacter((void *)0x0007, c);
            }

            glRasterPos2d(-0.15 * scale, i * scale - 0.01 * scale);
            for (char c : label) {
                glutBitmapCharacter((void *)0x0007, c);
            }
        }
        else {
            glRasterPos2d(i * scale - 0.01 * scale, -0.05 * scale);
            for (char c : "0") {
                glutBitmapCharacter((void *)0x0007, c);
            }
        }
    }

    // Подписи осей
    glColor3f(1.0, 1.0, 1.0); // Цвет текста
    glRasterPos2d(2.0 * a * scale - 0.1 * scale, -0.15 * scale);
    string xAxisLabel = "X";
    for (char c : xAxisLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2d(-0.15 * scale, 2.0 * a * scale - 0.1 * scale);
    string yAxisLabel = "Y";
    for (char c : yAxisLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        //GLUT_BITMAP_HELVETICA_18
    }

    // Стрелочки на концах осей
    glLineWidth(2.0);
    glColor3d(1.0, 1.0, 1.0); // Цвет стрелочек

    // Стрелочка на конце оси X
    glBegin(GL_LINES);
    glVertex2d(2.0 * a * scale - 0.1 * scale, -0.05 * scale);
    glVertex2d(2.0 * a * scale, 0.0);
    glVertex2d(2.0 * a * scale - 0.1 * scale, 0.05 * scale);
    glVertex2d(2.0 * a * scale, 0.0);
    glEnd();

    // Стрелочка на конце оси Y
    glBegin(GL_LINES);
    glVertex2d(-0.05 * scale, 2.0 * a * scale - 0.1 * scale);
    glVertex2d(0.0, 2.0 * a * scale);
    glVertex2d(0.05 * scale, 2.0 * a * scale - 0.1 * scale);
    glVertex2d(0.0, 2.0 * a * scale);
    glEnd();
    glLineWidth(1.0); // Восстанавливаем ширину линии по умолчанию
}

void drawCurve() {
    glBegin(GL_LINE_STRIP);
    double iterator = 0.0001, x1, y1, x2, y2;
    for (x1 = -a; x1 <= a; x1 += iterator) {
        y1 = sqrt(sqrt(pow(pow2C,2) + 4*pow(x1,2)*pow2C) - pow(x1,2) - pow2C);
        x2 = x1 + iterator;
        y2 = sqrt(sqrt(pow(pow2C, 2) + 4 * pow(x2, 2) * pow2C) - pow(x2, 2) - pow2C);
        glVertex2d((x1 - centerX) * scale, y1 * scale + centerY);
        glVertex2d((x2 - centerX) * scale, y2 * scale + centerY);
    }
    for (x1 = -a; x1 <= a; x1 += iterator) {
        y1 = -sqrt(sqrt(pow(pow2C, 2) + 4 * pow(x1, 2) * pow2C) - pow(x1, 2) - pow2C);
        x2 = x1 + iterator;
        y2 = -sqrt(sqrt(pow(pow2C, 2) + 4 * pow(x2, 2) * pow2C) - pow(x2, 2) - pow2C);
        glVertex2d((x1 - centerX) * scale, y1 * scale + centerY);
        glVertex2d((x2 - centerX) * scale, y2 * scale + centerY); // Зеркально отображаем нижнюю часть кривой
    }
    glEnd();
}

int main(int argc, char** argv) {
    cout << "Input a: ";
    cin >> a; // Пользователь вводит значение a
    pow2C = a * a / 2;
    // Инициализация GLUT
    glutInit(&argc, argv);
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    // Создаем окно GLFW
    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "Wonderful curve", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Инициализируем GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Ошибка инициализации GLEW" << std::endl;
        return -1;
    }

    // Устанавливаем вьюпорт и настройки OpenGL
    glViewport(0, 0, 800, 800);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glfwSetCharCallback(window, textCallback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        double aspect = (double)width / (double)height;

        // Устанавливаем масштабирование и центрирование
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double halfWidth = 2.0 * a * scale; 
        double halfHeight = 2.0 * a * scale / aspect;
        glOrtho(centerX - halfWidth, centerX + halfWidth, centerY - halfHeight, centerY + halfHeight, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);

        glColor3f(1.0, 1.0, 1.0); // Устанавливаем цвет

        drawAxes(); // Рисуем оси и масштабные линии
        drawCurve(); // Рисуем кривую

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Ждем ввода нового значения a
    }

    glfwTerminate();
    return 0;
}
