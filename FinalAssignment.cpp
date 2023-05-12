#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <string>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "utils.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
using namespace glm;
using namespace std;
using namespace ImGui;

vec3 cameraPosition(0, 0, -10), cameraForward(0, 0, 1), cameraUp(0, 1, 0);
bool showCursor = true;

struct LightInfo
{
	vec3 position;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct LightPropertyNames
{
	const char* positionName;
	const char* ambientName;
	const char* diffuseName;
	const char* specularName;

	const char* constantName;
	const char* linearName;
	const char* quadraticName;
};

struct LightingSystem
{
	vector<LightInfo> lightPropertiesList;
	vector<LightPropertyNames> lightPropertyNamesList;

	void AddLight(const LightInfo& light, const LightPropertyNames& names) {
		lightPropertiesList.push_back(light);
		lightPropertyNamesList.push_back(names);
	}

	void RemoveLight(int index)
	{
		lightPropertiesList.erase(lightPropertiesList.begin() + index);
		lightPropertyNamesList.erase(lightPropertyNamesList.begin() + index);

		for (int i = index; i < lightPropertiesList.size(); i++)
		{
			const char* str = "pointLights[%d].%s";

			char* positionBuffer = (char*)malloc(50 * sizeof(char));
			char* ambientBuffer = (char*)malloc(50 * sizeof(char));
			char* diffuseBuffer = (char*)malloc(50 * sizeof(char));
			char* specularBuffer = (char*)malloc(50 * sizeof(char));
			char* constantBuffer = (char*)malloc(50 * sizeof(char));
			char* linearBuffer = (char*)malloc(50 * sizeof(char));
			char* quadraticBuffer = (char*)malloc(50 * sizeof(char));

			std::snprintf(positionBuffer, 50, str, i, "position");
			lightPropertyNamesList[i].positionName = positionBuffer;
			std::snprintf(ambientBuffer, 50, str, i, "ambientColor");
			lightPropertyNamesList[i].ambientName = ambientBuffer;
			std::snprintf(diffuseBuffer, 50, str, i, "diffuseColor");
			lightPropertyNamesList[i].diffuseName = diffuseBuffer;
			std::snprintf(specularBuffer, 50, str, i, "specular");
			lightPropertyNamesList[i].specularName = specularBuffer;
			std::snprintf(constantBuffer, 50, str, i, "constant");
			lightPropertyNamesList[i].constantName = constantBuffer;
			std::snprintf(linearBuffer, 50, str, i, "linear");
			lightPropertyNamesList[i].linearName = linearBuffer;
			std::snprintf(quadraticBuffer, 50, str, i, "quadratic");
			lightPropertyNamesList[i].quadraticName = quadraticBuffer;
		}
	}

	void SetUniforms(unsigned int& shader)
	{
		for (int i = 0; i < lightPropertiesList.size(); i++)
		{
			int pointLightPosition = glGetUniformLocation(shader, lightPropertyNamesList[i].positionName);
			int pointLightAmbientColor = glGetUniformLocation(shader, lightPropertyNamesList[i].ambientName);
			int pointLightDiffuseColor = glGetUniformLocation(shader, lightPropertyNamesList[i].diffuseName);
			int pointLightSpecular = glGetUniformLocation(shader, lightPropertyNamesList[i].specularName);

			int pointLightConstant = glGetUniformLocation(shader, lightPropertyNamesList[i].constantName);
			int pointLightLinear = glGetUniformLocation(shader, lightPropertyNamesList[i].linearName);
			int pointLightQuadratic = glGetUniformLocation(shader, lightPropertyNamesList[i].quadraticName);

			glUniform3f(pointLightPosition, lightPropertiesList[i].position.x, lightPropertiesList[i].position.y, lightPropertiesList[i].position.z);
			glUniform3f(pointLightAmbientColor, lightPropertiesList[i].ambientColor.x, lightPropertiesList[i].ambientColor.y, lightPropertiesList[i].ambientColor.z);
			glUniform3f(pointLightDiffuseColor, lightPropertiesList[i].diffuseColor.x, lightPropertiesList[i].diffuseColor.y, lightPropertiesList[i].diffuseColor.z);
			glUniform3f(pointLightSpecular, lightPropertiesList[i].specular.x, lightPropertiesList[i].specular.y, lightPropertiesList[i].specular.z);

			glUniform1f(pointLightConstant, lightPropertiesList[i].constant);
			glUniform1f(pointLightLinear, lightPropertiesList[i].linear);
			glUniform1f(pointLightQuadratic, lightPropertiesList[i].quadratic);
		}
	}
};

int Clamp(int value, int minVal, int maxVal) {
	if (value < minVal) 
	{
		return minVal;
	}
	else if (value > maxVal) 
	{
		return maxVal;
	}
	else 
	{
		return value;
	}
}

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) return; // Only handle press events

	if (key == GLFW_KEY_ESCAPE)
	{
		showCursor = !showCursor;
	}
}

