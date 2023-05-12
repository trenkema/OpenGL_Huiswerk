//// OpenGL_Lessen.cpp : This file contains the 'main' function. Program execution beginsand ends there.
//#include <iostream>
//#include <glad/glad.h>
//#include "GLFW/glfw3.h"
//#include <fstream>
//#include "model.h"
//#include "mesh.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include "utils.h"
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//
//using namespace glm;
//
//struct ModelInfo
//{
//    int modelID;
//    vec3 position;
//    vec3 rotation;
//    float scale;
//};
//
//// forward declarations
//void renderTerrain(glm::mat4 view, glm::mat4 projection);
//void renderSkybox(glm::mat4 view, glm::mat4 projection);
//void setupResources();
//void RenderModel(Model* model, unsigned int shader, glm::vec3 position, glm::vec3 rotation, float scale, glm::mat4 view, glm::mat4 projection);
//unsigned int GeneratePlane(const char* heightmap, GLenum format, int comp, float hScale, float xzScale, unsigned int& size, unsigned int& heightmapID);
//void GenerateModelInfoStats();
//int RandomNumber(int min, int max);
//
//glm::vec3 cameraPosition(100, 100, 100), cameraForward(0, 0, 1), cameraUp(0, 1, 0);
//unsigned int plane, planeSize, VAO, cubeSize;
//unsigned int myProgram, modelProgram, skyProgram;
//
//// textures
//unsigned int heightmapID, heightNormalID;
//unsigned int dirtID, sandID, grassID;
//
//const int amountOfModels = 2;
//Model* models[amountOfModels]; // 0 - Backpack; 1 - Tree
//
//const int modelInfoSize = 25;
//ModelInfo modelInfos[modelInfoSize]
//{
//};
//
//int screenWidth = 1920;
//int screenHeight = 1080;
//
//void handleInput(GLFWwindow* window, float deltaTime) {
//    static bool w, s, a, d, space, ctrl;
//    static double cursorX = -1, cursorY = -1, lastCursorX, lastCursorY;
//    static float pitch, yaw;
//    static float speed = 100.0f;
//    float sensitivity = 100.0f * deltaTime;
//    float step = speed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) w = true; // FORWARD
//    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) w = false;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) s = true; // BACKWARDS
//    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) s = false;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) a = true; // LEFT
//    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) a = false;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) d = true; // RIGHT
//    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) d = false;
//
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) space = true; // UP
//    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) space = false;
//    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ctrl = true; // DOWN
//    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) ctrl = false;
//
//    if (cursorX == -1) {
//        glfwGetCursorPos(window, &cursorX, &cursorY);
//    }
//    lastCursorX = cursorX;
//    lastCursorY = cursorY;
//    glfwGetCursorPos(window, &cursorX, &cursorY);
//    glm::vec2 mouseDelta(cursorX - lastCursorX, cursorY - lastCursorY);
//    // TODO: calculate rotation & movement
//    yaw -= mouseDelta.x * sensitivity;
//    pitch += mouseDelta.y * sensitivity;
//    if (pitch < -90.0f) pitch = -90.0f;
//    else if (pitch > 90.0f) pitch = 90.0f;
//    if (yaw < -180.0f) yaw += 360;
//    else if (yaw > 180.0f) yaw -= 360;
//    glm::vec3 euler(glm::radians(pitch), glm::radians(yaw), 0);
//    glm::quat q(euler);
//    // update camera position / forward & up
//    glm::vec3 translation(0, 0, 0);
//    //implement movement
//    if (w) translation.z += speed * deltaTime;
//    if (s) translation.z -= speed * deltaTime;
//    if (a) translation.x += speed * deltaTime;
//    if (d) translation.x -= speed * deltaTime;
//    if (space) translation.y += speed * deltaTime;
//    if (ctrl) translation.y -= speed * deltaTime;
//    cameraPosition += q * translation;
//    cameraUp = q * glm::vec3(0, 1, 0);
//    cameraForward = q * glm::vec3(0, 0, 1);
//}
//
//int main()
//{
//    static double previousT = 0;
//    std::cout << "Welkom bij de bzt show!\n";
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
//    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "BZT Show! - Tom Renkema", glfwGetPrimaryMonitor(), nullptr);
//
//    glfwMakeContextCurrent(window);
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//    glViewport(0, 0, screenWidth, screenHeight);
//    setupResources();
//    // OPENGL SETTINGS //
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
//
//        // Define the vertices of the quad
//    float vertices[] = {
//        // positions          // colors           // texture coords
//          222.5f, 10.0f, 227.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,   // top left
//         227.5f, 10.0f, 227.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right
//          222.5f, 10.0f,  222.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
//         227.5f, 10.0f,  222.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f    // bottom right
//    };
//
//    // Generate a vertex array object (VAO) and a vertex buffer object (VBO)
//    unsigned int VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    // Bind the VAO and VBO
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    // Upload the vertex data to the VBO
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Set the vertex attribute pointers
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // color
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    // texture coord
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//    // Unbind the VAO and VBO
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    ImGui::StyleColorsDark();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//
//    bool drawSkybox = true;
//    ImGuiWindowFlags window_flags = 0;
//    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
//    window_flags |= ImGuiWindowFlags_NoCollapse;
//    window_flags |= ImGuiWindowFlags_NoResize;
//    window_flags |= ImGuiWindowFlags_NoMove;
//
//    while (!glfwWindowShouldClose(window)) {
//        double t = glfwGetTime();
//        float deltaTime = t - previousT;
//        previousT = t;
//        handleInput(window, deltaTime);
//
//        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition +
//            cameraForward, cameraUp);
//        glm::mat4 projection = glm::perspective(glm::radians(65.0f), screenWidth /
//            (float)screenHeight, 0.1f, 1000.0f);
//        // scherm clearen
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        // Render scene
//        if (drawSkybox)
//        {
//            renderSkybox(view, projection);
//        }
//
//        renderTerrain(view, projection);
//
//        for (int i = 0; i < modelInfoSize; i++)
//        {
//            RenderModel(models[modelInfos[i].modelID], modelProgram, modelInfos[i].position, modelInfos[i].rotation, modelInfos[i].scale, view, projection);
//        }
//
//        ImGui::Begin("Settings", NULL, window_flags);
//        ImGui::SetWindowPos(ImVec2(50, 50));
//        ImGui::Text("Text is finally working!");
//        ImGui::Checkbox("Draw Skybox", &drawSkybox);
//
//        if (ImGui::Button("Quit"))
//        {
//            glfwTerminate();
//        }
//
//        ImGui::End();
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwTerminate();
//    return 0;
//}
//
//void Reshape(int NewWidth, int NewHeight)
//{
//    // Apply needed glut viewport updates
//    glViewport(0, 0, NewWidth, NewHeight);
//}
//
//void renderTerrain(glm::mat4 view, glm::mat4 projection) {
//    // TERRAIN
//    glUseProgram(myProgram);
//    glCullFace(GL_BACK);
//    glEnable(GL_DEPTH_TEST);
//    glm::mat4 world = glm::mat4(1.f);
//    world = glm::translate(world, glm::vec3(0, 0, 0));
//    glUniformMatrix4fv(glGetUniformLocation(myProgram, "world"), 1, GL_FALSE,
//        glm::value_ptr(world));
//    glUniformMatrix4fv(glGetUniformLocation(myProgram, "view"), 1, GL_FALSE,
//        glm::value_ptr(view));
//    glUniformMatrix4fv(glGetUniformLocation(myProgram, "projection"), 1, GL_FALSE,
//        glm::value_ptr(projection));
//    glUniform3fv(glGetUniformLocation(myProgram, "cameraPosition"), 1,
//        glm::value_ptr(cameraPosition));
//    float t = glfwGetTime();
//    glUniform3f(glGetUniformLocation(myProgram, "lightDirection"), glm::cos(t), -
//        0.5f, glm::sin(t));
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, heightmapID);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, heightNormalID);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, dirtID);
//    glActiveTexture(GL_TEXTURE3);
//    glBindTexture(GL_TEXTURE_2D, sandID);
//    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_2D, grassID);
//    glBindVertexArray(plane);
//    glDrawElements(GL_TRIANGLES, planeSize, GL_UNSIGNED_INT, 0);
//}
//void renderSkybox(glm::mat4 view, glm::mat4 projection) {
//    // SKY BOX
//    glUseProgram(skyProgram);
//    glCullFace(GL_FRONT);
//    glDisable(GL_DEPTH_TEST);
//    glm::mat4 world = glm::mat4(1.f);
//    world = glm::translate(world, cameraPosition);
//    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "world"), 1, GL_FALSE,
//        glm::value_ptr(world));
//    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "view"), 1, GL_FALSE,
//        glm::value_ptr(view));
//    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "projection"), 1, GL_FALSE,
//        glm::value_ptr(projection));
//    glUniform3fv(glGetUniformLocation(skyProgram, "cameraPosition"), 1,
//        glm::value_ptr(cameraPosition));
//    float t = glfwGetTime();
//    glUniform3f(glGetUniformLocation(skyProgram, "lightDirection"), glm::cos(t), -
//        0.5f, glm::sin(t));
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, cubeSize, GL_UNSIGNED_INT, 0);
//}
//void setupResources() {
//    // Create models to spawn
//    models[0] = new Model("assets/Backpack/backpack.obj");
//    models[1] = new Model("assets/Tree/Tree.obj");
//        
//// need 24 vertices for normal/uv-mapped Cube
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
//    cubeSize = sizeof(indices);
//    glGenVertexArrays(1, &VAO);
//    unsigned int VBO;
//    glGenBuffers(1, &VBO);
//    unsigned int EBO;
//    glGenBuffers(1, &EBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
//        GL_STATIC_DRAW);
//    int stride = sizeof(float) * 11;
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//    glEnableVertexAttribArray(0);
//    // color
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
//        3));
//    glEnableVertexAttribArray(1);
//    // uv
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
//        6));
//    glEnableVertexAttribArray(2);
//    // normal
//    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
//        8));
//    glEnableVertexAttribArray(3);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    /// END SETUP OBJECT ///
//
//    plane = GeneratePlane("Heightmap.png", GL_RGBA, 4, 100.0f, 1.0f, planeSize, heightmapID);
//    // terrain textures
//    heightNormalID = loadTexture("heightnormal.png", GL_RGBA, 4);
//    dirtID = loadTexture("dirt.jpg", GL_RGB, 3);
//    sandID = loadTexture("sand.jpg", GL_RGB, 3);
//    grassID = loadTexture("grass.png", GL_RGBA, 4);
//    /// SETUP SHADER PROGRAM ///
//    stbi_set_flip_vertically_on_load(true);
//    char* vertexSource;
//    loadFromFile("vertexShaderLes5.shader", &vertexSource);
//    char* fragmentSource;
//    loadFromFile("fragmentShaderLes5.shader", &fragmentSource);
//    unsigned int vertSky, fragSky;
//    CreateShader("vertexShaderSkyLes5.shader", GL_VERTEX_SHADER, vertSky);
//    CreateShader("fragmentShaderSkyLes5.shader", GL_FRAGMENT_SHADER, fragSky);
//
//    unsigned int vertModel, fragModel;
//    CreateShader("vertModel.shader", GL_VERTEX_SHADER, vertModel);
//    CreateShader("fragModel.shader", GL_FRAGMENT_SHADER, fragModel);
//
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
//
//    myProgram = glCreateProgram();
//    glAttachShader(myProgram, vertID);
//    glAttachShader(myProgram, fragID);
//    glLinkProgram(myProgram);
//
//    skyProgram = glCreateProgram();
//    glAttachShader(skyProgram, vertSky);
//    glAttachShader(skyProgram, fragSky);
//    glLinkProgram(skyProgram);
//
//    modelProgram = glCreateProgram();
//    glAttachShader(modelProgram, vertModel);
//    glAttachShader(modelProgram, fragModel);
//    glLinkProgram(modelProgram);
//
//    glDeleteShader(vertID);
//    glDeleteShader(fragID);
//    glDeleteShader(vertSky);
//    glDeleteShader(fragSky);
//    glDeleteShader(fragModel);
//    glDeleteShader(vertModel);
//    /// END SETUP SHADER PROGRAM ///
//    glUseProgram(myProgram);
//    glUniform1i(glGetUniformLocation(myProgram, "heightmap"), 0);
//    glUniform1i(glGetUniformLocation(myProgram, "normalMap"), 1);
//
//    glUniform1i(glGetUniformLocation(myProgram, "dirt"), 2);
//    glUniform1i(glGetUniformLocation(myProgram, "sand"), 3);
//    glUniform1i(glGetUniformLocation(myProgram, "grass"), 4);
//}
//
//void RenderModel(Model* model, unsigned int shader, glm::vec3 position, glm::vec3 rotation, float scale, glm::mat4 view, glm::mat4 projection)
//{
//    // shader
//    glUseProgram(shader);
//
//    // world matrix
//    glm::mat4 world = glm::mat4(1);
//    world = glm::translate(world, position);
//    world = glm::scale(world, glm::vec3(scale));
//    glm::quat q(rotation);
//    world = world * glm::toMat4(q);
//
//    // setup shader
//    glUniformMatrix4fv(glGetUniformLocation(shader, "world"), 1, GL_FALSE, glm::value_ptr(world));
//    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
//    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, glm::value_ptr(cameraPosition));
//
//    // sun
//    float t = glfwGetTime();
//    glUniform3f(glGetUniformLocation(shader, "lightDirection"), glm::cos(t), -0.5f, glm::sin(t));
//
//    model->Draw(shader);
//}
//
//unsigned int GeneratePlane(const char* heightmap, GLenum format, int comp, float hScale, float xzScale, unsigned int& size, unsigned int& heightmapID) {
//    unsigned char* data = nullptr;
//    int channels;
//
//    if (heightmap != nullptr) {
//        data = stbi_load(heightmap, &heightMapWidth, &heightMapHeight, &channels, comp);
//        if (data) {
//            glGenTextures(1, &heightmapID);
//
//            glBindTexture(GL_TEXTURE_2D, heightmapID);
//
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//            glTexImage2D(GL_TEXTURE_2D, 0, format, heightMapWidth, heightMapHeight, 0, format, GL_UNSIGNED_BYTE, data);
//            glGenerateMipmap(GL_TEXTURE_2D);
//            glBindTexture(GL_TEXTURE_2D, 0);
//        }
//        else
//        {
//            printf("DATA IS NULL");
//            return NULL;
//        }
//    }
//    else
//    {
//        printf("HEIGHTMAP IS NULL");
//        return NULL;
//    }
//
//    GenerateModelInfoStats();
//
//    int stride = 8;
//    float* vertices = new float[(heightMapWidth * heightMapHeight) * stride];
//    unsigned int* indices = new unsigned int[(heightMapWidth - 1) * (heightMapHeight - 1) * 6];
//
//    int index = 0;
//
//    // Set y-position for the models to spawn
//    for (int i = 0; i < modelInfoSize; i++)
//    {
//        int y = (data[((int)modelInfos[i].position.x * heightMapWidth + (int)modelInfos[i].position.z) * channels] * hScale) / 255;
//
//        printf("X: %d, Y: %d, Z: %d", modelInfos[i].position.x, y, modelInfos[i].position.z);
//
//        modelInfos[i].position.y = y;
//    }
//
//    for (int i = 0; i < (heightMapWidth * heightMapHeight); i++) {
//        int x = i % heightMapWidth;
//        int z = i / heightMapWidth;
//
//        vertices[index++] = x * xzScale;
//        vertices[index++] = 0;
//        vertices[index++] = z * xzScale;
//
//        vertices[index++] = 0;
//        vertices[index++] = 1;
//        vertices[index++] = 0;
//
//        vertices[index++] = x / (float)(heightMapWidth - 1);
//        vertices[index++] = z / (float)(heightMapHeight - 1);
//    }
//
//    index = 0;
//
//    for (int i = 0; i < (heightMapWidth - 1) * (heightMapHeight - 1); i++) {
//        int x = i % (heightMapWidth - 1);
//        int z = i / (heightMapWidth - 1);
//
//        int vertex = z * heightMapWidth + x;
//
//        indices[index++] = vertex;
//        indices[index++] = vertex + heightMapWidth + 1;
//        indices[index++] = vertex + 1;
//
//        indices[index++] = vertex;
//        indices[index++] = vertex + heightMapWidth;
//        indices[index++] = vertex + heightMapWidth + 1;
//    }
//
//    unsigned int vertSize = (heightMapWidth * heightMapHeight) * stride * sizeof(float);
//    size = ((heightMapWidth - 1) * (heightMapHeight - 1) * 6) * sizeof(unsigned int);
//
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
//
//    // Vertex Info
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, 0);
//    glEnableVertexAttribArray(0);
//    // normal
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 3));
//    glEnableVertexAttribArray(1);
//    // uv
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 6));
//    glEnableVertexAttribArray(2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(0);
//
//    delete[] vertices;
//    delete[] indices;
//
//    stbi_image_free(data);
//
//    return VAO;
//}
//
//void GenerateModelInfoStats()
//{
//    for (int i = 0; i < modelInfoSize; i++)
//    {
//        modelInfos[i].modelID = RandomNumber(0, amountOfModels);
//        modelInfos[i].rotation = vec3(0, RandomNumber(0, 360), 0);
//        modelInfos[i].scale = RandomNumber(2, 10);
//        modelInfos[i].position = vec3(RandomNumber(0, heightMapWidth), 0, RandomNumber(0, heightMapHeight));
//    }
//}
//
//int RandomNumber(int min, int max)
//{
//    return (rand() % (max - min)) + min;
//}