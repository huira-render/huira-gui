#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"

// ====================== //
// === Console Output === //
// ====================== //
struct Console {
    std::vector<std::string> lines;
    bool scroll_to_bottom = false;

    void log(const char* fmt, ...) {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        lines.push_back(buf);
        scroll_to_bottom = true;
    }

    void draw() {
        for (const auto& line : lines) {
            ImGui::TextUnformatted(line.c_str());
        }
        if (scroll_to_bottom) {
            ImGui::SetScrollHereY(1.0f);
            scroll_to_bottom = false;
        }
    }

    void clear() {
        lines.clear();
    }
};

static Console g_console;

class StreamRedirector {
    std::streambuf* old_buf;
    std::ostringstream capture;
public:
    StreamRedirector() {
        old_buf = std::cout.rdbuf(capture.rdbuf());
    }

    ~StreamRedirector() {
        std::cout.rdbuf(old_buf);
    }

    // Call each frame to flush captured output to console
    void flush_to_console() {
        std::string output = capture.str();
        if (!output.empty()) {
            g_console.log("%s", output.c_str());
            capture.str("");
            capture.clear();
        }
    }
};

// ================ //
// === Menu Bar === //
// ================ //
void draw_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                // TODO: Implement file open
                g_console.log("File > Open clicked");
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                // TODO: Implement save
                g_console.log("File > Save clicked");
            }
            if (ImGui::MenuItem("Save As...")) {
                // TODO: Implement save as
                g_console.log("File > Save As clicked");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // TODO: Handle exit
                g_console.log("File > Exit clicked");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

// ====================== //
// === Viewport Panel === //
// ====================== //
void draw_central_panel() {
    if (ImGui::Begin("Viewport")) {
        if (ImGui::BeginTabBar("ViewportTabs")) {
            if (ImGui::BeginTabItem("Render")) {
                // TODO: 3D viewport for rendering
                ImGui::Text("Render viewport placeholder");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Solar Radiation")) {
                // TODO: Solar radiation pressure visualization
                ImGui::Text("Solar radiation viewport placeholder");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Bake")) {
                // TODO: Illumination/shadow baking viewport
                ImGui::Text("Bake viewport placeholder");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("LIDAR")) {
                // TODO: LIDAR simulation viewport
                ImGui::Text("LIDAR viewport placeholder");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

// ========================= //
// === Scene Graph Panel === //
// ========================= //
void draw_scene_graph_panel() {
    if (ImGui::Begin("Scene Graph")) {
        // TODO: Scene graph tree
        ImGui::Text("(scene tree placeholder)");
    }
    ImGui::End();
}

// ======================== //
// === Properties Panel === //
// ======================== //
void draw_properties_panel() {
    if (ImGui::Begin("Properties")) {
        // Vertical tabs using a child region for tab buttons
        static int selected_tab = 0;
        const char* tab_names[] = { "Options", "Camera", "Render" };
        const int tab_count = sizeof(tab_names) / sizeof(tab_names[0]);

        // Left side: vertical tab buttons
        ImGui::BeginChild("SideTabs", ImVec2(80, 0), true);
        for (int i = 0; i < tab_count; i++) {
            if (ImGui::Selectable(tab_names[i], selected_tab == i, 0, ImVec2(0, 30))) {
                selected_tab = i;
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right side: tab content
        ImGui::BeginChild("SideContent", ImVec2(0, 0), true);
        switch (selected_tab) {
        case 0:
            // TODO: General options
            ImGui::Text("Options");
            ImGui::Separator();
            ImGui::Text("(options placeholder)");
            break;
        case 1:
            // TODO: Camera settings
            ImGui::Text("Camera Settings");
            ImGui::Separator();
            ImGui::Text("(camera controls placeholder)");
            break;
        case 2:
            // TODO: Render settings
            ImGui::Text("Render Settings");
            ImGui::Separator();
            ImGui::Text("(render settings placeholder)");
            break;
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

// ===================== //
// === Console Panel === //
// ===================== //
void draw_console_panel() {
    if (ImGui::Begin("Console")) {
        // Clear button
        if (ImGui::Button("Clear")) {
            g_console.clear();
        }
        ImGui::Separator();

        // Scrolling region for log output
        ImGui::BeginChild("ConsoleScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        g_console.draw();
        ImGui::EndChild();
    }
    ImGui::End();
}

// ============ //
// === Main === //
// ============ //
int main() {
    // Allows capturing of std::cout streams
    StreamRedirector redirector;

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    // GL context settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Application", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // VSync

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.FontGlobalScale = 1.5f;  // 150% scale

    // Style
    ImGui::StyleColorsDark();

    // Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initial log message
    g_console.log("Application started");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        redirector.flush_to_console();

        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Dockspace over entire viewport
        ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // Set up default layout on first run
        static bool first_time = true;
        if (first_time) {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

            ImGuiID dock_main = dockspace_id;
            ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.25f, nullptr, &dock_main);
            ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down, 0.25f, nullptr, &dock_main);
            ImGuiID dock_right_bottom = ImGui::DockBuilderSplitNode(dock_right, ImGuiDir_Down, 0.5f, nullptr, &dock_right);

            ImGui::DockBuilderDockWindow("Viewport", dock_main);
            ImGui::DockBuilderDockWindow("Scene Graph", dock_right);
            ImGui::DockBuilderDockWindow("Properties", dock_right_bottom);
            ImGui::DockBuilderDockWindow("Console", dock_bottom);

            ImGui::DockBuilderFinish(dockspace_id);
        }

        // Draw UI
        draw_menu_bar();
        draw_central_panel();
        draw_scene_graph_panel();
        draw_properties_panel();
        draw_console_panel();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}