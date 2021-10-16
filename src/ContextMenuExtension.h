#pragma once

#include "Utils/ComClass.h"
#include <ShObjIdl_core.h>
#include <forward_list>
#include <rapidjson/rapidjson.h>
#include <string>

namespace vkloud::shellext
{

class ContextMenuExtension : public utils::ComClass<IShellExtInit, IContextMenu>
{
public:
    ContextMenuExtension();
    virtual ~ContextMenuExtension() = default;

    virtual HRESULT QueryInterface(REFIID riid, void** ppv) override;
    virtual HRESULT Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT dataObject, HKEY key) override;
    virtual HRESULT QueryContextMenu(HMENU menu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT flags) override;
    virtual HRESULT InvokeCommand(LPCMINVOKECOMMANDINFO info) override;
    virtual HRESULT GetCommandString(UINT_PTR isCommand, UINT flags, UINT* reserved, LPSTR name, UINT cchMax) override;

    static HRESULT Register(PCWSTR moduleName);
    static HRESULT Unregister();
    static const CLSID& ClassId();

private:
    static std::forward_list<std::wstring> GetSelectedFiles(LPDATAOBJECT dataObject);
    static std::wstring GetVkloudRootDirectory();

private:
    static constinit CLSID classId;

    std::forward_list<std::wstring> files;
};

} // namespace vkloud::shellext
