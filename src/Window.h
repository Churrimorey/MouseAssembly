
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Window
{
private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	float DeltaTime;
	float LastTime;
public:
	Window(const int width, const int height, const std::string& name);
	~Window();
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;
	void Update();			
	void ProcessInput();
	void SwapBuffers();
	void PollEvents();
	double GetTime();
	bool WindowShouldClose();

	glm::vec3 Camera_Pos(); // New
};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

