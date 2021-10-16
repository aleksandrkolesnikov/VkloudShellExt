#pragma once

#include <functional>
#include <memory>

namespace vkloud::shellext::utils
{

template <typename T>
using Handle = std::unique_ptr<std::remove_pointer_t<T>, std::function<void(T)>>;

}
