#include <iostream>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(400, 300, "Arcade Clicker", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool acState = true;
    float clicks_per_second = 15.0f;
    int userKey = 'G';
    bool waitingForKeyPress = false;
    char keyName[64];
    bool rcState = true;
    float clicks_per_second_right = 15.0f;
    int userKey_right = 'H';
    bool waitingForKeyPress_right = false;
    char keyName_right[64];

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(400, 300));

        ImGui::Begin("Arcade Clicker", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::SliderFloat("Left CPS", &clicks_per_second, 0.1f, 15.0f);
        ImGui::SliderFloat("Right CPS", &clicks_per_second_right, 0.1f, 15.0f);

        sprintf_s(keyName, "Left Keybind [%c]", userKey);
        sprintf_s(keyName_right, "Right Keybind [%c]", userKey_right);

        if (ImGui::Button(keyName)) {
            waitingForKeyPress = true;
        }

        if (ImGui::Button(keyName_right)) {
            waitingForKeyPress_right = true;
        }

        if (waitingForKeyPress) {
            for (int i = 1; i <= 255; i++) {
                if (GetAsyncKeyState(i) & 0x8000) {
                    userKey = i;
                    waitingForKeyPress = false;
                    break;
                }
            }
        }

        if (waitingForKeyPress_right) {
            for (int i = 1; i <= 255; i++) {
                if (GetAsyncKeyState(i) & 0x8000) {
                    userKey_right = i;
                    waitingForKeyPress_right = false;
                    break;
                }
            }
        }

        ImGui::End();

        if (!waitingForKeyPress && GetAsyncKeyState(userKey) & 0x8000) {
            acState = !acState;
            Sleep(200);
        }
        if (!waitingForKeyPress_right && GetAsyncKeyState(userKey_right) & 0x8000) {
            rcState = !rcState;
            Sleep(200);
        }

        int actual_delay = static_cast<int>(850.0f / clicks_per_second);
        int actual_delay_right = static_cast<int>(850.0f / clicks_per_second_right);

        if (acState && GetAsyncKeyState(VK_LBUTTON)) {
            HWND lwjglWindow = FindWindowA("LWJGL", nullptr);
            if (lwjglWindow == GetForegroundWindow()) {
                SendMessageW(lwjglWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                SendMessageW(lwjglWindow, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
                Sleep(actual_delay);
            }
        }

        if (rcState && GetAsyncKeyState(VK_RBUTTON)) {
            HWND lwjglWindow = FindWindowA("LWJGL", nullptr);
            if (lwjglWindow == GetForegroundWindow()) {
                SendMessageW(lwjglWindow, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
                SendMessageW(lwjglWindow, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
                Sleep(actual_delay_right);
            }
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}