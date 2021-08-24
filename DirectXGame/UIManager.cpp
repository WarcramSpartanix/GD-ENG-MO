#include "UIManager.h"
#include "GraphicsEngine.h"
#include "ProfilerScreen.h"
#include "MenuScreen.h"
#include "InspectorScreen.h"
#include "HierarchyScreen.h"
#include "ViewportScreen.h"

UIManager* UIManager::sharedInstance = nullptr;

UIManager* UIManager::getInstance()
{
    if (sharedInstance != nullptr)
        return sharedInstance;
    return nullptr;
}

void UIManager::initialize(HWND windowHandle)
{
    sharedInstance = new UIManager(windowHandle);
}

void UIManager::destroy()
{
}

void UIManager::drawAllUI()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    for (int i = 0; i < this->uiList.size(); i++)
    {
        this->uiList[i]->drawUI();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

ViewportScreen* UIManager::getViewportUI()
{
    UINames uinames;
    return (ViewportScreen*)this->uiTable[uinames.VIEWPORT_SCREEN];
}

UIManager::UIManager(HWND windowHandle)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(windowHandle);
    ImGui_ImplDX11_Init(GraphicsEngine::getInstance()->m_d3d_device, GraphicsEngine::getInstance()->m_imm_context);

    UINames uiNames;

    MenuScreen* menuScreen = new MenuScreen();
    this->uiTable[uiNames.MENU_SCREEN] = menuScreen;
    this->uiList.push_back(menuScreen);

    InspectorScreen* inspectorScreen = new InspectorScreen();
    this->uiTable[uiNames.INSPECTOR_SCREEN] = inspectorScreen;
    this->uiList.push_back(inspectorScreen);

    HierarchyScreen* hiScreen = new HierarchyScreen();
    this->uiTable[uiNames.HIERARCHY_SCREEN] = hiScreen;
    this->uiList.push_back(hiScreen);
    
    ViewportScreen* viewport = new ViewportScreen();
    this->uiTable[uiNames.VIEWPORT_SCREEN] = viewport;
    this->uiList.push_back(viewport);
}

UIManager::~UIManager()
{
}
