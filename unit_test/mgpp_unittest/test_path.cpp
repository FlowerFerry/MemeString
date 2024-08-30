
#include <catch2/catch.hpp>
#include <mego/util/get_module_path.h>

#include <string_view>

static std::string WideStringToUTF8(const std::wstring_view& wstr) {
    
    if (wstr.empty()) {
        return {};
    }

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);

    if (size_needed <= 0) {
        return {};
    }

    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), &str[0], size_needed, nullptr, nullptr);

    return str;
}

#if MG_OS__WIN_AVAIL
TEST_CASE("Get module path", "[get_module_path]") 
{
    wchar_t path_0_1[MAX_PATH * 10];
    wchar_t path_0_2[MAX_PATH * 10];

    auto len_0_1 = mgu_get_module_w_path(NULL, NULL, 0, NULL);
    auto len_0_2 = GetModuleFileNameW(NULL, path_0_2, sizeof(path_0_2) / sizeof(path_0_2[0]));
    REQUIRE(len_0_1 == len_0_2);
    
    int dir_pos_1_1;
    auto len_1_1 = mgu_get_module_w_path(NULL, path_0_1, sizeof(path_0_1) / sizeof(path_0_1[0]), &dir_pos_1_1);
    //auto len_1_2 = GetModuleFileNameW(NULL, path_0_2, sizeof(path_0_2) / sizeof(path_0_2[0]));
    REQUIRE(len_1_1 == len_0_2);
    REQUIRE(wcscmp(path_0_1, path_0_2) == 0);
    REQUIRE(std::wstring_view{ path_0_1 }.rfind(L'\\') == dir_pos_1_1);
    REQUIRE(std::wstring_view{ path_0_2 }.rfind(L'\\') == dir_pos_1_1);

    wchar_t path_1_1[1];
    auto len_2_1 = mgu_get_module_w_path(NULL, path_1_1, sizeof(path_1_1) / sizeof(path_1_1[0]), NULL);
    REQUIRE(len_2_1 == len_0_2);
    REQUIRE(path_1_1[0] == L'\0');

    
    auto str = WideStringToUTF8(std::wstring_view{ path_0_2 });
    auto len_3_1 = MegoUtilImpl_GetModulePath(NULL, NULL, 0, NULL);
    REQUIRE(len_3_1 == str.size());
    
    char path_2_1[MAX_PATH * 10];
    int dir_pos_2_1;
    auto len_4_1 = MegoUtilImpl_GetModulePath(NULL, path_2_1, sizeof(path_2_1) / sizeof(path_2_1[0]), &dir_pos_2_1);
    REQUIRE(len_4_1 == str.size());
    REQUIRE(strcmp(path_2_1, str.c_str()) == 0);
    
    char path_2_2[1];
    auto len_5_1 = MegoUtilImpl_GetModulePath(NULL, path_2_2, sizeof(path_2_2) / sizeof(path_2_2[0]), NULL);
    REQUIRE(len_5_1 == str.size());
    REQUIRE(path_2_2[0] == '\0');
    
}
#endif
