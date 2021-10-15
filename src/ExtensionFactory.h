#pragma once

#include "Utils/ComClass.h"
#include <Unknwn.h>

namespace vkloud::shellext
{

class ExtensionFactory : public utils::ComClass<IClassFactory>
{
public:
    ExtensionFactory();
    virtual ~ExtensionFactory();

    virtual HRESULT QueryInterface(REFIID riid, void** ppv) override;
    virtual HRESULT CreateInstance(IUnknown* outer, REFIID riid, void** ppv) override;
    virtual HRESULT LockServer(BOOL lock) override;
};

} // namespace vkloud::shellext
