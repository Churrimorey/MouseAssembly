/*
glwf 设置窗口 获取键盘鼠标事件

鼠标：  点击视角上下左右转动
键盘：  
        q e 视角上升下降
        w s 视角前后移动
        a d 视角左右移动
*/






#include "Window.h"
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


Window::Window(const int width, const int height, const std::string& name)
    :DeltaTime(0), LastTime(0), m_Width(width), m_Height(height)
{
    if (!glfwInit())
        std::cout << "GlfwInit Error" << std::endl;

    m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (!m_Window)
    {
        glfwTerminate();
        std::cout << "Window Error" << std::endl;
    }
    std::cout << "Hello World!" << std::endl;
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_Window, MouseCallback);
    glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
    glfwSetScrollCallback(m_Window, ScrollCallback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error" << std::endl;
    }

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(m_Window, true);
    ImGui::StyleColorsDark();



    //std::cout << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{

}

void Window::Update()
{
    DeltaTime = (float)glfwGetTime() - LastTime;
    LastTime = (float)glfwGetTime();
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projectMatrix = glm::perspective(glm::radians(fov), (float)m_Width / (float)m_Height, 0.1f, 100.0f);
}

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

double Window::GetTime()
{
    return glfwGetTime();
}

void Window::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);

    float cameraSpeed = static_cast<float>(2.5 * DeltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
}

glm::vec3 Window::Camera_Pos() {
    return cameraPos;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        firstMouse = true;
        return;
    }
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

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}
