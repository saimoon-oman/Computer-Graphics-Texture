//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "cube.h"
#include "stb_image.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void floor(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void sofa(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void wall(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void tv(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void mat(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void shaderActivate(Shader& shader);
void drawingRoom(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, Shader& lightingShaderWithTexture);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,  1.5f,  -0.75f),
    glm::vec3(0.0f,  1.5f,  0.0f),
    glm::vec3(1.0f,  4.5f,  0.5f),
    glm::vec3(0.0f,  3.0f,  0.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.7f, 0.7f, 0.7f,     // ambient
    0.7f, 0.7f, 0.7f,      // diffuse
    0.7f, 0.7f, 0.7f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.7f, 0.7f, 0.7f,     // ambient
    0.7f, 0.7f, 0.7f,      // diffuse
    0.7f, 0.7f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);


PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.7f, 0.7f, 0.7f,     // ambient
    0.7f, 0.7f, 0.7f,      // diffuse
    0.7f, 0.7f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);


PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.7f, 0.7f, 0.7f,     // ambient
    0.7f, 0.7f, 0.7f,      // diffuse
    0.7f, 0.7f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool onOffPointToggle = true;
bool onOffSpotToggle = true;
bool onOffDirectToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;

//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection;
glm::mat4 view;

string diffuseMapPath;
string specularMapPath;
unsigned int diffMap;
unsigned int specMap;

//Cube cube;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        // glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        /*bed(cubeVAO, lightingShader, model,0);
        bed(cubeVAO, lightingShader, model, 2);*/
        drawingRoom(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        
        /*building(cubeVAO, lightingShader, model);
        road(cubeVAO, lightingShader, model);*/


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ////texture
        //glm::mat4 modelTexture = glm::mat4(1.0f);
        //glm::mat4 translate = glm::mat4(1.0f);
        //glm::mat4 scale = glm::mat4(0.5f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X+1, translate_Y+1, translate_Z + 1));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.5, scale_Y * 0.5, scale_Z * 0.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("view", view);
        lightingShaderWithTexture.setMat4("projection", projection);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        /*diffuseMapPath = "images/emoji.png";
        specularMapPath = "images/emoji.png";
        diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);*/


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void shaderActivate(Shader& shader)
{
    shader.use();
    shader.setVec3("viewPos", camera.Position);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

void drawingRoom(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, Shader& lightingShaderWithTexture)
{
    floor(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    wall(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    sofa(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    table(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    tv(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    mat(cubeVAO, lightingShader, model, lightingShaderWithTexture);

}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void sofa(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 0.2f;
    float width = 1.2f;
    float length = 0.4f;

    // base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0.0, -0.5));
    translate2 = glm::translate(model, glm::vec3(width-0.5- 0.5, 0.0, -1.0));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.546, 0.335, 0.316);
    shaderActivate(lightingShaderWithTexture);
    diffuseMapPath = "images/sofa_top.jpg";
    specularMapPath = "images/sofa_top.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 4.0f, 2.0f);
    cube1.drawCubeWithTexture(lightingShaderWithTexture, model);
    //shaderActivate(lightingShader);

    // foam
    diffuseMapPath = "images/sofa_foam.jpg";
    specularMapPath = "images/sofa_foam.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.3, 0.03, 0.3));
    translate2 = glm::translate(model, glm::vec3(width - 0.19 - 0.5, 0.2, -1.0+0.05));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.2, 0.335, 0.5);
    Cube cube2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube2.drawCubeWithTexture(lightingShaderWithTexture, model);

    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.3, 0.03, 0.3));
    translate2 = glm::translate(model, glm::vec3(width - 0.195-0.3 - 0.5, 0.2, -1.0 + 0.05));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);
    Cube cube3 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.3, 0.03, 0.3));
    translate2 = glm::translate(model, glm::vec3(width - 0.2-0.3-0.3 - 0.5, 0.2, -1.0 + 0.05));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.2, 0.335, 0.5);
    Cube cube4 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube4.drawCubeWithTexture(lightingShaderWithTexture, model);
    //shaderActivate(lightingShader);

    // back
    diffuseMapPath = "images/sofa_foam.jpg";
    specularMapPath = "images/sofa_foam.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, 0.1));
    translate2 = glm::translate(model, glm::vec3(width - 0.5 - 0.5, 0.2, -1.15));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.2, 0.335, 0.5);
    Cube cube5 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube5.drawCubeWithTexture(lightingShaderWithTexture, model);

    // right
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.12, baseHeight, 0.4));
    translate2 = glm::translate(model, glm::vec3(width+0.04 - 0.5, 0.2, -1.0));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.2, 0.335, 0.5);
    Cube cube6 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube6.drawCubeWithTexture(lightingShaderWithTexture, model);

    // left
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.12, baseHeight, 0.4));
    translate2 = glm::translate(model, glm::vec3(width - 1.04 - 0.5, 0.2, -1.0));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.2, 0.335, 0.5);
    Cube cube7 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube7.drawCubeWithTexture(lightingShaderWithTexture, model);

    // left pillo
    //shaderActivate(lightingShaderWithTexture);
    diffuseMapPath = "images/sofa_pillow.jpg";
    specularMapPath = "images/sofa_pillow.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.01));
    translate2 = glm::translate(model, glm::vec3(width-0.8 - 0.5, 0.23, -1.09));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);
    Cube cube8 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube8.drawCubeWithTexture(lightingShaderWithTexture, model);

    // right pillo
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.01));
    translate2 = glm::translate(model, glm::vec3(width - 0.19 - 0.5, 0.23, -1.09));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);
    Cube cube9 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube9.drawCubeWithTexture(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);

}

