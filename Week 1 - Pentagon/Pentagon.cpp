#include <GLFW/glfw3.h>
#include "cmath"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Pentagon", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Triangle Version
        /*for (float i = 0; i < 5; i++) {
            float angle1 = (0 + 72 * i) * 3.14 / 180;
            float angle2 = (72 + 72 * i)* 3.14 / 180;
            glBegin(GL_TRIANGLES);
            glVertex2f(0.f, 0.f);
            glVertex2f(0.3f * cos(angle1), 0.3f * sin(angle1));
            glVertex2f(0.3f * cos(angle2), 0.3f * sin(angle2));
            glEnd();
        }*/

        glBegin(GL_POLYGON);
        for (float i = 0; i < 5; i++) {
            float angle1 = (0 + 72 * i) * 3.14 / 180;
            float angle2 = (72 + 72 * i) * 3.14 / 180;
            glVertex2f(0.3f * cos(angle1), 0.3f * sin(angle1));
        }
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}