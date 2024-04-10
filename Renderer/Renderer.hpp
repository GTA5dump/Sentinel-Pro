#pragma once
#include "../Common.hpp"
#include "../Utility/Joaat.hpp"

// Make our linker aware of the ImGui WndProcHandler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

typedef void(*PresentCallback)(void*);

class StateSaver
{
    // Public functions
public:
    StateSaver();
    ~StateSaver();

    bool saveCurrentState(ID3D11DeviceContext* pContext);
    bool restoreSavedState();
    void releaseSavedState();

    // Internal data
private:
    bool						m_savedState;
    D3D_FEATURE_LEVEL			m_featureLevel;
    ID3D11DeviceContext* m_pContext;
    D3D11_PRIMITIVE_TOPOLOGY	m_primitiveTopology;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11BlendState* m_pBlendState;
    FLOAT						m_blendFactor[4];
    UINT						m_sampleMask;
    ID3D11DepthStencilState* m_pDepthStencilState;
    UINT						m_stencilRef;
    ID3D11RasterizerState* m_pRasterizerState;
    ID3D11ShaderResourceView* m_pPSSRV;
    ID3D11SamplerState* m_pSamplerState;
    ID3D11VertexShader* m_pVS;
    ID3D11ClassInstance* m_pVSClassInstances[256];
    UINT						m_numVSClassInstances;
    ID3D11Buffer* m_pVSConstantBuffer;
    ID3D11GeometryShader* m_pGS;
    ID3D11ClassInstance* m_pGSClassInstances[256];
    UINT						m_numGSClassInstances;
    ID3D11Buffer* m_pGSConstantBuffer;
    ID3D11ShaderResourceView* m_pGSSRV;
    ID3D11PixelShader* m_pPS;
    ID3D11ClassInstance* m_pPSClassInstances[256];
    UINT						m_numPSClassInstances;
    ID3D11HullShader* m_pHS;
    ID3D11ClassInstance* m_pHSClassInstances[256];
    UINT						m_numHSClassInstances;
    ID3D11DomainShader* m_pDS;
    ID3D11ClassInstance* m_pDSClassInstances[256];
    UINT						m_numDSClassInstances;
    ID3D11Buffer* m_pVB;
    UINT						m_vertexStride;
    UINT						m_vertexOffset;
    ID3D11Buffer* m_pIndexBuffer;
    DXGI_FORMAT					m_indexFormat;
    UINT						m_indexOffset;

    StateSaver(const StateSaver&);
};

namespace Sentinel
{
    using DXCallback = std::function<void()>;
    using WindowProcedureCallback = std::function<void(HWND, UINT, WPARAM, LPARAM)>;

    class Renderer final
    {
    private:
        Renderer();

    public:
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        static void Destroy()
        {
            GetInstance().DestroyImpl();
        }
        static bool Initialize()
        {
            return GetInstance().InitImpl();
        }

        /**
         * @brief Add a callback function to draw using ImGui
         *
         * @param callback Callback function
         * @param priority Low values will be drawn before higher values.
         * @return true Successfully added callback.
         * @return false Duplicate render priority was given.
         */
        static bool AddDXCallback(DXCallback&& callback, std::uint32_t priority)
        {
            return GetInstance().AddDXCallbackImpl(std::move(callback), priority);
        }
        /**
         * @brief Add a callback function to handle Windows WindowProcedure
         *
         * @param callback Callback function
         * @param priority Low values will allow you handle the procedure earlier.
         * @return true Successfully added callback.
         * @return false Duplicate procedure priority was given.
         */
        static bool AddWindowProcedureCallback(WindowProcedureCallback&& callback, std::uint32_t priority)
        {
            return GetInstance().AddWindowProcedureCallbackImpl(std::move(callback), priority);
        }

        static void OnPresent()
        {
            GetInstance().OnPresentImpl();
        }
        static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        {
            return GetInstance().WndProcImpl(hwnd, msg, wparam, lparam);
        }

        static bool AddCallback(PresentCallback callback) 
        { 
            return GetInstance().AddCallbackImpl(callback); 
        }

        static bool RemoveCallback(PresentCallback callback) 
        { 
            return GetInstance().RemoveCallbackImpl(callback); 
        }

        static ImFont* Pricedown()
        {
            return GetInstance().m_Pricedown;
        }

        static ImFont* Icons()
        {
            return GetInstance().m_Icons;
        }

        static ImFont* RobotoMedium()
        {
            return GetInstance().m_RobotoMedium;
        }

        static ImFont* NotificationTitle()
        {
            return GetInstance().m_NotificationTitle;
        }

        static ImFont* NotificationContent()
        {
            return GetInstance().m_NotificationContent;
        }

        static void PreResize();
        static void PostResize();

    private:
        static void NewFrame();
        static void EndFrame();

    private:
        void DestroyImpl();
        bool InitImpl();

        bool AddDXCallbackImpl(DXCallback&& callback, std::uint32_t priority);
        bool AddWindowProcedureCallbackImpl(WindowProcedureCallback&& callback, std::uint32_t priority);

        void OnPresentImpl();
        LRESULT WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        bool AddCallbackImpl(PresentCallback callback) { return m_PresentCallbacks.insert(callback).second; }
        bool RemoveCallbackImpl(PresentCallback callback) { return m_PresentCallbacks.erase(callback) != 0; }

        static Renderer& GetInstance()
        {
            static Renderer i{};

            return i;
        }

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        IDXGISwapChain* m_SwapChain;

        ImFont* m_Pricedown;
        ImFont* m_Icons;
        ImFont* m_RobotoMedium;
        ImFont* m_NotificationTitle;
        ImFont* m_NotificationContent;

        std::map<JoaatType, DXCallback> m_DXCallbacks;
        std::set<PresentCallback> m_PresentCallbacks;
        std::map<JoaatType, WindowProcedureCallback> m_WindowProcedureCallbacks;

    };
}