void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 0.02;
    float width = 0.7;
    float length = 0.4;

    // base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0.0, -0.5));
    translate2 = glm::translate(model, glm::vec3(width - 0.5, 0.2, -0.2));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
    shaderActivate(lightingShaderWithTexture);
    string diffuseMapPath = "images/table_top.jpg";
    string specularMapPath = "images/table_top.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);

    // leg
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.05, 0.2, 0.05));
    translate2 = glm::translate(model, glm::vec3(width-0.175, 0.0, -0.35));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);

    // leg
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.05, 0.2, 0.05));
    translate2 = glm::translate(model, glm::vec3(width - 0.175, 0.0, -0.025));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);

    // leg
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.05, 0.2, 0.05));
    translate2 = glm::translate(model, glm::vec3(width - 0.825, 0.0, -0.35));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);

    // leg
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.05, 0.2, 0.05));
    translate2 = glm::translate(model, glm::vec3(width - 0.825, 0.0, -0.025));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.24, 0.635, 0.77);
    
}


void floor(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 0.01;
    float width = 3.0;
    float length = 3.0;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width+1, baseHeight, length+1));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.3, 0.3, 0.3);
    shaderActivate(lightingShaderWithTexture);
    diffuseMapPath = "images/floor2.jpg";
    specularMapPath = "images/floor2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 4.0f);
    cube1.drawCubeWithTexture(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);
}

void mat(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 0.01;
    float width = 0.6;
    float length = 0.4;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(0.0, 1.0, 0.5));
    model = alTogether * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);
    shaderActivate(lightingShaderWithTexture);
    diffuseMapPath = "images/mat.jpg";
    specularMapPath = "images/mat.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube1.drawCubeWithTexture(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);
}


void wall(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 1.5;
    float width = 0.01;
    float length = 3.0;

    // right
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(1.5, 0.0, -1.5));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.3, 0.3, 0.3);
    shaderActivate(lightingShaderWithTexture);
    diffuseMapPath = "images/wall2.jpg";
    specularMapPath = "images/wall2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 4.0f);
    cube1.drawCubeWithTexture(lightingShaderWithTexture, model);
    //shaderActivate(lightingShader);

    // left
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-1.5, 0.0, -1.5));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.3, 0.3, 0.3);
    Cube cube2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 4.0f);
    cube2.drawCubeWithTexture(lightingShaderWithTexture, model);

    // back
    width = 3.0;
    length = 0.01;
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-1.5, 0.0, -1.5));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.3, 0.3, 0.3);
    Cube cube3 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 4.0f);
    cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

    // front top
    baseHeight = baseHeight / 3;
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-1.5, 2*baseHeight, 1.5));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.3, 0.3, 0.3);
    Cube cube4 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 2.0f);
    cube4.drawCubeWithTexture(lightingShaderWithTexture, model);

    // front bottom
    baseHeight = baseHeight * 2;
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width-0.5, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-1.0, 0.0, 1.5));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.3, 0.3, 0.3);
    Cube cube5 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 4.0f);
    cube5.drawCubeWithTexture(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);
}

