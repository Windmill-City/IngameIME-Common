#include <cstdlib>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_KEYSTATE_BASED_INPUT
#define NK_IMPLEMENTATION
#include <nuklear.h>
#define NK_GLFW_GL4_IMPLEMENTATION
#include <nuklear_glfw_gl4.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

struct nk_context* nk;

int main()
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
    int wnd_w = 800, wnd_h = 800;

    GLFWwindow* window = glfwCreateWindow(wnd_w, wnd_h, "testWnd", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Place the window at screen center
    {
        int x, y, sw, sh;
        glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &sw, &sh);
        glfwSetWindowPos(window, x + sw / 2 - wnd_w / 2, y + sh / 2 - wnd_h / 2);
    }
    //  Create Window End

    // GLAD Init Start
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // GLAD Init End

    // Nuklear Init Start
    nk = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS, 512 * 1024, 128 * 1024);

    // Load Font
    struct nk_font_atlas* atlas;
    nk_glfw3_font_stash_begin(&atlas);
    nk_glfw3_font_stash_end();
    // Nuklear Init End

    // Event Loop
    while (!glfwWindowShouldClose(window))
    {
        // Nuklear Window Start
        int main_w = 600, main_h = 600;

        nk_glfw3_new_frame();
        if (nk_begin(nk,
                     "main",
                     nk_rect(wnd_w / 2 - main_w / 2, wnd_h / 2 - main_h / 2, main_w, main_h),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE))
        {
        }
        nk_end(nk);
        // Nuklear Window End

        // Background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    nk_glfw3_shutdown();
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    nk_window_set_position(nk, "main", nk_vec2(width / 2 - 300, height / 2 - 300));
}
