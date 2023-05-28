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

// Camera & Mouse Settings
double cursorX = -1, cursorY = -1, lastCursorX, lastCursorY;
int cameraSensitivity = 15;
int cameraSpeed = 2;
vec3 cameraPosition(0, 0, 10), cameraForward(0, 0, -1), cameraUp(0, 1, 0);
bool showCursor = true;

// Time Settings
double previousT = 0;

// Objects & Light Settings
int index = 0;
int selectedSceneObject = 0;
bool updateLightSettings = true;
bool updateBoxSettings = false;

enum class objectType { DirLight = 0, PointLight = 1, Box = 2 };

struct Object
{
	objectType type = objectType::DirLight;
	int index = 0; // Index per type
	string name = "";
};

struct ObjectSystem
{
	vector<Object> objects;

	void AddObject(Object _object)
	{
		objects.push_back(_object);
	}

	void RemoveObject(objectType _type, int _index)
	{
		objects.erase(objects.begin() + _index);

		for (int i = _index; i < objects.size(); i++)
		{
			if (objects[i].type != _type) continue;

			int newIndex = objects[i].index - 1;

			objects[i].index = newIndex;
		}
	}
};

struct BoxInfo
{
	vec3 position = vec3(0.0f);
	float angleX = 0.0f;
	float angleY = 0.0f;
	bool autoRotation = false;
};

struct BoxSystem
{
	vector<BoxInfo> boxes;

	void AddBox(const BoxInfo& _box)
	{
		boxes.push_back(_box);
	}

	void RemoveBox(int _index)
	{
		boxes.erase(boxes.begin() + _index);
	}
};

struct LightInfo
{
	vec3 direction = vec3(0.0f);
	vec3 position = vec3(0.0f);
	vec3 ambientColor = vec3(0.0f);
	vec3 diffuseColor = vec3(0.0f);
	vec3 specular = vec3(1.0f);
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

struct LightPropertyNames
{
	string directionName;
	string positionName;
	string ambientName;
	string diffuseName;
	string specularName;

	string constantName;
	string linearName;
	string quadraticName;
};

struct LightingSystem
{
	vector<LightInfo> lightPropertiesList;
	vector<LightPropertyNames> lightPropertyNamesList;

	void AddLight(int _index, LightInfo _currentLightInfo) {
		LightInfo lightInfo;
		LightPropertyNames lightPropertyNames;

		lightPropertyNames.directionName = "pointLights[" + std::to_string(_index) + "].direction";
		lightPropertyNames.positionName = "pointLights[" + std::to_string(_index) + "].position";
		lightPropertyNames.ambientName = "pointLights[" + std::to_string(_index) + "].ambientColor";
		lightPropertyNames.diffuseName = "pointLights[" + std::to_string(_index) + "].diffuseColor";
		lightPropertyNames.specularName = "pointLights[" + std::to_string(_index) + "].specular";
		lightPropertyNames.constantName = "pointLights[" + std::to_string(_index) + "].constant";
		lightPropertyNames.linearName = "pointLights[" + std::to_string(_index) + "].linear";
		lightPropertyNames.quadraticName = "pointLights[" + std::to_string(_index) + "].quadratic";

		lightPropertiesList.push_back(lightInfo);
		lightPropertyNamesList.push_back(lightPropertyNames);

		_currentLightInfo = lightInfo;
		updateLightSettings = true;
	}

	void AddDirectionalLight()
	{
		LightInfo directionalLightInfo;
		LightPropertyNames directionalLightPropertyNames;

		directionalLightInfo.ambientColor = vec3(1.0f);

		directionalLightPropertyNames.directionName = "directionalLight.direction";
		directionalLightPropertyNames.ambientName = "directionalLight.ambientColor";
		directionalLightPropertyNames.diffuseName = "directionalLight.diffuseColor";
		directionalLightPropertyNames.specularName = "directionalLight.specular";

		lightPropertiesList.push_back(directionalLightInfo);
		lightPropertyNamesList.push_back(directionalLightPropertyNames);
	}