void HandleInput(GLFWwindow* window, float deltaTime) {
	static bool w, s, a, d, space, ctrl;
	static double cursorX = -1, cursorY = -1, lastCursorX, lastCursorY;
	static float pitch, yaw;
	static float speed = 1.0f;
	float sensitivity = 5.0f * deltaTime;
	float step = speed * deltaTime;

	if (showCursor) return;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) w = true; // FORWARD
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) w = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) s = true; // BACKWARDS
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) s = false;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) a = true; // LEFT
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) a = false;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) d = true; // RIGHT
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) d = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) space = true; // UP
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) space = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ctrl = true; // DOWN
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) ctrl = false;

	if (cursorX == -1) {
		glfwGetCursorPos(window, &cursorX, &cursorY);
	}

	lastCursorX = cursorX;
	lastCursorY = cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	glm::vec2 mouseDelta(cursorX - lastCursorX, cursorY - lastCursorY);
	// TODO: calculate rotation & movement
	yaw -= mouseDelta.x * sensitivity;
	pitch += mouseDelta.y * sensitivity;
	if (pitch < -90.0f) pitch = -90.0f;
	else if (pitch > 90.0f) pitch = 90.0f;
	if (yaw < -180.0f) yaw += 360;
	else if (yaw > 180.0f) yaw -= 360;
	glm::vec3 euler(glm::radians(pitch), glm::radians(yaw), 0);
	glm::quat q(euler);
	// update camera position / forward & up
	glm::vec3 translation(0, 0, 0);
	//implement movement
	if (w) translation.z += speed * deltaTime;
	if (s) translation.z -= speed * deltaTime;
	if (a) translation.x += speed * deltaTime;
	if (d) translation.x -= speed * deltaTime;
	if (space) translation.y += speed * deltaTime;
	if (ctrl) translation.y -= speed * deltaTime;
	cameraPosition += q * translation;
	cameraUp = q * glm::vec3(0, 1, 0);
	cameraForward = q * glm::vec3(0, 0, 1);
}

void LoadFromFile(const char* url, char** target) {
	ifstream stream(url, ios::binary);
	stream.seekg(0, stream.end);
	int total = stream.tellg();
	*target = new char[total + 1];
	stream.seekg(0, stream.beg);
	stream.read(*target, total);
	(*target)[total] = '\0';
	stream.close();
}

