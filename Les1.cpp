//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//int main()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL!", nullptr,
//		nullptr);
//	glfwMakeContextCurrent(window);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		return -1;
//	}
//	glViewport(0, 0, 800, 600);
//	float vertices[] = {
//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
//	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
//	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
//	//-0.5f, -0.5f, 0.5f,
//	//0.5f, 0.5f, 0.5f,
//	//-0.5f, 0.5f, 0.5f,
//	};
//	int indices[] = {
//	0,1,2,
//	0,2,3
//	};
//	/// <summary>
//	/// SETUP TRIANGLES
//	/// </summary>
//	unsigned int VAO, VBO, EBO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
//		GL_STATIC_DRAW);
//	int stride = sizeof(float) * 6;
//	// position
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//	glEnableVertexAttribArray(0);
//	// color
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)
//		* 3));
//	glEnableVertexAttribArray(1);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	/// <summary>
//	/// SETUP SHADERS
//	/// </summary>
//	const char* vertexShader = "#version 330 core\n"
//		"layout ( location = 0 ) in vec3 vPos; \n"
//		"layout ( location = 1 ) in vec3 vCol; \n"
//		"out vec3 color;\n"
//		"void main() {\n"
//		"\tgl_Position = vec4(vPos.xyz, 1.0); \n"
//		"\tcolor = vCol;\n"
//		"}\n\0"
//		;
//	const char* fragmentShader = "#version 330 core\n"
//		"out vec4 FragColor;\n"
//		"in vec3 color;\n"
//		"void main() {\n"
//		"\tFragColor = vec4(color, 1.0); \n"
//		"}\n\0"
//		;
//	unsigned int vert, frag;
//	vert = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vert, 1, &vertexShader, nullptr);
//	glCompileShader(vert);
//	frag = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(frag, 1, &fragmentShader, nullptr);
//	glCompileShader(frag);
//	/* DEBUG
//	int success;
//	char infoLog[512];
//	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//	glGetShaderInfoLog(frag, 512, NULL, infoLog);
//	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
//	<< std::endl;
//	}
//	*/
//	unsigned int myProgram = glCreateProgram();
//	glAttachShader(myProgram, vert);
//	glAttachShader(myProgram, frag);
//	glLinkProgram(myProgram);
//	glDeleteShader(vert);
//	glDeleteShader(frag);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	while (!glfwWindowShouldClose(window)) {
//		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//		glUseProgram(myProgram);
//		// draw stuff
//		glBindVertexArray(VAO);
//		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
//		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	glfwTerminate();
//	return 0;
//}