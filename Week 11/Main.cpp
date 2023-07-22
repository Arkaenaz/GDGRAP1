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

double currentFrame = glfwGetTime();
double lastFrame = currentFrame;
double deltaTime;

float tx_mod = 0;
float ty_mod = 0;
float tz_mod = 0;
float sx_mod = 0;
float sy_mod = 0;
float rx_mod = 1;
float ry_mod = 1;
float theta_x_mod = 0;
float theta_y_mod = 0;

const float BASE_SPEED = 10.0f;
const float ROTATE_SPEED = 100.0f;

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
    unsigned char* tex_bytes = stbi_load("3D/brickwall.jpg", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    int img_width2, img_height2, colorChannels2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes2 = stbi_load("3D/brickwall_normal.jpg", &img_width2, &img_height2, &colorChannels2, 0);

    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width2, img_height2, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes2);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes2);

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
    glShaderSource(sky_vertShader, 1, &sky_v, NULL);
    glCompileShader(sky_vertShader);

    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png"
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shape[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shape[0].mesh.indices[i];
        tinyobj::index_t vData2 = shape[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shape[0].mesh.indices[i + 2];
        
        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );
        
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
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

        fullVertexData.push_back(tangents[i].x);
        fullVertexData.push_back(tangents[i].y);
        fullVertexData.push_back(tangents[i].z);

        fullVertexData.push_back(bitangents[i].x);
        fullVertexData.push_back(bitangents[i].y);
        fullVertexData.push_back(bitangents[i].z);
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
        14 * sizeof(GLfloat),
        (void*)0
    );
    
    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3, //UV
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)normPtr
    );

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2, //UV
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)uvPtr
    );
    
    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(
        3,
        3, 
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)tangentPtr
    );

    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(
        4,
        3, 
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)bitangentPtr
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
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);


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
    fTY = 0.f;
    fTZ = -10.f;

    float fSX, fSY, fSZ;
    fSX = fSY = fSZ = 0.3f;

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
    glm::vec3 lightPos = glm::vec3(0, 0, 8);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16;

    // Enable Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,                // Source Factor      - Foreground Layer
                GL_ONE_MINUS_SRC_ALPHA);     // Destination Factor - Background Layer

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //glfwSetKeyCallback(window, Key_Callback);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(skyboxProgram);

        glm::mat4 skyView = glm::mat4(1.f);
        skyView = glm::mat4(glm::mat3(viewMatrix));

        unsigned int sky_ProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
        glUniformMatrix4fv(sky_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        unsigned int sky_ViewLoc = glGetUniformLocation(skyboxProgram, "view");
        glUniformMatrix4fv(sky_ViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProgram);
        //x_mod += 0.001f;

        //unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        //glUniform1f(xLoc, x_mod);
        //unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        //glUniform1f(yLoc, y_mod);
        //theta += 0.5f;
        fTX += tx_mod * deltaTime;
        fTY += ty_mod * deltaTime;
        fTZ += tz_mod * deltaTime;
        fRX = rx_mod;
        fRY = ry_mod;
        theta_x += theta_x_mod * deltaTime;
        theta_y += theta_y_mod * deltaTime;
        fSX += sx_mod * deltaTime;
        fSY += sy_mod * deltaTime;
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
        
        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);
        
        glActiveTexture(GL_TEXTURE1);
        GLuint norm_texAddress = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(norm_texAddress, 0);

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

        glBindVertexArray(VAO);


        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);
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