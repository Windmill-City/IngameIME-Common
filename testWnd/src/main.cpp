#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "Main.hpp"

#include "imgui.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

MainContext MainContext::Main = MainContext();

int main()
{
    setlocale(LC_ALL, ".UTF-8");
    MainContext::Main.setup();

    // Event Loop
    while (!glfwWindowShouldClose(MainContext::Main.Window))
    {
        // Background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        MainContext::Main.Render.newFrame();

        glfwSwapBuffers(MainContext::Main.Window);
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
    Window = glfwCreateWindow(800, 600, "IngameIME-Test", NULL, NULL);
    if (!Window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(Window);
    glfwSetKeyCallback(Window, key_callback);
    // Create Window End

    // GLAD Init Start
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSwapInterval(1);
    // GLAD Init End
    MainContext::Main.Render.setup();
    configFullscreen();
}

void MainContext::configFullscreen()
{
    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
    int                sw      = mode->width;
    int                sh      = mode->height;
    if (isFullscreen)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwSetWindowMonitor(Window, monitor, 0, 0, sw, sh, mode->refreshRate);
    }
    else
    {
        int w = 800, h = 600;
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwSetWindowMonitor(Window, NULL, (sw - w) / 2, (sh - h) / 2, w, h, GLFW_DONT_CARE);
    }
}

void MainContext::toggleFullscreen()
{
    isFullscreen = !isFullscreen;
    configFullscreen();
}