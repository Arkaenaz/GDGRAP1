#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cmath"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "string"
#include "iostream"

const float SCREEN_WIDTH = 600;
const float SCREEN_HEIGHT = 600;

float tx_mod = 0;
float ty_mod = 0;
float tz_mod = 0;
float sx_mod = 0;
float sy_mod = 0;
float rx_mod = 1;
float ry_mod = 1;
float theta_mod = 0;

const float BASE_SPEED = 0.01f;
const float ROTATE_SPEED = 0.5f;

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scanCode,
    int action, // pressed / released
    int mod
) {
    if (action == GLFW_PRESS)
        switch (key) {
            case GLFW_KEY_W:
                ty_mod += BASE_SPEED;
                break;
            case GLFW_KEY_A:
                tx_mod -= BASE_SPEED;
                break;
            case GLFW_KEY_S:
                ty_mod -= BASE_SPEED;
                break;
            case GLFW_KEY_D:
                tx_mod += BASE_SPEED;
                break;
            case GLFW_KEY_UP:
                ry_mod = 0;
                rx_mod = 1;
                theta_mod += ROTATE_SPEED;
                break;
            case GLFW_KEY_LEFT:
                rx_mod = 0;
                ry_mod = 1;
                theta_mod -= ROTATE_SPEED;
                break;
            case GLFW_KEY_DOWN:
                ry_mod = 0;
                rx_mod = 1;
                theta_mod -= ROTATE_SPEED;
                break;
            case GLFW_KEY_RIGHT:
                rx_mod = 0;
                ry_mod = 1;
                theta_mod += ROTATE_SPEED;
                break;
            case GLFW_KEY_Z:
                tz_mod -= BASE_SPEED;
                break;
            case GLFW_KEY_X:
                tz_mod += BASE_SPEED;
                break;
            case GLFW_KEY_Q:
                sx_mod -= BASE_SPEED;
                sy_mod -= BASE_SPEED;
                break;
            case GLFW_KEY_E:
                sx_mod += BASE_SPEED;
                sy_mod += BASE_SPEED;
                break;
        }
    if (action == GLFW_RELEASE)
        switch (key) {
            case GLFW_KEY_W:
                ty_mod = 0.f;
                break;
            case GLFW_KEY_A:
                tx_mod = 0.f;
                break;
            case GLFW_KEY_S:
                ty_mod = 0.f;
                break;
            case GLFW_KEY_D:
                tx_mod = 0.f;
                break;
            case GLFW_KEY_UP:
                theta_mod = 0.f;
                break;
            case GLFW_KEY_LEFT:
                theta_mod = 0.f;
                break;
            case GLFW_KEY_DOWN:
                theta_mod = 0.f;
                break;
            case GLFW_KEY_RIGHT:
                theta_mod = 0.f;
                break;
            case GLFW_KEY_Z:
                tz_mod = 0.f;
                break;
            case GLFW_KEY_X:
                tz_mod = 0.f;
                break;
            case GLFW_KEY_Q:
                sx_mod = 0.f;
                sy_mod = 0.f;
                break;
            case GLFW_KEY_E:
                sx_mod = 0.f;
                sy_mod = 0.f;
                break;
        }
}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shane Cab", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0,   // Min X
               0,   // Min Y
               SCREEN_WIDTH, // Width
               SCREEN_HEIGHT); // Height

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
        0.f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // working with this VAO
    glBindVertexArray(VAO);
    // working with this VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //XYZ
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    glm::mat4 translation = glm::translate(identity_matrix, glm::vec3(0, 0, 0));
    glm::mat4 scale = glm::scale(identity_matrix, glm::vec3(0, 0, 0));

    float fTX = 0.f;
    float fTY = 0.f;
    float fTZ = -5.f;
    float fSX = 1.f;
    float fSY = 1.f;
    float fSZ = 1.f;
    float fRX = 1.f;
    float fRY = 1.f;
    float fRZ = 0.f;
    float theta = 0.f;

    /*glm::mat4 projection = glm::ortho(-2.0f,    // Left Most Point
                                       2.0f,    // Right Most Point
                                      -2.0f,    // Bottom Most Point
                                       2.0f,    // Top Most Point
                                      -1.0f,    // Z Near
                                       1.0f);   // Z Far*/

    glm::mat4 projection = glm::perspective(glm::radians(60.f), SCREEN_HEIGHT / SCREEN_WIDTH, 0.1f, 100.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //glfwSetKeyCallback(window, Key_Callback);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //x_mod += 0.001f;

        //unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        //glUniform1f(xLoc, x_mod);
        //unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        //glUniform1f(yLoc, y_mod);
        //theta += 0.5f;
        fTX += tx_mod;
        fTY += ty_mod;
        fTZ += tz_mod;
        fRX = rx_mod;
        fRY = ry_mod;
        theta += theta_mod;
        fSX += sx_mod;
        fSY += sy_mod;
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(fTX, fTY, fTZ));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(fSX, fSY, fSZ));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::vec3(fRX, fRY, 0));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(transformation_matrix)
                           );

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection)
                           );

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}