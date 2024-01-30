
#ifndef MEGOPP_DYNAMIC_LIBRARY_H_INCLUDED
#define MEGOPP_DYNAMIC_LIBRARY_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/util/os/windows/windows_simplify.h>

#include "memepp/string_view.hpp"
#include "memepp/string.hpp"
#include <memepp/convert/std/string.hpp>
#include <megopp/util/scope_cleanup.h>
#include "megopp/util/os/win/error_message.h"

#include <memory>

//#include "utf8/unchecked.h"

#if !MEGO_OS__WINDOWS__AVAILABLE
#	include <dlfcn.h>
#else
#	include <VersionHelpers.h>
#endif

namespace megopp {
namespace os {
	
	class dynamic_library
	{
		constexpr dynamic_library(void * _handle) noexcept : handle_(_handle) {}

		dynamic_library() = delete;
		dynamic_library(const dynamic_library&) = delete;
		dynamic_library& operator= (const dynamic_library&) = delete;
		dynamic_library(dynamic_library&&) = delete;
		dynamic_library& operator= (dynamic_library&&) = delete;
		
	public:
		~dynamic_library()
		{
			if (handle_) {
#if MEGO_OS__WINDOWS__AVAILABLE
				::FreeLibrary((HMODULE)handle_);
#else
				::dlclose(handle_);
#endif
			}
		}

		inline static std::shared_ptr<dynamic_library> load
			(const memepp::string & _path, memepp::string & _errorString, bool _dependentLibraryMode = false);

		template<typename _Fn>
		inline _Fn* get_symbol(const char* _symbol);

		inline static const memepp::string & file_name_suffix()
		{
#if MEGO_OS__WINDOWS__AVAILABLE
			static memepp::string suffix = "dll";
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
			static memepp::string suffix = "dylib";
#elif defined(linux) || defined(__linux) || defined(__linux__)
			static memepp::string suffix = "so";
#endif
			return suffix;
		}

	private:
		void * handle_;
	};
    typedef std::shared_ptr<dynamic_library> dynamic_library_ptr;

	inline std::shared_ptr<dynamic_library> dynamic_library::load(
		const memepp::string & _path, memepp::string & _errorString, bool _dependentLibraryMode)
	{
		if (_path.empty()) {
			_errorString = "path empty";
			return NULL;
		}

		void * handle = NULL;

#if MEGO_OS__WINDOWS__AVAILABLE
		static_assert(sizeof(wchar_t) == 2, "wchar_t is not two bytes");

		std::wstring u16name;
		
        ww898::utf::conv<ww898::utf::utf8, ww898::utf::utfw>(
			_path.cbegin(), _path.cend(), std::back_inserter(u16name));
		
		if (_dependentLibraryMode && IsWindows8OrGreater())
		{	
			handle = ::LoadLibraryExW(u16name.c_str(), NULL,
				LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | 
				LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | 
				LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
				LOAD_LIBRARY_SEARCH_SYSTEM32 | 
				LOAD_LIBRARY_SEARCH_USER_DIRS);
		}
		else
			handle = ::LoadLibraryW(u16name.c_str());

		if (!handle) {
			DWORD errorCode = ::GetLastError();

			char buffer[1024] = { 0 };
			snprintf(buffer, sizeof(buffer) -1, "megopp::os::dynamic_library::load function error, "
				"path(%s), error(<%lu>%s)", _path.data(),
				errorCode, megopp::os::win::errcode_as_string(errorCode).data());
			_errorString = buffer;
			return NULL;
		}
#else
		handle = ::dlopen(_path.c_str(), _dependentLibraryMode ? RTLD_NOW | RTLD_GLOBAL : RTLD_NOW);
		if (!handle) {
			memepp::string dlErrorString;
			const char *zErrorString = ::dlerror();
			if ( zErrorString )
				dlErrorString = zErrorString;
			
			char buffer[1024] = { 0 };
			snprintf(buffer, sizeof(buffer) - 1, 
				"failed to load \"%s\" and error desc(%s)", _path.data(), dlErrorString.data());
			_errorString = buffer;
			return NULL;
		}
#endif

		auto cleanup = megopp::util::scope_cleanup__create([&] 
		{
#if MEGO_OS__WINDOWS__AVAILABLE
			::FreeLibrary((HMODULE)handle);
#else
			::dlclose(handle);
#endif
		});

		auto original = new dynamic_library(handle);
		if (!original)
			return nullptr;

		cleanup.cancel();
		return std::shared_ptr<dynamic_library>(original);
	}

	template<typename _Fn>
	inline _Fn * dynamic_library::get_symbol(const char * _symbol)
	{
		if (!handle_)
			return NULL;

#if MEGO_OS__WINDOWS__AVAILABLE
		return (_Fn *)::GetProcAddress((HMODULE)handle_, _symbol);
#else
		return (_Fn *)::dlsym(handle_, _symbol);
#endif
	}

};
};

#endif // !MEGOPP_DYNAMIC_LIBRARY_H_INCLUDED
