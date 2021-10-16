#pragma once

#include "../Utils/Handle.h"
#include <Windows.h>
#include <string>
#include <vector>

namespace vkloud::shellext::control
{

using RawData = std::vector<std::byte>;

class NamedPipeClient final
{
public:
    explicit NamedPipeClient(std::wstring_view pipeName);

    RawData SendData(RawData&& data);

private:
    void Write(RawData&& data);
    RawData Read();

    static utils::Handle<HANDLE> TryOpenPipe(std::wstring_view pipeName);

private:
    utils::Handle<HANDLE> handle;
};

} // namespace vkloud::shellext::control
