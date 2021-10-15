#include "ExtensionFactory.h"

namespace vkloud::shellext
{

ExtensionFactory::ExtensionFactory()
{
}

ExtensionFactory::~ExtensionFactory()
{
}

HRESULT ExtensionFactory::QueryInterface(REFIID riid, void** ppv)
{
    HRESULT result = S_OK;

    if (::IsEqualIID(IID_IUnknown, riid))
    {
        *ppv = this->As<IUnknown>();
    }
    else if (::IsEqualIID(IID_IClassFactory, riid))
    {
        *ppv = this->As<IClassFactory>();
    }
    else
    {
        result = E_NOINTERFACE;
        *ppv = nullptr;
    }

    return result;
}

HRESULT ExtensionFactory::CreateInstance(IUnknown* outer, REFIID riid, void** ppv)
{
    HRESULT result = CLASS_E_NOAGGREGATION;

    if (!outer)
    {
        *ppv = nullptr;
    }

    return result;
}

HRESULT ExtensionFactory::LockServer(BOOL lock)
{
    return S_OK;
}

} // namespace vkloud::shellext
