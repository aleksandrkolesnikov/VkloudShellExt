#pragma once

#include <Unknwn.h>
#include <concepts>

namespace vkloud::shellext::utils
{

template <typename T>
concept IUnknowable = std::derived_from<T, IUnknown>;

template <IUnknowable... T>
class ComClass : public T...
{
public:
    ComClass() noexcept
        : counter{1}
    {
    }

    virtual ~ComClass() = default;

    virtual ULONG AddRef() override
    {
        return ::InterlockedIncrement(&counter);
    }

    virtual ULONG Release() override
    {
        const auto current = ::InterlockedDecrement(&counter);
        if (counter == 0)
        {
            delete this;
        }

        return current;
    }

    template <IUnknowable U>
    U* As()
    {
        AddRef();
        return static_cast<U*>(this);
    }

private:
    ULONG counter;
};

} // namespace vkloud::shellext::utils
