#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cmath"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
float theta_x_mod = 0;
float theta_y_mod = 0;

const float BASE_SPEED = 0.5f;
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
                theta_x_mod += ROTATE_SPEED;
                break;
            case GLFW_KEY_LEFT:
                theta_y_mod -= ROTATE_SPEED;
                break;
            case GLFW_KEY_DOWN:
                theta_x_mod -= ROTATE_SPEED;
                break;
            case GLFW_KEY_RIGHT:
                theta_y_mod += ROTATE_SPEED;
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
                theta_x_mod = 0.f;
                break;
            case GLFW_KEY_LEFT:
                theta_y_mod = 0.f;
                break;
            case GLFW_KEY_DOWN:
                theta_x_mod = 0.f;
                break;
            case GLFW_KEY_RIGHT:
                theta_y_mod = 0.f;
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

    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load("3D/partenza.jpg", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

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

    std::fstream sky_vertSrc("Shaders/skybox.vert");
    std::stringstream sky_vertBuff;
    sky_vertBuff << sky_vertSrc.rdbuf();
    std::string sky_vertS = sky_vertBuff.str();
    const char* sky_v = sky_vertS.c_str();

    std::fstream sky_fragSrc("Shaders/skybox.frag");
    std::stringstream sky_fragBuff;
    sky_fragBuff << sky_fragSrc.rdbuf();
    std::string sky_fragS = sky_fragBuff.str();
    const char* sky_f = sky_fragS.c_str();

    GLuint sky_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sky_vertShader, 1, &v, NULL);
    glCompileShader(sky_vertShader);

    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &f, NULL);
    glCompileShader(sky_fragShader);

    GLuint skyboxProgram = glCreateProgram();
    glAttachShader(skyboxProgram, sky_vertShader);
    glAttachShader(skyboxProgram, sky_fragShader);

    glLinkProgram(skyboxProgram);

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    std::string path = "3D/djSword.obj";
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

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0)


    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];
        // X
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        // Y
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
        // Z
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);
        // U
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        // V
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
    }

    GLfloat vertices[]{
        0.f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &VBO_UV);
    //glGenBuffers(1, &EBO);

    // working with this VAO
    glBindVertexArray(VAO);
    // working with this VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //XYZ
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GLfloat),
        (void*)0
    );
    
    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3, //UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)normPtr
    );

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2, //UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)uvPtr
    );
    glEnableVertexAttribArray(0);

    /*glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        2,
        2, //UV
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );*/
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );*/

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    glm::mat4 translation = glm::translate(identity_matrix, glm::vec3(0, 0, 0));
    glm::mat4 scale = glm::scale(identity_matrix, glm::vec3(0, 0, 0));

    float fTX, fTY, fTZ;
    fTX = fTY = fTZ = 0.f;
    fTY = 30.f;
    fTZ = -70.f;

    float fSX, fSY, fSZ;
    fSX = fSY = fSZ = 0.75f;

    float fRX = 1.f;
    float fRY = 1.f;
    float fRZ = 0.f;
    float theta_x = 0.f;
    float theta_y = 0.f;

    /*glm::mat4 projection = glm::ortho(-2.0f,    // Left Most Point
                                       2.0f,    // Right Most Point
                                      -2.0f,    // Bottom Most Point
                                       2.0f,    // Top Most Point
                                      -1.0f,    // Z Near
                                       1.0f);   // Z Far*/

    glm::mat4 projection = glm::perspective(glm::radians(60.f), SCREEN_HEIGHT / SCREEN_WIDTH, 0.1f, 1000.0f);
    glm::vec3 lightPos = glm::vec3(-10, 3, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //glfwSetKeyCallback(window, Key_Callback);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 10.f);
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.0f);

        glm::vec3 worldUp = glm::normalize(glm::vec3(0.f, 1.0f, 0.f));
        glm::vec3 cameraCenter = glm::vec3(0.f, 3.0f, 0.f);

        glm::vec3 F = (cameraCenter - cameraPos);
        F = glm::normalize(F);

        glm::vec3 R = glm::normalize(glm::cross(F, worldUp));
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraOrientation = glm::mat4(1.0f);

        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;

        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;  

        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;

        //glm::mat4 viewMatrix = cameraOrientation * cameraPosMatrix;
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);

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
        theta_x += theta_x_mod;
        theta_y += ROTATE_SPEED;
        fSX += sx_mod;
        fSY += sy_mod;
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(fTX, fTY, fTZ));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(fSX, fSY, fSZ));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_x), glm::vec3(1.f, 0, 0));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_y), glm::vec3(0, 1.f, 0));


        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(viewMatrix)
                           );

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection)
                           );

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(transformation_matrix)
                           );

        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);


        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
        //glDrawElements(
        //    GL_TRIANGLES,
        //    mesh_indices.size(),
        //    GL_UNSIGNED_INT,
        //    0
        //);

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