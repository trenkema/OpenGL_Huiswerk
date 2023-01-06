//#include <iostream>
//#include <glad/glad.h>
//#include "GLFW/glfw3.h"
//#include <fstream>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <stdlib.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//using namespace glm;

//void loadFromFile(const char* url, char** target) {
//    std::ifstream stream(url, std::ios::binary);
//    stream.seekg(0, stream.end);
//    int total = stream.tellg();
//    *target = new char[total + 1];
//    stream.seekg(0, stream.beg);
//    stream.read(*target, total);
//    (*target)[total] = '\0';
//    stream.close();
//}
//unsigned int loadTexture(std::string url, GLenum format) {
//    // gen & bind IDs
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    // instellen texture params
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    // shit inladen
//    int width, height, channels;
//    unsigned char* data;
//    data = stbi_load(url.c_str(), &width, &height, &channels, 0);
//    if (data == nullptr) {
//        std::cout << "Error loading file: " << url.c_str() << std::endl;
//    }
//    else {
//        // shit aan opengl geven
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
//            GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    stbi_image_free(data);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    return textureID;
//}
//int main()
//{
//    std::cout << "OpenGL Code by Tom Renkema!\n";
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    int width = 1280;
//    int height = 720;
//    GLFWwindow* window = glfwCreateWindow(width, height, "Les 3 | Tom Renkema", nullptr,
//        nullptr);
//    glfwMakeContextCurrent(window);
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//    glViewport(0, 0, width, height);
//    ///SETUP OBJECT///
//    // Vertices of our triangle!
//    // need 24 vertices for normal/uv-mapped Cube
//    float vertices[] = {
//        // positions            //colors            // tex coords   // normals
//        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,   1.f, 0.f,       0.f, -1.f, 0.f,
//        0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 0.0f,   1.f, 1.f,       0.f, -1.f, 0.f,
//        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, -1.f, 0.f,
//        -0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, -1.f, 0.f,
//        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   2.f, 0.f,       1.f, 0.f, 0.f,
//        0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   2.f, 1.f,       1.f, 0.f, 0.f,
//        0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   1.f, 2.f,       0.f, 0.f, 1.f,
//        -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   0.f, 2.f,       0.f, 0.f, 1.f,
//        -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   -1.f, 1.f,      -1.f, 0.f, 0.f,
//        -0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   -1.f, 0.f,      -1.f, 0.f, 0.f,
//        -0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   0.f, -1.f,      0.f, 0.f, -1.f,
//        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   1.f, -1.f,      0.f, 0.f, -1.f,
//        -0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   3.f, 0.f,       0.f, 1.f, 0.f,
//        -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   3.f, 1.f,       0.f, 1.f, 0.f,
//        0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       0.f, 0.f, 1.f,
//        -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, 0.f, 1.f,
//        -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       -1.f, 0.f, 0.f,
//        -0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       -1.f, 0.f, 0.f,
//        -0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, 0.f, -1.f,
//        0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.f, 0.f,       0.f, 0.f, -1.f,
//        0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.f, 0.f,       1.f, 0.f, 0.f,
//        0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       1.f, 0.f, 0.f,
//        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   2.f, 0.f,       0.f, 1.f, 0.f,
//        0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   2.f, 1.f,       0.f, 1.f, 0.f
//    };
//    unsigned int indices[] = {  // note that we start from 0!
//        // DOWN
//        0, 1, 2,   // first triangle
//        0, 2, 3,    // second triangle
//        // BACK
//        14, 6, 7,   // first triangle
//        14, 7, 15,    // second triangle
//        // RIGHT
//        20, 4, 5,   // first triangle
//        20, 5, 21,    // second triangle
//        // LEFT
//        16, 8, 9,   // first triangle
//        16, 9, 17,    // second triangle
//        // FRONT
//        18, 10, 11,   // first triangle
//        18, 11, 19,    // second triangle
//        // UP
//        22, 12, 13,   // first triangle
//        22, 13, 23,    // second triangle
//    };
//
//    struct LightInfo
//    {
//        vec3 position;
//        vec3 ambientColor;
//        vec3 diffuseColor;
//        vec3 specular;
//        float constant;
//        float linear;
//        float quadratic;
//    };
//
//    struct LightPropertyNames
//    {
//        const GLchar* positionName;
//        const GLchar* ambientName;
//        const GLchar* diffuseName;
//        const GLchar* specularName;
//
//        const GLchar* constantName;
//        const GLchar* linearName;
//        const GLchar* quadraticName;
//    };
//
//    int amountOfLights = 2;
//
//    LightInfo lightProperties[2]
//    { 
//        { vec3(1.2f, 1.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f },
//        { vec3(-1.2f, 1.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f },
//    };
//
//    LightPropertyNames lightPropertyNames[2]
//    {
//        { "pointLights[0].position", "pointLights[0].ambientColor", "pointLights[0].diffuseColor", "pointLights[0].specular", "pointLights[0].constant", "pointLights[0].linear", "pointLights[0].quadratic" },
//        { "pointLights[1].position", "pointLights[1].ambientColor", "pointLights[1].diffuseColor", "pointLights[1].specular", "pointLights[1].constant", "pointLights[1].linear", "pointLights[1].quadratic" }
//    };
//
//    unsigned int VAO;
//    glGenVertexArrays(1, &VAO);
//    unsigned int lightVAO;
//    glGenVertexArrays(1, &lightVAO);
//    glBindVertexArray(lightVAO);
//    unsigned int VBO;
//    glGenBuffers(1, &VBO);
//    unsigned int EBO;
//    glGenBuffers(1, &EBO);
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
//        GL_STATIC_DRAW);
//    int stride = sizeof(float) * 11;
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//    glEnableVertexAttribArray(0);
//    // uv
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
//        6));
//    glEnableVertexAttribArray(1);
//    // normal
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
//        8));
//    glEnableVertexAttribArray(2);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    ///END SETUP OBJECT///
//    ///SETUP SHADER PROGRAM///
//    char* vertexSource;
//    loadFromFile("vertexShaderLes2.shader", &vertexSource);
//    char* fragmentSource;
//    loadFromFile("fragmentShaderLes2.shader", &fragmentSource);
//    // LOAD & CREATE TEXTURES
//
//    unsigned int diffuseTexID = loadTexture("container2.png", GL_RGBA);
//    unsigned int specularTexID = loadTexture("container2_specular.png", GL_RGBA);
//    //unsigned int diffuseTexID = loadTexture("box.jpg", GL_RGB);
//    // END
//    unsigned int vertID, fragID;
//    vertID = glCreateShader(GL_VERTEX_SHADER);
//    fragID = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(vertID, 1, &vertexSource, nullptr);
//    glShaderSource(fragID, 1, &fragmentSource, nullptr);
//    int success;
//    char infoLog[512];
//    glCompileShader(vertID);
//    glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertID, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
//            std::endl;
//    };
//    glCompileShader(fragID);
//    glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragID, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
//            std::endl;
//    };
//    unsigned int myProgram = glCreateProgram();
//    glAttachShader(myProgram, vertID);
//    glAttachShader(myProgram, fragID);
//    glLinkProgram(myProgram);
//    glDeleteShader(vertID);
//    glDeleteShader(fragID);
//    ///END SETUP SHADER PROGRAM///
//    /// MATRIX SETUP ///
//    glUseProgram(myProgram);
//    int worldLoc = glGetUniformLocation(myProgram, "world");
//    int viewLoc = glGetUniformLocation(myProgram, "view");
//    int projLoc = glGetUniformLocation(myProgram, "projection");
//    /// END MATRIX SETUP ///
//    // OPENGL SETTINGS //
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glEnable(GL_MULTISAMPLE);
//
//    int startIndexName = 0;
//    int startIndexProperties = 0;
//
//    while (!glfwWindowShouldClose(window)) {
//        double time = glfwGetTime();
//        glm::mat4 world = glm::mat4(1.f);
//        world = glm::rotate(world, glm::radians((float)time * 45.0f), glm::vec3(1, 1, 1));
//        world = glm::scale(world, glm::vec3(1, 1, 1));
//        world = glm::translate(world, glm::vec3(0, 0, 0));
//        glm::mat4 view = glm::lookAt(glm::vec3(0, 2, -2), glm::vec3(0, 0, 0),
//            glm::vec3(0, 1, 0));
//        glm::mat4 projection = glm::perspective(glm::radians(65.0f), width /
//            (float)height, 0.1f, 100.0f);
//        glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//        // iets tekenen
//        //glClearColor(r,g,b,1.0f);
//        glClearColor(0, 0, 0, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // be sure to activate the shader
//        glUseProgram(myProgram);
//
//        glm::vec3 lightColor;
//        lightColor.x = sin(time * 2.0f);
//        lightColor.y = sin(time * 0.7f);
//        lightColor.z = sin(time * 1.3f);
//
//        glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
//        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.35f);
//
//        for (int i = 0; i < amountOfLights; i++)
//        {
//            int pointLightPosition = glGetUniformLocation(myProgram, lightPropertyNames[i].positionName);
//            int pointLightAmbientColor = glGetUniformLocation(myProgram, lightPropertyNames[i].ambientName);
//            int pointLightDiffuseColor = glGetUniformLocation(myProgram, lightPropertyNames[i].diffuseName);
//            int pointLightSpecular = glGetUniformLocation(myProgram, lightPropertyNames[i].specularName);
//
//            int pointLightConstant = glGetUniformLocation(myProgram, lightPropertyNames[i].constantName);
//            int pointLightLinear = glGetUniformLocation(myProgram, lightPropertyNames[i].linearName);
//            int pointLightQuadratic = glGetUniformLocation(myProgram, lightPropertyNames[i].quadraticName);
//
//            glUniform3f(pointLightPosition, lightProperties[i].position.x, lightProperties[i].position.y, lightProperties[i].position.z);
//            glUniform3f(pointLightAmbientColor, lightProperties[i].ambientColor.x, lightProperties[i].ambientColor.y, lightProperties[i].ambientColor.z);
//            glUniform3f(pointLightDiffuseColor, lightProperties[i].diffuseColor.x, lightProperties[i].diffuseColor.y, lightProperties[i].diffuseColor.z);
//            glUniform3f(pointLightSpecular, lightProperties[i].specular.x, lightProperties[i].specular.y, lightProperties[i].specular.z);
//
//            glUniform1f(pointLightConstant, lightProperties[i].constant);
//            glUniform1f(pointLightLinear, lightProperties[i].linear);
//            glUniform1f(pointLightQuadratic, lightProperties[i].quadratic);
//            startIndexProperties++;
//        }
//
//        int materialDiffuseSampler = glGetUniformLocation(myProgram, "material.diffuseSample");
//        int materialSpecular = glGetUniformLocation(myProgram, "material.specular");
//        int materialShine = glGetUniformLocation(myProgram, "material.shininess");
//        glUniform1i(materialDiffuseSampler, 0);
//        glUniform1i(materialSpecular, 1);
//        glUniform1f(materialShine, 64.0f);
//
//        // update the uniform color
//        //int materialDiffuseSampler = glGetUniformLocation(myProgram, "material.diffuseSample");
//        //int materialSpecular = glGetUniformLocation(myProgram, "material.specular");
//        //int materialShine = glGetUniformLocation(myProgram, "material.shininess");
//        //glUniform1i(materialDiffuseSampler, 0);
//        //glUniform1i(materialSpecular, 1);
//        //glUniform1f(materialShine, 64.0f);
//
//        //int pointLightPosition = glGetUniformLocation(myProgram, "pointLight.position");
//        //int pointLightDiffuseColor = glGetUniformLocation(myProgram, "pointLight.diffuseColor");
//        //int pointLightAmbientColor = glGetUniformLocation(myProgram, "pointLight.ambientColor");
//        //int pointLightSpecular = glGetUniformLocation(myProgram, "pointLight.specular");
//
//        //glUniform3f(pointLightPosition, 1.2f, 1.0f, 2.0f);
//        //glUniform3f(pointLightDiffuseColor, diffuseColor.x, diffuseColor.y, diffuseColor.z);
//        //glUniform3f(pointLightAmbientColor, ambientColor.x, ambientColor.y, ambientColor.z);
//        //glUniform3f(pointLightSpecular, 1.0f, 1.0f, 1.0f);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, diffuseTexID);
//
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, specularTexID);
//
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    glfwTerminate();
//    return 0;
//}