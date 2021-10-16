#pragma once

#include "Utils/ComClass.h"
#include <Unknwnbase.h>
#include <forward_list>

namespace vkloud::shellext
{

class ExtensionFactory : public utils::ComClass<IClassFactory>
{
public:
    explicit ExtensionFactory(const CLSID& id);
    virtual ~ExtensionFactory();

    virtual HRESULT QueryInterface(REFIID riid, void** ppv) override;
    virtual HRESULT CreateInstance(IUnknown* outer, REFIID riid, void** ppv) override;
    virtual HRESULT LockServer(BOOL lock) override;

private:
    IUnknown* CreateExtension();

    static bool IsExtensionSupported(const CLSID& id);

private:
    CLSID classId;
};

} // namespace vkloud::shellext
