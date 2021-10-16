#include "ContextMenuExtension.h"
#include "ExtensionFactory.h"
#include <Windows.h>

HMODULE thisModule = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            thisModule = hModule;
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    HRESULT result = CLASS_E_CLASSNOTAVAILABLE;

    try
    {
        auto factory = new vkloud::shellext::ExtensionFactory(rclsid);
        result = factory->QueryInterface(riid, ppv);
        factory->Release();
    }
    catch (...)
    {
    }

    return result;
}

STDAPI DllCanUnloadNow()
{
    return S_OK;
}

STDAPI DllRegisterServer()
{
    using namespace vkloud::shellext;

    wchar_t moduleName[MAX_PATH];
    if (!::GetModuleFileName(thisModule, moduleName, ARRAYSIZE(moduleName)))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return ContextMenuExtension::Register(moduleName);
}

STDAPI DllUnregisterServer()
{
    using namespace vkloud::shellext;

    return ContextMenuExtension::Unregister();
}
