#define _WIN32_WINNT 0x0A000001
#include <core/cache/rgmCache.h>
#include <Core/util/rgmUtil.h>
#include <windows.h>
#include <cwchar>

/// <summary>
/// Removes all files.
/// </summary>
/// <returns></returns>
auto RubyGM::Cache::RemoveAllFiles() noexcept ->Result {
    constexpr size_t len = MAX_PATH;  wchar_t path[len];
    auto str = L"\\*.*"; auto ll = std::wcslen(str);
    // 获取路径名
    if (auto buf = Cache::GetDirName(path, len - ll - 1)) {
        // 添加
        std::wcscpy(buf, str);
        buf[ll + 1] = 0;
#if 0
        // 文件操作
        IFileOperation* fileop = nullptr;
        auto hr = ::CoCreateInstance(
            CLSID_FileOperation,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IFileOperation,
            reinterpret_cast<void**>(&fileop)
        );
        // 创建成功
        if (SUCCEEDED(hr)) {
            // 删除文件
            fileop->De
        }
        RubyGM::SafeRelease(fileop);
        return Result(hr);
#else
        SHFILEOPSTRUCTW fileop{};
        fileop.hwnd = nullptr;
        fileop.wFunc = FO_DELETE;
        fileop.pFrom = path;
        fileop.pTo = nullptr;
        fileop.fFlags = FOF_NO_UI;
        fileop.fAnyOperationsAborted = FALSE;
        fileop.hNameMappings = nullptr;
        fileop.lpszProgressTitle = nullptr;
        ::SHFileOperationW(&fileop);
        return Result(S_OK);
#endif
    }
    return Result(S_FALSE);
}

// file op
#include <Platonly/luiPoFile.h>

// rubygm::impl 命名空间
namespace RubyGM { namespace impl {
    // get file name
    template<typename T>
    auto get_file_name(size_t id, T call) noexcept -> Result {
        constexpr size_t BUFLEN = MAX_PATH;
        wchar_t path[BUFLEN];
        // 获取路径
        if (auto buf = Cache::GetDirName(path, BUFLEN)) {
            // 先尝试创建目录
            ::CreateDirectoryW(path, nullptr);
            // 匹配对应文件名
            const size_t remain = BUFLEN - (buf - path) - 1;
            auto c = std::swprintf(buf, remain, L"f_%p.dat", reinterpret_cast<void*>(id));
            // 缓存有效
            if (c < int(remain) && c > 0) {
                return call(path);
            }
        }
        return Result(E_NOT_SUFFICIENT_BUFFER);
    }
}}

/// <summary>
/// Writes the data.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="data">The data.</param>
/// <param name="len">The length.</param>
/// <returns></returns>
auto RubyGM::Cache::WriteData(
    size_t id, void * data, size_t len) noexcept -> Result {
    return impl::get_file_name(id, [=](const wchar_t* path) noexcept {
        LongUI::CUIFile* tmp;
        auto flag = tmp->Flag_Write | tmp->Flag_CreateNew;
        LongUI::CUIFile file(path, flag);
        if (file.IsOk()) {
            auto l = file.Write(data, len);
            return Result((l == len) ? S_OK : E_FAIL);
        }
        else {
            return Result(E_NOT_SET);
        }
    });
}

/// <summary>
/// Reads the data.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="data">The data.</param>
/// <param name="len">The length.</param>
/// <returns></returns>
auto RubyGM::Cache::ReadData(
    size_t id, void* data, size_t len) noexcept -> Result {
    return impl::get_file_name(id, [=](const wchar_t* path) noexcept {
        LongUI::CUIFile file(path, LongUI::CUIFile::Flag_Read);
        if (file.IsOk()) {
            auto l = file.Read(data, len);
            return Result((l == len) ? S_OK : E_FAIL);
        }
        else {
            return Result(E_NOT_SET);
        }
    });
}