	void RemoveLight(int _index)
	{
		lightPropertiesList.erase(lightPropertiesList.begin() + _index);
		lightPropertyNamesList.erase(lightPropertyNamesList.begin() + _index);

		for (int i = _index; i < lightPropertiesList.size(); i++)
		{
			int newIndex = i - 1;
			const char* str = "pointLights[%i].%s";

			lightPropertyNamesList[i].directionName = "pointLights[" + std::to_string(newIndex) + "].direction";
			lightPropertyNamesList[i].positionName = "pointLights[" + std::to_string(newIndex) + "].position";
			lightPropertyNamesList[i].ambientName = "pointLights[" + std::to_string(newIndex) + "].ambientColor";
			lightPropertyNamesList[i].diffuseName = "pointLights[" + std::to_string(newIndex) + "].diffuseColor";
			lightPropertyNamesList[i].specularName = "pointLights[" + std::to_string(newIndex) + "].specular";
			lightPropertyNamesList[i].constantName = "pointLights[" + std::to_string(newIndex) + "].constant";
			lightPropertyNamesList[i].linearName = "pointLights[" + std::to_string(newIndex) + "].linear";
			lightPropertyNamesList[i].quadraticName = "pointLights[" + std::to_string(newIndex) + "].quadratic";
		}
	}

	void SetUniforms(unsigned int& _shader)
	{
		for (int i = 0; i < lightPropertiesList.size(); i++)
		{
			int directionalLightDirection = glGetUniformLocation(_shader, lightPropertyNamesList[i].directionName.c_str());
			int pointLightPosition = glGetUniformLocation(_shader, lightPropertyNamesList[i].positionName.c_str());
			int pointLightAmbientColor = glGetUniformLocation(_shader, lightPropertyNamesList[i].ambientName.c_str());
			int pointLightDiffuseColor = glGetUniformLocation(_shader, lightPropertyNamesList[i].diffuseName.c_str());
			int pointLightSpecular = glGetUniformLocation(_shader, lightPropertyNamesList[i].specularName.c_str());

			int pointLightConstant = glGetUniformLocation(_shader, lightPropertyNamesList[i].constantName.c_str());
			int pointLightLinear = glGetUniformLocation(_shader, lightPropertyNamesList[i].linearName.c_str());
			int pointLightQuadratic = glGetUniformLocation(_shader, lightPropertyNamesList[i].quadraticName.c_str());

			glUniform3f(pointLightPosition, lightPropertiesList[i].position.x, lightPropertiesList[i].position.y, lightPropertiesList[i].position.z);
			glUniform3f(pointLightAmbientColor, lightPropertiesList[i].ambientColor.x, lightPropertiesList[i].ambientColor.y, lightPropertiesList[i].ambientColor.z);
			glUniform3f(pointLightDiffuseColor, lightPropertiesList[i].diffuseColor.x, lightPropertiesList[i].diffuseColor.y, lightPropertiesList[i].diffuseColor.z);
			glUniform3f(pointLightSpecular, lightPropertiesList[i].specular.x, lightPropertiesList[i].specular.y, lightPropertiesList[i].specular.z);
			glUniform3f(directionalLightDirection, lightPropertiesList[i].direction.x, lightPropertiesList[i].direction.y, lightPropertiesList[i].direction.z);

			glUniform1f(pointLightConstant, lightPropertiesList[i].constant);
			glUniform1f(pointLightLinear, lightPropertiesList[i].linear);
			glUniform1f(pointLightQuadratic, lightPropertiesList[i].quadratic);
		}
	}
};

int Clamp(int _value, int _minVal, int _maxVal) {
	if (_value < _minVal)
	{
		return _minVal;
	}
	else if (_value > _maxVal)
	{
		return _maxVal;
	}
	else
	{
		return _value;
	}
}

void Key_Callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_RELEASE) return; // Only handle press events

	if (_key == GLFW_KEY_ESCAPE)
	{
		showCursor = !showCursor;
	}
}

