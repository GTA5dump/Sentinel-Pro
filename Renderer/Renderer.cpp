#include "Renderer.hpp"
#include "../Pointers/Pointers.hpp"
#include "../Fonts/Pricedown.hpp"
#include "../Fonts/Icons.hpp"
#include "../Fonts/RobotoBold.hpp"
#include "../Fonts/RobotoMedium.hpp"

namespace Sentinel
{
    Renderer::Renderer()
    {

    }

    Renderer::~Renderer()
    {

    }

    void Renderer::DestroyImpl()
    {
        if (m_SwapChain && m_Device && m_DeviceContext)
        {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
    }

    bool Renderer::InitImpl()
    {
        if (!Pointers::pSwapChain)
        {
           LOG(FATAL) << "[Renderer] SwapChain pointer is invalid!";

            return false;
        }

        if (m_SwapChain = *Pointers::pSwapChain; !m_SwapChain)
        {
            LOG(FATAL) << "[Renderer] Dereferenced SwapChain pointer is invalid!";

            return false;
        }

        if (const auto result = m_SwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&m_Device)); result < 0)
        {
            LOG(FATAL) << std::format("[Renderer] Failed to get D3D Device with result [{}]", result);

            return false;
        }
        m_Device->GetImmediateContext(&m_DeviceContext);

        ImGui::CreateContext();
        
        auto& style = ImGui::GetStyle();
        style.WindowPadding = { 10.f, 10.f };
        style.PopupRounding = 0.f;
        style.FramePadding = { 8.f, 4.f };
        style.ItemSpacing = { 10.f, 8.f };
        style.ItemInnerSpacing = { 6.f, 6.f };
        style.TouchExtraPadding = { 0.f, 0.f };
        style.IndentSpacing = 21.f;
        style.ScrollbarSize = 13.f;
        style.GrabMinSize = 20.f;
        style.WindowBorderSize = 2.f;
        style.ChildBorderSize = 0.f;
        style.PopupBorderSize = 1.f;
        style.FrameBorderSize = 0.f;
        style.TabBorderSize = 0.f;
        style.WindowRounding = 3.f;
        style.ChildRounding = 0.f;
        style.FrameRounding = 4.f;
        style.ScrollbarRounding = 4.f;
        style.GrabRounding = 0.f;
        style.TabRounding = 0.f;
        style.WindowTitleAlign = { 0.5f, 0.5f };
        style.ButtonTextAlign = { 0.5f, 0.5f };
        style.DisplaySafeAreaPadding = { 3.f, 3.f };

