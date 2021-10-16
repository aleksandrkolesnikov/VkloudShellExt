#include "ExtensionFactory.h"
#include "ContextMenuExtension.h"

extern long globalDllRefCounter;

namespace vkloud::shellext
{

ExtensionFactory::ExtensionFactory(const CLSID& id)
{
    if (!IsExtensionSupported(id))
    {
        throw "TODO: Create suitable exception";
    }

    classId = id;
    ::InterlockedIncrement(&globalDllRefCounter);
}

ExtensionFactory::~ExtensionFactory()
{
    ::InterlockedDecrement(&globalDllRefCounter);
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
        IUnknown* extension = CreateExtension();
        result = extension->QueryInterface(riid, ppv);
        extension->Release();
    }

    return result;
}

HRESULT ExtensionFactory::LockServer(BOOL lock)
{
    if (lock)
    {
        ::InterlockedIncrement(&globalDllRefCounter);
    }
    else
    {
        ::InterlockedDecrement(&globalDllRefCounter);
    }
}

IUnknown* ExtensionFactory::CreateExtension()
{
    if (classId == ContextMenuExtension::ClassId())
    {
        return reinterpret_cast<IUnknown*>(new ContextMenuExtension());
    }

    // TODO: Unreachable code
    return nullptr;
}

bool ExtensionFactory::IsExtensionSupported(const CLSID& id)
{
    return id == ContextMenuExtension::ClassId();
}

} // namespace vkloud::shellext