void HandleInput(GLFWwindow* _window, float _deltaTime) {
	static bool w, s, a, d, space, ctrl;
	static float pitch = 0, yaw = 180 * cameraForward.z;
	float sensitivity = cameraSensitivity * _deltaTime;
	float speed = cameraSpeed * _deltaTime;

	lastCursorX = cursorX;
	lastCursorY = cursorY;
	glfwGetCursorPos(_window, &cursorX, &cursorY);
	glm::vec2 mouseDelta(cursorX - lastCursorX, cursorY - lastCursorY);

	if (showCursor) return;

	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) w = true; // FORWARD
	else if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_RELEASE) w = false;
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) s = true; // BACKWARDS
	else if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_RELEASE) s = false;
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) a = true; // LEFT
	else if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_RELEASE) a = false;
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) d = true; // RIGHT
	else if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_RELEASE) d = false;

	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) space = true; // UP
	else if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_RELEASE) space = false;
	if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ctrl = true; // DOWN
	else if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) ctrl = false;

	// calculate rotation & movement
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
	if (w) translation.z += speed;
	if (s) translation.z -= speed;
	if (a) translation.x += speed;
	if (d) translation.x -= speed;
	if (space) translation.y += speed;
	if (ctrl) translation.y -= speed;
	cameraPosition += q * translation;
	cameraUp = q * glm::vec3(0, 1, 0);
	cameraForward = q * glm::vec3(0, 0, 1);
}

void LoadFromFile(const char* _url, char** _target) {
	ifstream stream(_url, ios::binary);
	stream.seekg(0, stream.end);
	int total = stream.tellg();
	*_target = new char[total + 1];
	stream.seekg(0, stream.beg);
	stream.read(*_target, total);
	(*_target)[total] = '\0';
	stream.close();
}

