//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <fstream>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include <vector>
//#include <string>
//#include <sstream>
//#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;
//
//// An array of 3 vectors which represents 3 vertices
//static const GLfloat g_vertex_buffer_data[] = {
//	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
//	-1.0f,-1.0f, 1.0f,
//	-1.0f, 1.0f, 1.0f, // triangle 1 : end
//	1.0f, 1.0f,-1.0f, // triangle 2 : begin
//	-1.0f,-1.0f,-1.0f,
//	-1.0f, 1.0f,-1.0f, // triangle 2 : end
//	1.0f,-1.0f, 1.0f,
//	-1.0f,-1.0f,-1.0f,
//	1.0f,-1.0f,-1.0f,
//	1.0f, 1.0f,-1.0f,
//	1.0f,-1.0f,-1.0f,
//	-1.0f,-1.0f,-1.0f,
//	-1.0f,-1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f,-1.0f,
//	1.0f,-1.0f, 1.0f,
//	-1.0f,-1.0f, 1.0f,
//	-1.0f,-1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f,
//	-1.0f,-1.0f, 1.0f,
//	1.0f,-1.0f, 1.0f,
//	1.0f, 1.0f, 1.0f,
//	1.0f,-1.0f,-1.0f,
//	1.0f, 1.0f,-1.0f,
//	1.0f,-1.0f,-1.0f,
//	1.0f, 1.0f, 1.0f,
//	1.0f,-1.0f, 1.0f,
//	1.0f, 1.0f, 1.0f,
//	1.0f, 1.0f,-1.0f,
//	-1.0f, 1.0f,-1.0f,
//	1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f,
//	1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f, 1.0f,
//	1.0f,-1.0f, 1.0f
//};
//
//// One color for each vertex. They were generated randomly.
//static const GLfloat g_color_buffer_data[] = {
//	0.583f,  0.771f,  0.014f,
//	0.609f,  0.115f,  0.436f,
//	0.327f,  0.483f,  0.844f,
//	0.822f,  0.569f,  0.201f,
//	0.435f,  0.602f,  0.223f,
//	0.310f,  0.747f,  0.185f,
//	0.597f,  0.770f,  0.761f,
//	0.559f,  0.436f,  0.730f,
//	0.359f,  0.583f,  0.152f,
//	0.483f,  0.596f,  0.789f,
//	0.559f,  0.861f,  0.639f,
//	0.195f,  0.548f,  0.859f,
//	0.014f,  0.184f,  0.576f,
//	0.771f,  0.328f,  0.970f,
//	0.406f,  0.615f,  0.116f,
//	0.676f,  0.977f,  0.133f,
//	0.971f,  0.572f,  0.833f,
//	0.140f,  0.616f,  0.489f,
//	0.997f,  0.513f,  0.064f,
//	0.945f,  0.719f,  0.592f,
//	0.543f,  0.021f,  0.978f,
//	0.279f,  0.317f,  0.505f,
//	0.167f,  0.620f,  0.077f,
//	0.347f,  0.857f,  0.137f,
//	0.055f,  0.953f,  0.042f,
//	0.714f,  0.505f,  0.345f,
//	0.783f,  0.290f,  0.734f,
//	0.722f,  0.645f,  0.174f,
//	0.302f,  0.455f,  0.848f,
//	0.225f,  0.587f,  0.040f,
//	0.517f,  0.713f,  0.338f,
//	0.053f,  0.959f,  0.120f,
//	0.393f,  0.621f,  0.362f,
//	0.673f,  0.211f,  0.457f,
//	0.820f,  0.883f,  0.371f,
//	0.982f,  0.099f,  0.879f
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//	//1.0f,  0.0f,  0.0f,
//	//0.0f,  1.0f,  0.0f,
//	//0.0,  0.0f,  1.0f,
//	//1.0f,  1.0f,  0.0f,
//};
//
//// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
//static const GLfloat g_uv_buffer_data[] = {
//	0.000059f, 1.0f - 0.000004f,
//	0.000103f, 1.0f - 0.336048f,
//	0.335973f, 1.0f - 0.335903f,
//	1.000023f, 1.0f - 0.000013f,
//	0.667979f, 1.0f - 0.335851f,
//	0.999958f, 1.0f - 0.336064f,
//	0.667979f, 1.0f - 0.335851f,
//	0.336024f, 1.0f - 0.671877f,
//	0.667969f, 1.0f - 0.671889f,
//	1.000023f, 1.0f - 0.000013f,
//	0.668104f, 1.0f - 0.000013f,
//	0.667979f, 1.0f - 0.335851f,
//	0.000059f, 1.0f - 0.000004f,
//	0.335973f, 1.0f - 0.335903f,
//	0.336098f, 1.0f - 0.000071f,
//	0.667979f, 1.0f - 0.335851f,
//	0.335973f, 1.0f - 0.335903f,
//	0.336024f, 1.0f - 0.671877f,
//	1.000004f, 1.0f - 0.671847f,
//	0.999958f, 1.0f - 0.336064f,
//	0.667979f, 1.0f - 0.335851f,
//	0.668104f, 1.0f - 0.000013f,
//	0.335973f, 1.0f - 0.335903f,
//	0.667979f, 1.0f - 0.335851f,
//	0.335973f, 1.0f - 0.335903f,
//	0.668104f, 1.0f - 0.000013f,
//	0.336098f, 1.0f - 0.000071f,
//	0.000103f, 1.0f - 0.336048f,
//	0.000004f, 1.0f - 0.671870f,
//	0.336024f, 1.0f - 0.671877f,
//	0.000103f, 1.0f - 0.336048f,
//	0.336024f, 1.0f - 0.671877f,
//	0.335973f, 1.0f - 0.335903f,
//	0.667969f, 1.0f - 0.671889f,
//	1.000004f, 1.0f - 0.671847f,
//	0.667979f, 1.0f - 0.335851f
//};
//
//GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
//
//	// Create the shaders
//	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
//	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
//
//	// Read the Vertex Shader code from the file
//	std::string VertexShaderCode;
//	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
//	if (VertexShaderStream.is_open()) {
//		std::stringstream sstr;
//		sstr << VertexShaderStream.rdbuf();
//		VertexShaderCode = sstr.str();
//		VertexShaderStream.close();
//	}
//	else {
//		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
//		getchar();
//		return 0;
//	}
//
//	// Read the Fragment Shader code from the file
//	std::string FragmentShaderCode;
//	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
//	if (FragmentShaderStream.is_open()) {
//		std::stringstream sstr;
//		sstr << FragmentShaderStream.rdbuf();
//		FragmentShaderCode = sstr.str();
//		FragmentShaderStream.close();
//	}
//
//	GLint Result = GL_FALSE;
//	int InfoLogLength;
//
//	// Compile Vertex Shader
//	printf("Compiling shader : %s\n", vertex_file_path);
//	char const* VertexSourcePointer = VertexShaderCode.c_str();
//	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
//	glCompileShader(VertexShaderID);
//
//	// Check Vertex Shader
//	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
//	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//	if (InfoLogLength > 0) {
//		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
//		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
//		printf("%s\n", &VertexShaderErrorMessage[0]);
//	}
//
//	// Compile Fragment Shader
//	printf("Compiling shader : %s\n", fragment_file_path);
//	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
//	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
//	glCompileShader(FragmentShaderID);
//
//	// Check Fragment Shader
//	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
//	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//	if (InfoLogLength > 0) {
//		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
//		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
//		printf("%s\n", &FragmentShaderErrorMessage[0]);
//	}
//
//	// Link the program
//	printf("Linking program\n");
//	GLuint ProgramID = glCreateProgram();
//	glAttachShader(ProgramID, VertexShaderID);
//	glAttachShader(ProgramID, FragmentShaderID);
//	glLinkProgram(ProgramID);
//
//	// Check the program
//	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
//	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//	if (InfoLogLength > 0) {
//		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
//		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
//		printf("%s\n", &ProgramErrorMessage[0]);
//	}
//
//	glDetachShader(ProgramID, VertexShaderID);
//	glDetachShader(ProgramID, FragmentShaderID);
//
//	glDeleteShader(VertexShaderID);
//	glDeleteShader(FragmentShaderID);
//
//	return ProgramID;
//}
//
//unsigned int loadTexture(std::string url, GLenum format) {
//    // gen & bind IDs
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    // instellen texture params
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
//
//int main()
//{
//	if (!glfwInit())
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL!", nullptr,
//        nullptr);
//    glfwMakeContextCurrent(window);
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//    glViewport(0, 0, 800, 600);
//
//	unsigned int VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
//
//	// Or, for an ortho camera :
//	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
//
//	// Camera matrix
//	glm::mat4 View = glm::lookAt(
//		glm::vec3(4, 3, -3), // Camera is at (4,3,3), in World Space
//		glm::vec3(0, 0, 0), // and looks at the origin
//		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
//	);
//
//	// Model matrix : an identity matrix (model will be at the origin)
//	glm::mat4 Model = glm::mat4(1.0f);
//	// Our ModelViewProjection : multiplication of our 3 matrices
//	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
//
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);
//
//    // This will identify our vertex buffer
//    GLuint vertexbuffer;
//    // Generate 1 buffer, put the resulting identifier in vertexbuffer
//    glGenBuffers(1, &vertexbuffer);
//    // The following commands will talk about our 'vertexbuffer' buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    // Give our vertices to OpenGL.
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//	GLuint colorbuffer;
//	glGenBuffers(1, &colorbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
//
//	GLuint uvbuffer;
//	glGenBuffers(1, &uvbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
//
//	GLuint programID = LoadShaders("vertexShader.shader", "fragmentShader.shader");
//
//	// Get a handle for our "MVP" uniform
//	// Only during the initialisation
//	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
//
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	unsigned int diffuseTexID = loadTexture("container2.png", GL_RGBA);
//
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glUseProgram(programID);
//
//        // 1st attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(
//            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//            3,                  // size
//            GL_FLOAT,           // type
//            GL_FALSE,           // normalized?
//            0,                  // stride
//            (void*)0            // array buffer offset
//        );
//
//		// 2nd attribute buffer : colors
//		glEnableVertexAttribArray(1);
//		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//		glVertexAttribPointer(
//			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
//			3,                                // size
//			GL_FLOAT,                         // type
//			GL_FALSE,                         // normalized?
//			0,                                // stride
//			(void*)0                          // array buffer offset
//		);
//
//		glEnableVertexAttribArray(2);
//		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//		glVertexAttribPointer(
//			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
//			2,                                // size
//			GL_FLOAT,                         // type
//			GL_FALSE,                         // normalized?
//			0,                                // stride
//			(void*)0                          // array buffer offset
//		);
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, diffuseTexID);
//		glBindVertexArray(VAO);
//
//        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//        glDisableVertexAttribArray(0);
//
//		// Send our transformation to the currently bound shader, in the "MVP" uniform
//// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
//		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//}