        auto& colors = style.Colors;
        colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(ImColor(170, 170, 170, 255));
        colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(ImColor(30, 30, 30, 250));
        colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(ImColor(50, 30, 30, 0));
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(ImColor(99, 66, 245, 255));
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
        colors[ImGuiCol_TitleBg] = ImColor(38, 38, 38, 255);
        colors[ImGuiCol_TitleBgActive] = ImColor(38, 38, 38, 255);
        colors[ImGuiCol_TitleBgCollapsed] = ImColor(38, 38, 38, 255);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.0f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(ImColor(99, 66, 245, 255));
        colors[ImGuiCol_SliderGrab] = ImGui::ColorConvertU32ToFloat4(ImColor(99, 66, 245, 255));
        colors[ImGuiCol_SliderGrabActive] = ImGui::ColorConvertU32ToFloat4(ImColor(99, 66, 245, 255));
        colors[ImGuiCol_Button] = ImGui::ColorConvertU32ToFloat4(ImColor(60, 60, 60, 255));
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.78f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 0.87f);
        colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
        colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
        colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        ImFontConfig PricedownFontConfig{};
        PricedownFontConfig.FontDataOwnedByAtlas = false;
        std::strcpy(PricedownFontConfig.Name, "Pricedown");

        ImFontConfig IconsConfig{};
        IconsConfig.FontDataOwnedByAtlas = false;
        std::strcpy(IconsConfig.Name, "Icons");

        ImFontConfig RobotoFamilyConfig{};
        RobotoFamilyConfig.FontDataOwnedByAtlas = false;
        std::strcpy(RobotoFamilyConfig.Name, "Roboto-Family");

        m_Pricedown = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(g_PricedownFontBytes), sizeof(g_PricedownFontBytes), 24.f, &PricedownFontConfig);
        m_Icons = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(g_IconFontBytes), sizeof(g_IconFontBytes), 25.f, &IconsConfig);
        m_RobotoMedium = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(g_RobotoMediumFontBytes), sizeof(g_RobotoMediumFontBytes), 14.f, &RobotoFamilyConfig);
        m_NotificationTitle = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(g_RobotoBoldFontBytes), sizeof(g_RobotoBoldFontBytes), 18.f, &RobotoFamilyConfig);
        m_NotificationContent = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(g_RobotoMediumFontBytes), sizeof(g_RobotoMediumFontBytes), 18.f, &RobotoFamilyConfig);

        // Never returns false, useless to check return
        ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
        ImGui_ImplWin32_Init(Pointers::pGameWindow);

        LOG(INFO) << "[Renderer] Ready to use";
        return true;
    }

    bool Renderer::AddDXCallbackImpl(DXCallback&& callback, std::uint32_t priority)
    {
        return m_DXCallbacks.insert({ priority, callback }).second;
    }

    bool Renderer::AddWindowProcedureCallbackImpl(WindowProcedureCallback&& callback, std::uint32_t priority)
    {
        return m_WindowProcedureCallbacks.insert({ priority, callback }).second;
    }

    void Renderer::OnPresentImpl()
    {
        Renderer::NewFrame();
        for (const auto& callback : m_DXCallbacks | std::views::values)
            callback();
        Renderer::EndFrame();
    }

    LRESULT Renderer::WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        for (const auto& callback : m_WindowProcedureCallbacks | std::views::values)
            callback(hwnd, msg, wparam, lparam);

        return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    }

    void Renderer::PreResize()
    {
        ImGui_ImplDX11_InvalidateDeviceObjects();
    }

    void Renderer::PostResize()
    {
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    void Renderer::NewFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void Renderer::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

//====================================================================================================================================================================
//State Saver Class

// Construct
StateSaver::StateSaver() :
    m_savedState(false),
    m_featureLevel(D3D_FEATURE_LEVEL_11_0),
    m_pContext(NULL),
    m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED),
    m_pInputLayout(NULL),
    m_pBlendState(NULL),
    m_sampleMask(0xffffffff),
    m_pDepthStencilState(NULL),
    m_stencilRef(0),
    m_pRasterizerState(NULL),
    m_pPSSRV(NULL),
    m_pSamplerState(NULL),
    m_pVS(NULL),
    m_numVSClassInstances(0),
    m_pVSConstantBuffer(NULL),
    m_pGS(NULL),
    m_numGSClassInstances(0),
    m_pGSConstantBuffer(NULL),
    m_pGSSRV(NULL),
    m_pPS(NULL),
    m_numPSClassInstances(0),
    m_pHS(NULL),
    m_numHSClassInstances(0),
    m_pDS(NULL),
    m_numDSClassInstances(0),
    m_pVB(NULL),
    m_vertexStride(0),
    m_vertexOffset(0),
    m_pIndexBuffer(NULL),
    m_indexFormat(DXGI_FORMAT_UNKNOWN),
    m_indexOffset(0)
{
    for (int i = 0; i < 4; ++i)
    {
        m_blendFactor[i] = 0.0f;
    }
    for (int i = 0; i < 256; ++i)
    {
        m_pVSClassInstances[i] = NULL;
        m_pGSClassInstances[i] = NULL;
        m_pPSClassInstances[i] = NULL;
        m_pHSClassInstances[i] = NULL;
        m_pDSClassInstances[i] = NULL;
    }
}

// Destruct
StateSaver::~StateSaver()
{
    releaseSavedState();
}

// Save all states that are changed by the font-wrapper when drawing a string
bool StateSaver::saveCurrentState(ID3D11DeviceContext* pContext)
{
    if (m_savedState) releaseSavedState();
    if (pContext == NULL) return false;

    ID3D11Device* pDevice;
    pContext->GetDevice(&pDevice);
    if (pDevice != NULL)
    {
        m_featureLevel = pDevice->GetFeatureLevel();
        pDevice->Release();
    }

    pContext->AddRef();
    m_pContext = pContext;

    m_pContext->IAGetPrimitiveTopology(&m_primitiveTopology);
    m_pContext->IAGetInputLayout(&m_pInputLayout);

    m_pContext->OMGetBlendState(&m_pBlendState, m_blendFactor, &m_sampleMask);
    m_pContext->OMGetDepthStencilState(&m_pDepthStencilState, &m_stencilRef);

    m_pContext->RSGetState(&m_pRasterizerState);

    m_numVSClassInstances = 256;
    m_pContext->VSGetShader(&m_pVS, m_pVSClassInstances, &m_numVSClassInstances);
    m_pContext->VSGetConstantBuffers(0, 1, &m_pVSConstantBuffer);

    m_numPSClassInstances = 256;
    m_pContext->PSGetShader(&m_pPS, m_pPSClassInstances, &m_numPSClassInstances);
    m_pContext->PSGetShaderResources(0, 1, &m_pPSSRV);
    pContext->PSGetSamplers(0, 1, &m_pSamplerState);

    if (m_featureLevel >= D3D_FEATURE_LEVEL_10_0)
    {
        m_numGSClassInstances = 256;
        m_pContext->GSGetShader(&m_pGS, m_pGSClassInstances, &m_numGSClassInstances);
        m_pContext->GSGetConstantBuffers(0, 1, &m_pGSConstantBuffer);

        m_pContext->GSGetShaderResources(0, 1, &m_pGSSRV);

        if (m_featureLevel >= D3D_FEATURE_LEVEL_11_0)
        {
            m_numHSClassInstances = 256;
            m_pContext->HSGetShader(&m_pHS, m_pHSClassInstances, &m_numHSClassInstances);

            m_numDSClassInstances = 256;
            m_pContext->DSGetShader(&m_pDS, m_pDSClassInstances, &m_numDSClassInstances);
        }
    }

    m_pContext->IAGetVertexBuffers(0, 1, &m_pVB, &m_vertexStride, &m_vertexOffset);

    m_pContext->IAGetIndexBuffer(&m_pIndexBuffer, &m_indexFormat, &m_indexOffset);

    m_savedState = true;

    return true;
}


