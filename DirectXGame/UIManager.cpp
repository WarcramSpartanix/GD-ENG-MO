#include "UIManager.h"
#include "GraphicsEngine.h"
#include "ProfilerScreen.h"
#include "MenuScreen.h"
#include "InspectorScreen.h"
#include "HierarchyScreen.h"
#include "RenderSystem.h"
#include "EditorPlayScreen.h"
#include "EditorUndoScreen.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    GraphicsEngine::getInstance()->getRenderSystem()->m_d3d_device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    GraphicsEngine::getInstance()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}

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
    sharedInstance->init();
}

void UIManager::destroy()
{
}

DebugConsoleScreen* UIManager::getDebugScreen()
{
    return debugScreen;
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

UIManager::UIManager(HWND windowHandle)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(windowHandle);
    ImGui_ImplDX11_Init(GraphicsEngine::getInstance()->getRenderSystem()->m_d3d_device, GraphicsEngine::getInstance()->getRenderSystem()->m_imm_context);

    UINames uiNames;

    debugScreen = new DebugConsoleScreen();
    this->uiTable[uiNames.DEBUG_WINDOW] = debugScreen;
    this->uiList.push_back(debugScreen);

    ProfilerScreen* profilerScreen = new ProfilerScreen();
    this->uiTable[uiNames.PROFILER_SCREEN] = profilerScreen;
    this->uiList.push_back(profilerScreen);
    
    MenuScreen* menuScreen = new MenuScreen();
    this->uiTable[uiNames.MENU_SCREEN] = menuScreen;
    this->uiList.push_back(menuScreen);

    InspectorScreen* inspectorScreen = new InspectorScreen();
    this->uiTable[uiNames.INSPECTOR_SCREEN] = inspectorScreen;
    this->uiList.push_back(inspectorScreen);

    HierarchyScreen* hierarchyScreen = new HierarchyScreen();
    this->uiTable[uiNames.HIERARCHY_SCREEN] = hierarchyScreen;
    this->uiList.push_back(hierarchyScreen);
    
    EditorPlayScreen* editorPlayScreen = new EditorPlayScreen();
    this->uiTable[uiNames.EDITOR_PLAY_SCREEN] = editorPlayScreen;
    this->uiList.push_back(editorPlayScreen);
    
    EditorUndoScreen* editorUndoScreen = new EditorUndoScreen();
    this->uiTable[uiNames.EDITOR_UNDO_SCREEN] = editorUndoScreen;
    this->uiList.push_back(editorUndoScreen);
}

UIManager::~UIManager()
{
}

void UIManager::init()
{
    bool ret = LoadTextureFromFile("Assets\\Textures\\download.png", &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
}

void UIManager::release()
{
}