void tv(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 0.4;
    float width = 0.6;
    float length = 0.02;

    // tv
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(0.0, 0.5, 1.475));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);
    shaderActivate(lightingShaderWithTexture);
    diffuseMapPath = "images/tv.jpg";
    specularMapPath = "images/tv.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube1.drawCubeWithTexture(lightingShaderWithTexture, model);
    //shaderActivate(lightingShader);

    // sound box
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, 0.05, length));
    translate = glm::translate(model, glm::vec3(0.0, 0.4, 1.475));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);
    diffuseMapPath = "images/soundbox.jpg";
    specularMapPath = "images/soundbox.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 4.0f, 1.0f);
    cube2.drawCubeWithTexture(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
   // if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
   // {
   //     if (onOffPointToggle)
   //     {
   //         pointlight1.turnOff();
   //         
   //         onOffPointToggle = false;
   //     }
   //     else
   //     {
   //         pointlight1.turnOn();
   //       
   //         onOffPointToggle = true;
   //     }
   //    // pointlight3.turnOff();
   //    // pointlight4.turnOff();

   // }
   // 

   // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
   // {
   //     
   //     if (onOffSpotToggle)
   //     {
   //        
   //         pointlight2.turnOff();
   //         onOffSpotToggle = false;
   //     }
   //     else
   //     {
   //         pointlight2.turnOn();
   //         onOffSpotToggle = true;
   //     }
   // }

   // if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
   // {

   //     if (onOffDirectToggle)
   //     {

   //         pointlight3.turnOff();
   //         onOffDirectToggle = false;
   //     }
   //     else
   //     {
   //         pointlight3.turnOn();
   //         onOffDirectToggle = true;
   //     }
   // }
   // 
   // if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
   // {
   //     pointlight1.turnAmbientOn();
   //     pointlight2.turnAmbientOn();
   //    // pointlight3.turnAmbientOn();
   //    // pointlight4.turnAmbientOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
   // {
   //     pointlight1.turnAmbientOff();
   //     pointlight2.turnAmbientOff();
   //   //  pointlight3.turnAmbientOff();
   //   //  pointlight4.turnAmbientOff();
   // }
   // if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
   // {
   //     pointlight1.turnDiffuseOn();
   //     pointlight2.turnDiffuseOn();
   //  //   pointlight3.turnDiffuseOn();
   // //    pointlight4.turnDiffuseOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
   // {
   //     pointlight1.turnDiffuseOff();
   //     pointlight2.turnDiffuseOff();
   ////     pointlight3.turnDiffuseOff();
   // //    pointlight4.turnDiffuseOff();
   // }
   // if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
   // {
   //     pointlight1.turnSpecularOn();
   //     pointlight2.turnSpecularOn();
   // //    pointlight3.turnSpecularOn();
   // //    pointlight4.turnSpecularOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
   // {
   //     pointlight1.turnSpecularOff();
   //     pointlight2.turnSpecularOff();
   ////     pointlight3.turnSpecularOff();
   // //    pointlight4.turnDiffuseOff();
   // }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        pointlight2.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        pointlight2.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        pointlight3.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        pointlight3.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOn();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOn();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOn();
        //pointlight4.turnDiffuseOn();
        //diffuseToggle = !diffuseToggle;
    //}
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOff();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOff();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOff();
        //pointlight4.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
    //}
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOn();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOn();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOn();
        //pointlight4.turnAmbientOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOff();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOff();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }


    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOn();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOn();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOn();
        //pointlight4.turnSpecularOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        /*cout << "1 " << pointlight1.isOn() << endl;
        cout << pointlight2.isOn() << endl;
        cout << pointlight3.isOn() << endl;*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOff();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOff();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOff();
        //pointlight4.turnSpecularOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
