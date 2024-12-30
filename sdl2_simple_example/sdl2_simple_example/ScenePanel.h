#pragma once
#include "imgui.h"
#include "MyWindow.h"
#include "HierarchyPanel.h"

class ScenePanel {
public:
    ScenePanel();
    ~ScenePanel();

    void Render();
    void Log(const char* message); 

    void SetFramebufferTexture(unsigned int texture);

    void SetWindow(MyWindow* window);
    void SetHierarchyPanel(HierarchyPanel* hierarchyPanel);

    bool IsFocused()  { return isPanelFocused; }
    bool IsHovered()  { return isPanelHovered; }

private:
    MyWindow* window = nullptr;        
    HierarchyPanel* hierarchy = nullptr; 
    unsigned int framebufferTexture;

    bool isPanelFocused = false;
    bool isPanelHovered = false;
};
