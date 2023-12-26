#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "Main.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

MainContext MainContext::Main = MainContext();

int main()
{
    setlocale(LC_ALL, ".UTF-8");
    // GLFW Init Start
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // GLFW Init End

    // Create Window Start
    GLFWwindow* window = glfwCreateWindow(800, 600, "IngameIME-Test", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    // Create Window End

    // GLAD Init Start
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
    // GLAD Init End

    MainContext::Main.Window = window;
    MainContext::Main.setup();
    MainContext::Main.Render.setup();
    MainContext::Main.centerWindow();

    // Event Loop
    while (!glfwWindowShouldClose(window))
    {
        // Background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        MainContext::Main.Render.newFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Esc to exit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    // F11 - Toggle fullscreen
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) MainContext::Main.toggleFullscreen();
    // F12 - Toggle Debugger
    if (key == GLFW_KEY_F12 && action == GLFW_PRESS)
        MainContext::Main.Render.showDebugger = !MainContext::Main.Render.showDebugger;
    // F7 - Toggle Demo
    if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
        MainContext::Main.Render.showDemo = !MainContext::Main.Render.showDemo;
}

void MainContext::setup()
{
}

void MainContext::centerWindow()
{
    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int sw = videoMode->width;
    int sh = videoMode->height;

    int w, h;
    glfwGetWindowSize(Window, &w, &h);

    glfwSetWindowPos(Window, (sw - w) / 2, (sh - h) / 2);
}

void MainContext::toggleFullscreen()
{
    isFullscreen = !isFullscreen;
    if (isFullscreen)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(Window, NULL, 0, 0, 800, 600, GLFW_DONT_CARE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        centerWindow();
    }

    if (InputCtx) InputCtx->setFullScreen(isFullscreen);
}