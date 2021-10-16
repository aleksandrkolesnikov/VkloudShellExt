#include "RegisterHelper.h"
#include <strsafe.h>

namespace vkloud::shellext::utils
{

HRESULT SetHKCRRegistryKeyAndValue(PCWSTR subKey, PCWSTR valueName, PCWSTR data)
{
    HRESULT result;
    HKEY hKey = nullptr;

    // Creates the specified registry key. If the key already exists, the
    // function opens it.
    result = HRESULT_FROM_WIN32(
        RegCreateKeyEx(HKEY_CLASSES_ROOT, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL));

    if (SUCCEEDED(result))
    {
        if (data != nullptr)
        {
            // Set the specified value of the key.
            DWORD cbData = lstrlen(data) * sizeof(*data);
            result = HRESULT_FROM_WIN32(
                RegSetValueEx(hKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(data), cbData));
        }

        RegCloseKey(hKey);
    }

    return result;
}

HRESULT GetHKCRRegistryKeyAndValue(PCWSTR subKey, PCWSTR valueName, PWSTR data, DWORD cbData)
{
    HRESULT result;
    HKEY hKey = nullptr;

    // Try to open the specified registry key.
    result = HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CLASSES_ROOT, subKey, 0, KEY_READ, &hKey));

    if (SUCCEEDED(result))
    {
        // Get the data for the specified value name.
        result =
            HRESULT_FROM_WIN32(RegQueryValueEx(hKey, valueName, NULL, NULL, reinterpret_cast<LPBYTE>(data), &cbData));

        RegCloseKey(hKey);
    }

    return result;
}

HRESULT RegisterInprocServer(PCWSTR module, const CLSID& id, PCWSTR friendlyName, PCWSTR threadModel)
{
    if (module == nullptr || threadModel == nullptr)
    {
        return E_INVALIDARG;
    }

    HRESULT result;

    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(id, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // Create the HKCR\CLSID\{<CLSID>} key.
    result = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(result))
    {
        result = SetHKCRRegistryKeyAndValue(szSubkey, NULL, friendlyName);

        // Create the HKCR\CLSID\{<CLSID>}\InprocServer32 key.
        if (SUCCEEDED(result))
        {
            result = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s\\InprocServer32", szCLSID);
            if (SUCCEEDED(result))
            {
                // Set the default value of the InprocServer32 key to the
                // path of the COM module.
                result = SetHKCRRegistryKeyAndValue(szSubkey, NULL, module);
                if (SUCCEEDED(result))
                {
                    // Set the threading model of the component.
                    result = SetHKCRRegistryKeyAndValue(szSubkey, L"ThreadingModel", threadModel);
                }
            }
        }
    }

    return result;
}

HRESULT UnregisterInprocServer(const CLSID& id)
{
    HRESULT result = S_OK;

    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(id, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // Delete the HKCR\CLSID\{<CLSID>} key.
    result = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(result))
    {
        result = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
    }

    return result;
}

HRESULT RegisterShellExtContextMenuHandler(PCWSTR fileType, const CLSID& id, PCWSTR friendlyName)
{
    if (fileType == nullptr)
    {
        return E_INVALIDARG;
    }

    HRESULT result;

    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(id, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // If pszFileType starts with '.', try to read the default value of the
    // HKCR\<File Type> key which contains the ProgID to which the file type
    // is linked.
    if (*fileType == L'.')
    {
        wchar_t szDefaultVal[260];
        result = GetHKCRRegistryKeyAndValue(fileType, NULL, szDefaultVal, sizeof(szDefaultVal));

        // If the key exists and its default value is not empty, use the
        // ProgID as the file type.
        if (SUCCEEDED(result) && szDefaultVal[0] != L'\0')
        {
            fileType = szDefaultVal;
        }
    }

    // Create the key HKCR\<File Type>\shellex\ContextMenuHandlers\{<CLSID>}
    result = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"%s\\shellex\\ContextMenuHandlers\\%s", fileType, szCLSID);
    if (SUCCEEDED(result))
    {
        // Set the default value of the key.
        result = SetHKCRRegistryKeyAndValue(szSubkey, NULL, friendlyName);
    }

    return result;
}

HRESULT UnregisterShellExtContextMenuHandler(PCWSTR fileType, const CLSID& id)
{
    if (fileType == nullptr)
    {
        return E_INVALIDARG;
    }

    HRESULT result;

    wchar_t szCLSID[MAX_PATH];
    StringFromGUID2(id, szCLSID, ARRAYSIZE(szCLSID));

    wchar_t szSubkey[MAX_PATH];

    // If pszFileType starts with '.', try to read the default value of the
    // HKCR\<File Type> key which contains the ProgID to which the file type
    // is linked.
    if (*fileType == L'.')
    {
        wchar_t szDefaultVal[260];
        result = GetHKCRRegistryKeyAndValue(fileType, NULL, szDefaultVal, sizeof(szDefaultVal));

        // If the key exists and its default value is not empty, use the
        // ProgID as the file type.
        if (SUCCEEDED(result) && szDefaultVal[0] != L'\0')
        {
            fileType = szDefaultVal;
        }
    }

    // Remove the HKCR\<File Type>\shellex\ContextMenuHandlers\{<CLSID>} key.
    result = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"%s\\shellex\\ContextMenuHandlers\\%s", fileType, szCLSID);
    if (SUCCEEDED(result))
    {
        result = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
    }

    return result;
}

} // namespace vkloud::shellext::utils
