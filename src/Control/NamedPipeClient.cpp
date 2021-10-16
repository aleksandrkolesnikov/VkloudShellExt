#include "NamedPipeClient.h"
#include <system_error>

namespace vkloud::shellext::control
{

NamedPipeClient::NamedPipeClient(std::wstring_view pipeName)
    : handle{TryOpenPipe(pipeName)}
{
}

RawData NamedPipeClient::SendData(RawData&& data)
{
    Write(std::move(data));
    return Read();
}

void NamedPipeClient::Write(RawData&& data)
{
    DWORD bytesWritten = 0;

    if (!::WriteFile(handle.get(), data.data(), static_cast<DWORD>(data.size() * sizeof(std::byte)), &bytesWritten, nullptr))
    {
        throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
    }
}

RawData NamedPipeClient::Read()
{
    RawData data(1024 * 2);
    DWORD bytesRead = 0;

    if (!::ReadFile(handle.get(), data.data(), 1024 * sizeof(std::byte), &bytesRead, nullptr))
    {
        throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
    }

    return data;
}

utils::Handle<HANDLE> NamedPipeClient::TryOpenPipe(std::wstring_view pipeName)
{
    std::wstring fullName = L"\\\\.\\pipe\\";
    fullName += pipeName;
    utils::Handle<HANDLE> handle{
        ::CreateFile(fullName.data(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr),
        [](HANDLE h) { ::CloseHandle(h); }};

    if (handle.get() == INVALID_HANDLE_VALUE)
    {
        throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
    }

    return handle;
}

} // namespace vkloud::shellext::control