unsigned int LoadTexture(string _url, GLenum _format) {
	// gen & bind IDs
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// instellen texture params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	int width, height, channels;
	unsigned char* data;
	data = stbi_load(_url.c_str(), &width, &height, &channels, 0);
	if (data == nullptr) {
		cout << "Error loading file: " << _url.c_str() << endl;
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, _format, width, height, 0, _format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

int main()
{
	LightingSystem lightingSystem;
	BoxSystem boxSystem;
	ObjectSystem objectSystem;
	BoxInfo currentBoxInfo;
	LightInfo currentLightInfo;

	// GLFW Setup //
	glfwInit();

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Final Assignment", NULL, NULL); // MAXIMIZED WINDOWED
	glfwSetWindowMonitor(window, NULL, 0, 0, 0, 0, mode->refreshRate); // MAXIMIZED WINDOWED

	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Final Assignment", primaryMonitor, NULL); // FULL SCREEN
	//glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate); // FULL SCREEN

	glfwMakeContextCurrent(window);
	glfwMaximizeWindow(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	int stride = sizeof(float) * 11;
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(1);
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 8));
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
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	};
	glCompileShader(fragID);
	glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
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
	int cubeColorLoc = glGetUniformLocation(myProgram, "cubeColor");
	int numPointLightsLocation = glGetUniformLocation(myProgram, "numPointLights");
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
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGuiWindowFlags crosshair_flags = 0;
	crosshair_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	crosshair_flags |= ImGuiWindowFlags_NoCollapse;
	crosshair_flags |= ImGuiWindowFlags_NoResize;
	crosshair_flags |= ImGuiWindowFlags_NoMove;
	crosshair_flags |= ImGuiWindowFlags_NoBackground;
	crosshair_flags |= ImGuiWindowFlags_NoTitleBar;

	// ADD DIRECTIONAL LIGHT
	lightingSystem.AddDirectionalLight();

	objectSystem.AddObject(Object{ objectType::DirLight, 0, "Directional Light" });

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		double time = glfwGetTime();
		float deltaTime = time - previousT;
		previousT = time;
		HandleInput(window, deltaTime);

		float spacing = GetStyle().ItemInnerSpacing.x;

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

		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(65.0f), mode->width / (float)mode->height, 0.1f, 1000.0f);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(camLoc, 1, glm::value_ptr(cameraPosition));
		glUniform1i(numPointLightsLocation, lightingSystem.lightPropertiesList.size() - 1);

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();

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

		// OBJECTS //
		// BOXES //
		for (int i = 0; i < boxSystem.boxes.size(); i++)
		{
			glm::mat4 world = glm::mat4(1.f);
			world = glm::translate(world, boxSystem.boxes[i].position);
			world = glm::rotate(world, glm::radians((boxSystem.boxes[i].autoRotation ? (float)time : 1.0f) * boxSystem.boxes[i].angleX), glm::vec3(1, 0, 0));
			world = glm::rotate(world, glm::radians((boxSystem.boxes[i].autoRotation ? (float)time : 1.0f) * boxSystem.boxes[i].angleY), glm::vec3(0, 1, 0));
			world = glm::scale(world, glm::vec3(1.0f));
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
			glUniform3fv(cubeColorLoc, 1, glm::value_ptr(vec3(-1.0f)));
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		}

		// FLOOR //
		glm::mat4 world = glm::mat4(1.f);
		world = glm::translate(world, glm::vec3(0.0f, -5.0f, 0.0f));
		world = glm::scale(world, glm::vec3(25, 1, 25));
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
		glUniform3fv(cubeColorLoc, 1, glm::value_ptr(vec3(-1.0f)));
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		// LIGHT CUBES //
		if (lightingSystem.lightPropertiesList.size() > 1)
		{
			for (int i = 1; i < lightingSystem.lightPropertiesList.size(); i++)
			{
				glm::mat4 world = glm::mat4(1.f);
				world = glm::translate(world, glm::vec3(lightingSystem.lightPropertiesList[i].position[0], lightingSystem.lightPropertiesList[i].position[1], lightingSystem.lightPropertiesList[i].position[2]));
				world = glm::scale(world, glm::vec3(0.25f, 0.25f, 0.25f));
				glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
				glUniform3fv(cubeColorLoc, 1, glm::value_ptr(lightingSystem.lightPropertiesList[i].diffuseColor));
				glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
			}
		}
		// END OBJECTS //

		ImGuiStyle& style = GetStyle();
		style.FrameRounding = 3.0f;
		style.WindowRounding = 3.0f;

		// ImGui UI Elements //
		Begin("Crosshair", NULL, crosshair_flags);
		SetWindowFontScale(3.0f);
		ImVec2 windowSize = GetWindowSize();
		SetWindowPos(ImVec2((mode->width / 2) - windowSize.x / 2, (mode->height / 2) - windowSize.y / 2));
		Text("+");
		End();

		Begin("Options", NULL, window_flags);
		ImVec2 newLightSettingsWindowSize = GetWindowSize();
		ImVec2 newLightSettingsWindowPosition = ImVec2(25, 25);
		SetWindowPos(newLightSettingsWindowPosition);
		SetWindowFontScale(1.25f);

		Text("Camera Sensitivity");
		SameLine();
		SetCursorPosX(177.5f);
		PushItemWidth(90);
		DragInt("##CameraSensitivity", &cameraSensitivity, 0.1f, 0, 100);

		Text("Camera Speed");
		SameLine();
		SetCursorPosX(177.5f);
		PushItemWidth(90);
		DragInt("##CameraSpeed", &cameraSpeed, 0.1f, 0, 100);

		if (Button("Add Point Light", ImVec2(260, 20)))
		{
			int i = lightingSystem.lightPropertiesList.size() - 1;
			lightingSystem.AddLight(i, currentLightInfo);

			objectSystem.AddObject(Object{ objectType::PointLight, i + 1, "Point Light" });

			index = objectSystem.objects.size() - 1;
			selectedSceneObject = objectSystem.objects.size() - 1;
			updateLightSettings = true;
		}

		if (Button("Add Box", ImVec2(260, 20)))
		{
			currentBoxInfo = BoxInfo();
			int i = boxSystem.boxes.size();
			boxSystem.AddBox(currentBoxInfo);

			objectSystem.AddObject(Object{ objectType::Box, i, "Box" });

			index = objectSystem.objects.size() - 1;
			selectedSceneObject = objectSystem.objects.size() - 1;
			updateBoxSettings = true;
		}

		End();

		Begin("Objects", NULL, window_flags);
		ImVec2 objectsSettingsWindowSize = GetWindowSize();
		ImVec2 objectsSettingsWindowPosition = ImVec2(25, newLightSettingsWindowPosition.y + newLightSettingsWindowSize.y + 25);
		SetWindowPos(objectsSettingsWindowPosition);
		SetWindowFontScale(1.25f);

		if (TreeNodeEx("Scene Objects", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::BeginChild("##scrollingregion", ImVec2(200, 200));

			for (int i = 0; i < objectSystem.objects.size(); i++)
			{
				if (Selectable((objectSystem.objects[i].name + " " + to_string(objectSystem.objects[i].index)).c_str(), selectedSceneObject == i))
				{
					index = i;
					selectedSceneObject = i;
					updateLightSettings = true;
					updateBoxSettings = true;
				}
			}

			EndChild();
			TreePop();
		}

		End();

		Begin("Adjust Object | Settings", NULL, window_flags);
		ImVec2 adjustObjectSettingsWindowSize = GetWindowSize();
		ImVec2 adjustObjectSettingsWindowPosition = ImVec2(25, objectsSettingsWindowPosition.y + objectsSettingsWindowSize.y + 25);
		SetWindowPos(adjustObjectSettingsWindowPosition);
		SetWindowFontScale(1.25f);

		Text("Current Object: %s", objectSystem.objects[index].name.c_str());

		// Update Current Light or Box Settings
		if (updateLightSettings || updateBoxSettings)
		{
			if (objectSystem.objects[index].type != objectType::Box)
			{
				currentLightInfo = lightingSystem.lightPropertiesList[objectSystem.objects[index].index];
			}
			else if (boxSystem.boxes.size() > 0 && objectSystem.objects[index].type == objectType::Box)
			{
				currentBoxInfo = boxSystem.boxes[objectSystem.objects[index].index];
			}

			updateLightSettings = false;
			updateBoxSettings = false;
		}

		if (objectSystem.objects[index].type != objectType::Box)
		{
			if (index == 0) DragFloat3("Light Direction", value_ptr(currentLightInfo.direction));
			if (index != 0) DragFloat3("Light Position", value_ptr(currentLightInfo.position));
			ColorEdit3("Ambient Color", value_ptr(currentLightInfo.ambientColor));
			ColorEdit3("Diffuse Color", value_ptr(currentLightInfo.diffuseColor));
			DragFloat3("Specular", value_ptr(currentLightInfo.specular));

			if (index != 0)
			{
				DragFloat("Constant", &currentLightInfo.constant);
				DragFloat("Linear", &currentLightInfo.linear);
				DragFloat("Quadratic", &currentLightInfo.quadratic);
			}

			lightingSystem.lightPropertiesList[objectSystem.objects[index].index] = currentLightInfo;

			if (index != 0)
			{
				if (Button("Remove Light", ImVec2(260, 20)))
				{
					lightingSystem.RemoveLight(objectSystem.objects[index].index);
					objectSystem.RemoveObject(objectSystem.objects[index].type, index);
					if (index > objectSystem.objects.size()) index--;
					updateLightSettings = true;
				}
			}
		}

		if (boxSystem.boxes.size() > 0 && objectSystem.objects[index].type == objectType::Box)
		{
			DragFloat3("Box Position", value_ptr(currentBoxInfo.position));
			DragFloat("Box X Rotation", &currentBoxInfo.angleX);
			DragFloat("Box Y Rotation", &currentBoxInfo.angleY);
			Checkbox("Box Auto Rotation", &currentBoxInfo.autoRotation);

			boxSystem.boxes[objectSystem.objects[index].index] = currentBoxInfo;

			if (Button("Remove Box", ImVec2(260, 20)))
			{
				boxSystem.RemoveBox(objectSystem.objects[index].index);
				objectSystem.RemoveObject(objectSystem.objects[index].type, index);
				if (index >= objectSystem.objects.size()) index--;
				updateBoxSettings = true;
			}
		}

		End();

		//Text(currentLightIndex == 0 ? "Directional Light" : "Spot Light");
		//Text("Current Light:");
		//SameLine(0.0f, spacing);

		//PushButtonRepeat(true);
		//if (ArrowButton("#leftLightIndex", ImGuiDir_Left))
		//{
		//	if (lightingSystem.lightPropertiesList.size() > 0)
		//	{
		//		currentLightIndex--;
		//		currentLightIndex = Clamp(currentLightIndex, 0, lightingSystem.lightPropertiesList.size() - 1);
		//		updateLightSettings = true;
		//	}
		//}

		//SameLine(0.0f, spacing);
		//Text("%d", currentLightIndex);
		//SameLine(0.0f, spacing);

		//if (ArrowButton("#rightLightIndex", ImGuiDir_Right))
		//{
		//	if (lightingSystem.lightPropertiesList.size() > 0)
		//	{
		//		currentLightIndex++;
		//		currentLightIndex = Clamp(currentLightIndex, 0, lightingSystem.lightPropertiesList.size() - 1);
		//		updateLightSettings = true;
		//	}
		//}
		//PopButtonRepeat();

		//// Update Current Light Settings
		//if (updateLightSettings)
		//{
		//	if (lightingSystem.lightPropertiesList.size() > 0)
		//	{
		//		currentLightInfo = lightingSystem.lightPropertiesList[currentLightIndex];
		//	}

		//	updateLightSettings = false;
		//}


		//End();

		//Begin("Adjust Box | Settings", NULL, window_flags);
		//ImVec2 adjustBoxSettingsWindowSize = GetWindowSize();
		//ImVec2 adjustBoxSettingsWindowPosition = ImVec2(25, adjustLightSettingsWindowPosition.y + adjustLightSettingsWindowSize.y + 25);
		//SetWindowPos(adjustBoxSettingsWindowPosition);
		//SetWindowFontScale(1.25f);

		//Text("Current Box:");
		//SameLine(0.0f, spacing);

		//PushButtonRepeat(true);
		//if (ArrowButton("#leftBoxIndex", ImGuiDir_Left))
		//{
		//	if (boxSystem.boxes.size() > 0)
		//	{
		//		currentBoxIndex--;
		//		currentBoxIndex = Clamp(currentBoxIndex, 0, boxSystem.boxes.size() - 1);
		//		updateBoxSettings = true;
		//	}
		//}

		//SameLine(0.0f, spacing);
		//Text("%d", currentBoxIndex);
		//SameLine(0.0f, spacing);

		//if (ArrowButton("#rightBoxIndex", ImGuiDir_Right))
		//{
		//	if (boxSystem.boxes.size() > 0)
		//	{
		//		currentBoxIndex++;
		//		currentBoxIndex = Clamp(currentBoxIndex, 0, boxSystem.boxes.size() - 1);
		//		updateBoxSettings = true;
		//	}
		//}
		//PopButtonRepeat();

		//// Update Current Box Settings
		//if (updateBoxSettings)
		//{
		//	if (boxSystem.boxes.size() > 0)
		//	{
		//		currentBoxInfo = boxSystem.boxes[currentBoxIndex];
		//	}

		//	updateBoxSettings = false;
		//}

		//if (boxSystem.boxes.size() > 0)
		//{
		//	DragFloat3("Box Position", value_ptr(currentBoxInfo.position));
		//	DragFloat("Box X Rotation", &currentBoxInfo.angleX);
		//	DragFloat("Box Y Rotation", &currentBoxInfo.angleY);
		//	Checkbox("Box Auto Rotation", &currentBoxInfo.autoRotation);

		//	boxSystem.boxes[currentBoxIndex] = currentBoxInfo;

		//	if (Button("Remove Box", ImVec2(260, 20)))
		//	{
		//		boxSystem.RemoveBox(currentBoxIndex);
		//		if (currentBoxIndex > 0) currentBoxIndex--;
		//		updateBoxSettings = true;
		//	}
		//}

		//End();

		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

		lightingSystem.SetUniforms(myProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}