// Restore state
bool StateSaver::restoreSavedState()
{
    if (!m_savedState) return false;

    m_pContext->IASetPrimitiveTopology(m_primitiveTopology);
    m_pContext->IASetInputLayout(m_pInputLayout);

    m_pContext->OMSetBlendState(m_pBlendState, m_blendFactor, m_sampleMask);
    m_pContext->OMSetDepthStencilState(m_pDepthStencilState, m_stencilRef);

    m_pContext->RSSetState(m_pRasterizerState);

    m_pContext->VSSetShader(m_pVS, m_pVSClassInstances, m_numVSClassInstances);
    m_pContext->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);

    m_pContext->PSSetShader(m_pPS, m_pPSClassInstances, m_numPSClassInstances);
    m_pContext->PSSetShaderResources(0, 1, &m_pPSSRV);
    m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);

    if (m_featureLevel >= D3D_FEATURE_LEVEL_10_0)
    {
        m_pContext->GSSetShader(m_pGS, m_pGSClassInstances, m_numGSClassInstances);
        m_pContext->GSSetConstantBuffers(0, 1, &m_pGSConstantBuffer);

        m_pContext->GSSetShaderResources(0, 1, &m_pGSSRV);

        if (m_featureLevel >= D3D_FEATURE_LEVEL_11_0)
        {
            m_pContext->HSSetShader(m_pHS, m_pHSClassInstances, m_numHSClassInstances);

            m_pContext->DSSetShader(m_pDS, m_pDSClassInstances, m_numDSClassInstances);
        }
    }

    m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &m_vertexStride, &m_vertexOffset);

    m_pContext->IASetIndexBuffer(m_pIndexBuffer, m_indexFormat, m_indexOffset);

    return true;
}

/* General Misc */
template <typename T>
inline void SafeRelease(T*& p)
{
    if (nullptr != p)
    {
        p->Release();
        p = nullptr;
    }
}
// Release state
void StateSaver::releaseSavedState()
{
    m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    SafeRelease(m_pInputLayout);
    SafeRelease(m_pBlendState);
    for (int i = 0; i < 4; ++i)
        m_blendFactor[i] = 0.0f;
    m_sampleMask = 0xffffffff;
    SafeRelease(m_pDepthStencilState);
    m_stencilRef = 0;
    SafeRelease(m_pRasterizerState);
    SafeRelease(m_pPSSRV);
    SafeRelease(m_pSamplerState);
    SafeRelease(m_pVS);
    for (UINT i = 0; i < m_numVSClassInstances; ++i)
        SafeRelease(m_pVSClassInstances[i]);
    m_numVSClassInstances = 0;
    SafeRelease(m_pVSConstantBuffer);
    SafeRelease(m_pGS);
    for (UINT i = 0; i < m_numGSClassInstances; ++i)
        SafeRelease(m_pGSClassInstances[i]);
    m_numGSClassInstances = 0;
    SafeRelease(m_pGSConstantBuffer);
    SafeRelease(m_pGSSRV);
    SafeRelease(m_pPS);
    for (UINT i = 0; i < m_numPSClassInstances; ++i)
        SafeRelease(m_pPSClassInstances[i]);
    m_numPSClassInstances = 0;
    SafeRelease(m_pHS);
    for (UINT i = 0; i < m_numHSClassInstances; ++i)
        SafeRelease(m_pHSClassInstances[i]);
    m_numHSClassInstances = 0;
    SafeRelease(m_pDS);
    for (UINT i = 0; i < m_numDSClassInstances; ++i)
        SafeRelease(m_pDSClassInstances[i]);
    m_numDSClassInstances = 0;
    SafeRelease(m_pVB);
    m_vertexStride = 0;
    m_vertexOffset = 0;
    SafeRelease(m_pIndexBuffer);
    m_indexFormat = DXGI_FORMAT_UNKNOWN;
    m_indexOffset = 0;

    SafeRelease(m_pContext);
    m_featureLevel = D3D_FEATURE_LEVEL_11_0;

    m_savedState = false;
}