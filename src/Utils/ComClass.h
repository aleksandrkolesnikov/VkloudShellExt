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
        : counter_{1}
    {
    }

    virtual ~ComClass() = default;

    virtual ULONG AddRef() override
    {
        return ::InterlockedIncrement(&counter_);
    }

    virtual ULONG Release() override
    {
        const auto current = ::InterlockedDecrement(&counter_);
        if (counter_ == 0)
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
    ULONG counter_;
};

} // namespace vkloud::shellext::utils
