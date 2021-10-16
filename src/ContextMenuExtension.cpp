#include "ContextMenuExtension.h"
#include "Utils/RegisterHelper.h"
#include <memory>

constexpr auto IDM_DISPLAY = 0;

namespace vkloud::shellext
{

// {948A74A3-F2CD-4984-A387-8FE00DDB979A}
constinit CLSID ContextMenuExtension::classId = {
    0x948a74a3, 0xf2cd, 0x4984, {0xa3, 0x87, 0x8f, 0xe0, 0xd, 0xdb, 0x97, 0x9a}};

ContextMenuExtension::ContextMenuExtension()
{
}

HRESULT ContextMenuExtension::QueryInterface(REFIID riid, void** ppv)
{
    HRESULT result = S_OK;

    if (riid == IID_IUnknown || riid == IID_IContextMenu)
    {
        *ppv = this->As<IContextMenu>();
    }
    else if (riid == IID_IShellExtInit)
    {
        *ppv = this->As<IShellExtInit>();
    }
    else
    {
        result = E_NOINTERFACE;
        *ppv = nullptr;
    }

    return result;
}

HRESULT ContextMenuExtension::Initialize(LPCITEMIDLIST idlFolder, LPDATAOBJECT dataObject, HKEY key)
{
    if (!dataObject)
    {
        return E_INVALIDARG;
    }

    return E_FAIL;
}

HRESULT ContextMenuExtension::QueryContextMenu(HMENU menu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT flags)
{
    if (CMF_DEFAULTONLY & flags)
    {
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
    }

    static WCHAR text[] = L"Hello";

    MENUITEMINFO menuInfo = {sizeof(menuInfo)};
    menuInfo.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_STATE;
    menuInfo.wID = idCmdFirst + IDM_DISPLAY;
    menuInfo.fType = MFT_STRING;
    menuInfo.dwTypeData = text;
    menuInfo.fState = MFS_ENABLED;

    if (!::InsertMenuItem(menu, indexMenu, TRUE, &menuInfo))
    {
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    // Add a separator.
    MENUITEMINFO menuSeparator = {sizeof(menuSeparator)};
    menuSeparator.fMask = MIIM_TYPE;
    menuSeparator.fType = MFT_SEPARATOR;
    if (!::InsertMenuItem(menu, indexMenu + 1, TRUE, &menuSeparator))
    {
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_DISPLAY + 1));
}

HRESULT ContextMenuExtension::InvokeCommand(LPCMINVOKECOMMANDINFO info)
{
    if (0 != HIWORD(info->lpVerb))
    {
        return E_INVALIDARG;
    }

    switch (LOWORD(info->lpVerb))
    {
        case 0:
        {
            return S_OK;
        }

        default:
            return E_INVALIDARG;
    }

    return S_OK;
}

HRESULT ContextMenuExtension::GetCommandString(UINT_PTR isCommand, UINT flags, UINT* reserved, LPSTR name, UINT cchMax)
{
    return S_OK;
}

HRESULT ContextMenuExtension::Register(PCWSTR moduleName)
{
    auto result = utils::RegisterInprocServer(moduleName, classId, L"Vkloud.ContextMenuExtension Class", L"Apartment");
    if (SUCCEEDED(result))
    {
        result = utils::RegisterShellExtContextMenuHandler(L"*", classId, L"ContextMenuExtension");
    }

    return result;
}

HRESULT ContextMenuExtension::Unregister()
{
    auto result = utils::UnregisterInprocServer(classId);
    if (SUCCEEDED(result))
    {
        result = utils::UnregisterShellExtContextMenuHandler(L"*", classId);
    }

    return result;
}

std::forward_list<std::wstring> ContextMenuExtension::GetSelectedFiles(LPDATAOBJECT dataObject)
{
    std::forward_list<std::wstring> files;

    FORMATETC fe = {CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM stm = {TYMED_HGLOBAL};

    if (SUCCEEDED(dataObject->GetData(&fe, &stm)))
    {
        std::unique_ptr<STGMEDIUM, void (*)(STGMEDIUM*)> res{&stm, [](STGMEDIUM* s) { ::ReleaseStgMedium(s); }};
        HDROP hDrop = static_cast<HDROP>(::GlobalLock(stm.hGlobal));
        std::unique_ptr<std::remove_pointer_t<HGLOBAL>, void (*)(HGLOBAL)> global{&stm.hGlobal,
                                                                                  [](HGLOBAL g) { ::GlobalUnlock(g); }};
        if (hDrop != nullptr)
        {
            if (const auto filesCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0); filesCount > 0)
            {
                wchar_t szFileName[MAX_PATH];
                for (UINT i = 0; i < filesCount; ++i)
                {
                    if (::DragQueryFile(hDrop, i, szFileName, ARRAYSIZE(szFileName)))
                    {
                        files.emplace_front(szFileName);
                    }
                }
            }
        }
    }

    return files;
}

} // namespace vkloud::shellext
