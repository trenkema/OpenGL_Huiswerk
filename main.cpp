//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//int main(void)
//{
//	GLFWwindow* window;
//
//	// Initialize the GLFW
//	if (!glfwInit())
//	{
//		return -1;
//	}
//
//	// Create a windowed mode and it's OpenGL Context
//	window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
//
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//	
//	// Make the window's Context current
//
//	// Loop until the user closes the window
//	while (!glfwWindowShouldClose(window))
//	{
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		// Render the OpenGL here
//
//		// Sweap front and back buffers
//		glfwSwapBuffers(window);
//
//		// Poll for and process events
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//}