unsigned int LoadTexture(string url, GLenum format) {
	// gen & bind IDs
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// instellen texture params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	int width, height, channels;
	unsigned char* data;
	data = stbi_load(url.c_str(), &width, &height, &channels, 0);
	if (data == nullptr) {
		cout << "Error loading file: " << url.c_str() << endl;
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

int main()
{
	LightingSystem lightingSystem;

	static double previousT = 0;
	int currentLightIndex = 0;
	bool updateLightSettings = false;

	float lightPosition[3] = { 0.0f, 0.0f, 0.0f };
	float ambientColor[3] = { 0.0f, 0.0f, 0.0f };
	float diffuseColor[3] = { 0.0f, 0.0f, 0.0f };
	float specular[3] = { 1.0f, 1.0f, 1.0f };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float tempLightPosition[3] = { 0.0f, 0.0f, 0.0f };
	float tempAmbientColor[3] = { 0.0f, 0.0f, 0.0f };
	float tempDiffuseColor[3] = { 0.0f, 0.0f, 0.0f };
	float tempSpecular[3] = { 1.0f, 1.0f, 1.0f };
	float tempConstant = 1.0f;
	float tempLinear = 0.09f;
	float tempQuadratic = 0.032f;

	// GLFW Setup //
	glfwInit();

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//GLFWwindow* window = glfwCreateWindow(800, 800, "Les 3 | Tom Renkema", nullptr, nullptr);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Final Assignment", primaryMonitor, NULL);
	glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, mode->width, mode->height);
	glfwShowWindow(window);

	glfwSetKeyCallback(window, Key_Callback);

	// Setup Box //
	// Vertices of our triangle!
	// need 24 vertices for normal/uv-mapped Cube
	float vertices[] = {
		// positions            //colors            // tex coords   // normals
		0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,   1.f, 0.f,       0.f, -1.f, 0.f,
		0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 0.0f,   1.f, 1.f,       0.f, -1.f, 0.f,
		-0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, -1.f, 0.f,
		-0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, -1.f, 0.f,
		0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   2.f, 0.f,       1.f, 0.f, 0.f,
		0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   2.f, 1.f,       1.f, 0.f, 0.f,
		0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   1.f, 2.f,       0.f, 0.f, 1.f,
		-0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   0.f, 2.f,       0.f, 0.f, 1.f,
		-0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   -1.f, 1.f,      -1.f, 0.f, 0.f,
		-0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   -1.f, 0.f,      -1.f, 0.f, 0.f,
		-0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   0.f, -1.f,      0.f, 0.f, -1.f,
		0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   1.f, -1.f,      0.f, 0.f, -1.f,
		-0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   3.f, 0.f,       0.f, 1.f, 0.f,
		-0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   3.f, 1.f,       0.f, 1.f, 0.f,
		0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       0.f, 0.f, 1.f,
		-0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, 0.f, 1.f,
		-0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       -1.f, 0.f, 0.f,
		-0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       -1.f, 0.f, 0.f,
		-0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, 0.f, -1.f,
		0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.f, 0.f,       0.f, 0.f, -1.f,
		0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.f, 0.f,       1.f, 0.f, 0.f,
		0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       1.f, 0.f, 0.f,
		0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   2.f, 0.f,       0.f, 1.f, 0.f,
		0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   2.f, 1.f,       0.f, 1.f, 0.f
	};

	unsigned int indices[] = {  // note that we start from 0!
		// DOWN
		0, 1, 2,   // first triangle
		0, 2, 3,    // second triangle
		// BACK
		14, 6, 7,   // first triangle
		14, 7, 15,    // second triangle
		// RIGHT
		20, 4, 5,   // first triangle
		20, 5, 21,    // second triangle
		// LEFT
		16, 8, 9,   // first triangle
		16, 9, 17,    // second triangle
		// FRONT
		18, 10, 11,   // first triangle
		18, 11, 19,    // second triangle
		// UP
		22, 12, 13,   // first triangle
		22, 13, 23,    // second triangle
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);
	int stride = sizeof(float) * 11;
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
		6));
	glEnableVertexAttribArray(1);
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) *
		8));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	///END SETUP OBJECT///
	///SETUP SHADER PROGRAM///
	char* vertexSource;
	LoadFromFile("vertexShaderFinalAssignment.shader", &vertexSource);
	char* fragmentSource;
	LoadFromFile("fragmentShaderFinalAssignment.shader", &fragmentSource);
	// LOAD & CREATE TEXTURES

	unsigned int diffuseTexID = LoadTexture("container2.png", GL_RGBA);
	unsigned int specularTexID = LoadTexture("container2_specular.png", GL_RGBA);
	// END
	unsigned int vertID, fragID;
	vertID = glCreateShader(GL_VERTEX_SHADER);
	fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertID, 1, &vertexSource, nullptr);
	glShaderSource(fragID, 1, &fragmentSource, nullptr);
	int success;
	char infoLog[512];
	glCompileShader(vertID);
	glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
			endl;
	};
	glCompileShader(fragID);
	glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
			endl;
	};
	unsigned int myProgram = glCreateProgram();
	glAttachShader(myProgram, vertID);
	glAttachShader(myProgram, fragID);
	glLinkProgram(myProgram);
	glDeleteShader(vertID);
	glDeleteShader(fragID);
	///END SETUP SHADER PROGRAM///
	/// MATRIX SETUP ///
	glUseProgram(myProgram);
	int worldLoc = glGetUniformLocation(myProgram, "world");
	int viewLoc = glGetUniformLocation(myProgram, "view");
	int projLoc = glGetUniformLocation(myProgram, "projection");
	int camLoc = glGetUniformLocation(myProgram, "camera");
	int numPointLightsLocation = glGetUniformLocation(myProgram, "numPointLights");
	//int amountOfPointLights = glGetUniformLocation(myProgram, "nrOfPointLights");
	/// END MATRIX SETUP ///
	// OPENGL SETTINGS //
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);

	// Setup ImGui //
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO(); (void)io;
	StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Set ImGui Flags //
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;

	ImGuiWindowFlags crosshair_flags = 0;
	crosshair_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	crosshair_flags |= ImGuiWindowFlags_NoCollapse;
	crosshair_flags |= ImGuiWindowFlags_NoResize;
	crosshair_flags |= ImGuiWindowFlags_NoMove;
	crosshair_flags |= ImGuiWindowFlags_NoBackground;
	crosshair_flags |= ImGuiWindowFlags_NoTitleBar;

	while (!glfwWindowShouldClose(window)) {
		double time = glfwGetTime();
		float deltaTime = time - previousT;
		previousT = time;
		HandleInput(window, deltaTime);

		if (showCursor)
		{
			SetMouseCursor(ImGuiMouseCursor_Arrow);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			SetMouseCursor(ImGuiMouseCursor_None);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		glm::mat4 world = glm::mat4(1.f);
		world = glm::rotate(world, glm::radians((float)time * 45.0f), glm::vec3(1, 1, 1));
		world = glm::scale(world, glm::vec3(1, 1, 1));
		world = glm::translate(world, glm::vec3(0, 0, 0));
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(65.0f), mode->width / (float)mode->height, 0.01f, 1000.0f);
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(camLoc, 1, glm::value_ptr(cameraPosition));
		glUniform1i(numPointLightsLocation, lightingSystem.lightPropertiesList.size());

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Activate the shader
		glUseProgram(myProgram);

		int materialDiffuseSampler = glGetUniformLocation(myProgram, "material.diffuseSample");
		int materialSpecular = glGetUniformLocation(myProgram, "material.specular");
		int materialShine = glGetUniformLocation(myProgram, "material.shininess");
		glUniform1i(materialDiffuseSampler, 0.0f);
		glUniform1i(materialSpecular, 1.0f);
		glUniform1f(materialShine, 64.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexID);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 3.0f;
		style.WindowRounding = 3.0f;

		// ImGui UI Elements //
		Begin("Crosshair", NULL, crosshair_flags);
		SetWindowFontScale(3.0f);
		ImVec2 windowSize = GetWindowSize();
		SetWindowPos(ImVec2((mode->width / 2) - windowSize.x / 2, (mode->height / 2) - windowSize.y / 2));
		Text("+");
		End();

		Begin("Settings", NULL, window_flags);
		ImVec2 newLightSettingsWindowSize = GetWindowSize();
		ImVec2 newLightSettingsWindowPosition = ImVec2(50, 50);
		SetWindowPos(newLightSettingsWindowPosition);
		SetWindowFontScale(1.25f);

		if (Button("Add Light", ImVec2(260, 20)))
		{
			int index = lightingSystem.lightPropertiesList.size();

			// Reset All Light Values
			for (int i = 0; i < 3; i++)
			{
				lightPosition[i] = tempLightPosition[i];
				ambientColor[i] = tempAmbientColor[i];
				diffuseColor[i] = tempDiffuseColor[i];
				specular[i] = tempSpecular[i];
			}

			constant = tempConstant;
			linear = tempLinear;
			quadratic = tempQuadratic;

			// Set All Light Info
			LightInfo lightInfo;
			lightInfo.position = vec3(lightPosition[0], lightPosition[1], lightPosition[2]);
			lightInfo.ambientColor = vec3(ambientColor[0], ambientColor[1], ambientColor[2]);
			lightInfo.diffuseColor = vec3(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			lightInfo.specular = vec3(specular[0], specular[1], specular[2]);
			lightInfo.constant = constant;
			lightInfo.linear = linear;
			lightInfo.quadratic = quadratic;

			// Set All Light Property Names
			LightPropertyNames lightPropertyNames;
			const char* str = "pointLights[%d].%s";

			char* positionBuffer = (char*)malloc(50 * sizeof(char));
			char *ambientBuffer = (char*)malloc(50 * sizeof(char));
			char *diffuseBuffer = (char*)malloc(50 * sizeof(char));
			char *specularBuffer = (char*)malloc(50 * sizeof(char));
			char *constantBuffer = (char*)malloc(50 * sizeof(char));
			char *linearBuffer = (char*)malloc(50 * sizeof(char));
			char *quadraticBuffer = (char*)malloc(50 * sizeof(char));

			std::snprintf(positionBuffer, 50, str, index, "position");
			lightPropertyNames.positionName = positionBuffer;
			std::snprintf(ambientBuffer, 50, str, index, "ambientColor");
			lightPropertyNames.ambientName = ambientBuffer;
			std::snprintf(diffuseBuffer, 50, str, index, "diffuseColor");
			lightPropertyNames.diffuseName = diffuseBuffer;
			std::snprintf(specularBuffer, 50, str, index, "specular");
			lightPropertyNames.specularName = specularBuffer;
			std::snprintf(constantBuffer, 50, str, index, "constant");
			lightPropertyNames.constantName = constantBuffer;
			std::snprintf(linearBuffer, 50, str, index, "linear");
			lightPropertyNames.linearName = linearBuffer;
			std::snprintf(quadraticBuffer, 50, str, index, "quadratic");
			lightPropertyNames.quadraticName = quadraticBuffer;

			lightingSystem.AddLight(lightInfo, lightPropertyNames);

			currentLightIndex = index;
			updateLightSettings = true;
		}

		End();

		Begin("Adjust Light | Settings", NULL, window_flags);
		SetWindowPos(ImVec2(50, newLightSettingsWindowPosition.y + newLightSettingsWindowSize.y + 25));
		SetWindowFontScale(1.25f);
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

		Text("Current Light:");
		SameLine(0.0f, spacing);

		if (ArrowButton("##left", ImGuiDir_Left)) 
		{ 
			if (lightingSystem.lightPropertiesList.size() > 0)
			{
				currentLightIndex--;
				currentLightIndex = Clamp(currentLightIndex, 0, lightingSystem.lightPropertiesList.size() - 1);
				updateLightSettings = true;
			}
		}

		SameLine(0.0f, spacing);
		Text("%d", currentLightIndex);
		SameLine(0.0f, spacing);

		if (ArrowButton("##right", ImGuiDir_Right)) 
		{ 
			if (lightingSystem.lightPropertiesList.size() > 0)
			{
				currentLightIndex++;
				currentLightIndex = Clamp(currentLightIndex, 0, lightingSystem.lightPropertiesList.size() - 1);
				updateLightSettings = true;
			}
		}

		// Update Current Light Settings
		if (updateLightSettings)
		{
			if (lightingSystem.lightPropertiesList.size() > 0)
			{
				for (int i = 0; i < 3; i++)
				{
					lightPosition[i] = lightingSystem.lightPropertiesList[currentLightIndex].position[i];
					ambientColor[i] = lightingSystem.lightPropertiesList[currentLightIndex].ambientColor[i];
					diffuseColor[i] = lightingSystem.lightPropertiesList[currentLightIndex].diffuseColor[i];
					specular[i] = lightingSystem.lightPropertiesList[currentLightIndex].specular[i];
				}

				constant = lightingSystem.lightPropertiesList[currentLightIndex].constant;
				linear = lightingSystem.lightPropertiesList[currentLightIndex].linear;
				quadratic = lightingSystem.lightPropertiesList[currentLightIndex].quadratic;
			}

			updateLightSettings = false;
		}

		if (lightingSystem.lightPropertiesList.size() > 0)
		{
			InputFloat3("Light Position", lightPosition);
			ColorEdit3("Ambient Color", ambientColor);
			ColorEdit3("Diffuse Color", diffuseColor);
			InputFloat3("Specular", specular);
			InputFloat("Constant", &constant);
			InputFloat("Linear", &linear);
			InputFloat("Quadratic", &quadratic);

			lightingSystem.lightPropertiesList[currentLightIndex].position = vec3(lightPosition[0], lightPosition[1], lightPosition[2]);
			lightingSystem.lightPropertiesList[currentLightIndex].ambientColor = vec3(ambientColor[0], ambientColor[1], ambientColor[2]);
			lightingSystem.lightPropertiesList[currentLightIndex].diffuseColor = vec3(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			lightingSystem.lightPropertiesList[currentLightIndex].specular = vec3(specular[0], specular[1], specular[2]);
			lightingSystem.lightPropertiesList[currentLightIndex].constant = constant;
			lightingSystem.lightPropertiesList[currentLightIndex].linear = linear;
			lightingSystem.lightPropertiesList[currentLightIndex].quadratic = quadratic;

			if (Button("Remove Light", ImVec2(260, 20)))
			{
				lightingSystem.RemoveLight(currentLightIndex);
				if (currentLightIndex > 0) currentLightIndex--;
				updateLightSettings = true;
			}
		}

		End();

		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

		lightingSystem.SetUniforms(myProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}