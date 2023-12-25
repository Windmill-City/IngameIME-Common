#include <cstdlib>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IMRenderer.hpp"

#define NK_IMPLEMENTATION
#include <nuklear.h>
#define NK_GLFW_GL4_IMPLEMENTATION
#include <nuklear_glfw_gl4.h>

#ifdef WIN32
  #define GLFW_EXPOSE_NATIVE_WIN32
  #include <GLFW/glfw3native.h>
#endif // WIN32

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

struct nk_context* nk;

/**
 * @brief Range of commonly used CJK charactors, if there still missing charactors,
 * refer to http://www.unicode.org/charts/ and fix the table
 */
// clang-format off

const nk_rune ranges[] = {
    0x0370, 0x03FF, // Greek
    0x0020, 0x007F, // Basic Latin(ASCII)
    0x3000, 0x303F, // CJK Symbols and Punctuation
    0xFF00, 0xFFEF, // Halfwidth and Fullwidth Forms
    0x4E00, 0x9FFF, // CJK Unified Ideographs (Han)
    0x1100, 0x11FF, // Hangul Jamo
    0xAC00, 0xD7AF, // Hangul Syllables
    0x3040, 0x309F, // Hiragana
    0x30A0, 0x30FF, // Katakana
    0
};

// clang-format on

int main()
{
    setlocale(LC_ALL, "");
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
    int wnd_w = 800, wnd_h = 800, wnd_x = 0, wnd_y = 0;

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
    struct nk_font_config config = nk_font_config(0);
    config.range                 = ranges;
    /* align every character to pixel boundary (if true set oversample (1,1)) */
    config.oversample_h          = 1;
    config.oversample_v          = 1;
    config.pixel_snap            = true;

    struct nk_font_atlas* atlas;
    nk_glfw3_font_stash_begin(&atlas);
    struct nk_font* font = nk_font_atlas_add_from_file(atlas, "assets/fonts/NotoSansCJKsc-VF.ttf", 20, &config);
    nk_glfw3_font_stash_end();
    nk_style_set_font(nk, &font->handle);
    // Nuklear Init End

    // IngameIME: Initialize
    auto im = IngameIME::Global::getInstance().getInputContext(glfwGetWin32Window(window));

    // TextEdit Init Start
    struct nk_text_edit       edit;
    IngameIME::PreEditContext preedit;
    nk_textedit_init_default(&edit);
    // IngameIME: Receive commit text and insert it into your textedit
    im->comp->IngameIME::CommitCallbackHolder::setCallback(
        [window](std::wstring commit)
        {
            nk_input_begin(nk);
            for (auto iter = commit.begin(); iter != commit.end(); iter++)
            {
                nk_rune u = *iter;
                nk_glfw3_char_callback(window, u);
            }
            nk_input_end(nk);
        });
    // IngameIME: Receive PreEdit text and draw it in your textedit
    im->comp->IngameIME::PreEditCallbackHolder::setCallback(
        [&preedit](IngameIME::CompositionState state, const IngameIME::PreEditContext* ctx)
        {
            switch (state)
            {
            case IngameIME::CompositionState::Begin:
            case IngameIME::CompositionState::End:
                preedit.content  = std::wstring();
                preedit.selStart = preedit.selEnd = -1;
                break;
            case IngameIME::CompositionState::Update:
                preedit = *ctx;
                break;
            }
        });
    // TextEdit Init End

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
            // Press ESC to exit the window
            if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);

            // Toggle Fullscreen
            nk_layout_row_dynamic(nk, 0, 1);
            if (nk_button_label(nk, "Toggle Fullscreen"))
            {
                if (glfwGetWindowMonitor(window))
                {
                    glfwSetWindowMonitor(window, NULL, wnd_x, wnd_y, wnd_w, wnd_h, 0);
                    im->setFullScreen(false);
                }
                else
                {
                    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
                    const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
                    glfwGetWindowPos(window, &wnd_x, &wnd_y);
                    glfwGetWindowSize(window, &wnd_w, &wnd_h);
                    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                    im->setFullScreen(true);
                }
            }

            // TextEdit
            nk_layout_row_dynamic(nk, 0, 1);
            nk_flags events = nk_edit_buffer(nk,
                                             NK_EDIT_FIELD | NK_EDIT_SIG_ENTER | NK_EDIT_GOTO_END_ON_ACTIVATE,
                                             &edit,
                                             nk_filter_default);
            if (events & NK_EDIT_ACTIVATED)
            {
                im->setActivated(true);
            }
            if (events & NK_EDIT_DEACTIVATED)
            {
                im->setActivated(false);
            }
            if (events & NK_EDIT_ACTIVE)
            {
                // IngameIME: Pass PreEdit Rect to position the CandidateList Window
                im->comp->setPreEditRect({(int)edit.preedit_pos.x, (int)edit.preedit_pos.y, 0, 20});
            }

            // Attribute
            nk_layout_row_dynamic(nk, 0, 1);
            nk_label(nk, "Attributes", NK_TEXT_CENTERED);

            nk_layout_row_dynamic(nk, 0, 2);
            nk_labelf(nk, NK_TEXT_LEFT, "PreEdit Pos: %.1f, %.1f", edit.preedit_pos.x, edit.preedit_pos.y);
            nk_labelf(nk, NK_TEXT_LEFT, "PreEdit Cursor: %d, %d", preedit.selStart, preedit.selEnd);
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
