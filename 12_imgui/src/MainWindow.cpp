#include "MainWindow.h"

#if RIO_IS_CAFE
#include <controller/rio_ControllerMgr.h>
#include <controller/cafe/rio_CafeVPadDeviceCafe.h>
#include <controller/cafe/rio_CafeWPadDeviceCafe.h>
#endif // RIO_IS_CAFE
#include <gfx/rio_Graphics.h>
#include <gfx/rio_Window.h>

#include <rio.h>

#if RIO_IS_CAFE
#include <imgui_impl_gx2.h>
#include <imgui_impl_wiiu.h>
#else
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif // RIO_IS_CAFE

MainWindow::MainWindow()
    : ITask("MainWindow")
{
}

void MainWindow::prepare_()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); p_io = &io;
#if RIO_IS_CAFE
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#else
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
#endif // RIO_IS_CAFE
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

#if !RIO_IS_CAFE
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif

#if RIO_IS_CAFE
    // Scale everything by 1.5 for the Wii U
    ImGui::GetStyle().ScaleAllSizes(1.5f);
    io.FontGlobalScale = 1.5f;
#endif // RIO_IS_CAFE

    // Setup platform and renderer backends
#if RIO_IS_CAFE
    ImGui_ImplWiiU_Init();
    ImGui_ImplGX2_Init();
#else
    ImGui_ImplGlfw_InitForOpenGL(rio::Window::instance()->getNativeWindow().getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
#endif // RIO_IS_CAFE

    // Our state
    show_demo_window = true;
    show_another_window = false;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup display sizes and scales
    io.DisplaySize.x = (float)rio::Window::instance()->getWidth(); // set the current display width
    io.DisplaySize.y = (float)rio::Window::instance()->getHeight(); // set the current display height here

#if RIO_IS_WIN
    rio::Window::instance()->setOnResizeCallback(&MainWindow::onResizeCallback_);
#endif // RIO_IS_WIN
}

#if RIO_IS_WIN

void MainWindow::resize_(s32 width, s32 height)
{
    ImGuiIO& io = *p_io;

    // Setup display sizes and scales
    io.DisplaySize.x = (float)width; // set the current display width
    io.DisplaySize.y = (float)height; // set the current display height here
}

void MainWindow::onResizeCallback_(s32 width, s32 height)
{
    static_cast<MainWindow*>(rio::sRootTask)->resize_(width, height);
}

#endif // RIO_IS_WIN

void MainWindow::calc_()
{
    ImGuiIO& io = *p_io;

#if RIO_IS_CAFE
    ImGui_ImplWiiU_ControllerInput input;

    // VPAD
    {
        rio::CafeVPadDevice* device = static_cast<rio::CafeVPadDevice*>(rio::ControllerMgr::instance()->getControlDevice(rio::ControllerDefine::DEVICE_CAFE_V_PAD));
        if (device)
        {
            {
                const rio::CafeVPadDevice::VPadInfo& v_pad_info = device->getVPadInfo();
                if (v_pad_info.last_read_error == VPAD_READ_SUCCESS)
                    input.vpad = const_cast<VPADStatus*>(&(v_pad_info.status[0]));
            }
        }
    }
    // KPAD
    {
        rio::CafeWPadDevice* device = static_cast<rio::CafeWPadDevice*>(rio::ControllerMgr::instance()->getControlDevice(rio::ControllerDefine::DEVICE_CAFE_W_PAD));
        if (device)
        {
            for (int i = 0; i < 4; i++)
            {
                const rio::CafeWPadDevice::KPadInfo& k_pad_info = device->getKPadInfo(i);
                if (k_pad_info.last_read_error == KPAD_ERROR_OK)
                    input.kpad[i] = const_cast<KPADStatus*>(&(k_pad_info.status[0]));
            }
        }
    }

    ImGui_ImplWiiU_ProcessInput(&input);
#endif // RIO_IS_CAFE

    // Start the Dear ImGui frame
#if RIO_IS_CAFE
    ImGui_ImplGX2_NewFrame();
#else
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
#endif // RIO_IS_CAFE
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    rio::Graphics::setViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    rio::Window::instance()->clearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

    // Render draw data
#if RIO_IS_CAFE
    ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());
#else
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // RIO_IS_CAFE

#if RIO_IS_CAFE
    // Render keyboard overlay
    rio::Graphics::setViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 1.0f);
    rio::Graphics::setScissor(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGui_ImplWiiU_DrawKeyboardOverlay();
#else
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
#endif // RIO_IS_CAFE
}

void MainWindow::exit_()
{
    // Cleanup
#if RIO_IS_CAFE
    ImGui_ImplGX2_Shutdown();
    ImGui_ImplWiiU_Shutdown();
#else
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
#endif // RIO_IS_CAFE
    ImGui::DestroyContext();
}
