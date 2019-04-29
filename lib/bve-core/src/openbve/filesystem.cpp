#include "core/openbve/filesystem.hpp"
#include "core/openbve/logger.hpp"
#include "foundational/util/platform.hpp"
#include "util/language.hpp"
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
#include <cppfs/fs.h>

#if defined(FOUNDATIONAL_WINDOWS)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	pragma warning(push)
#	pragma warning(disable : 4103)
#	include <ShlObj.h>
#	pragma warning(pop)

#elif defined(FOUNDATIONAL_LINUX)
#	include <errno.h>
#	include <pwd.h>
#	include <stdlib.h>
#	include <unistd.h>
#endif

namespace bve::openbve {
	namespace detail {

#if defined(FOUNDATIONAL_WINDOWS)
		static cppfs::FilePath get_windows_file_path(GUID const folder) {
			PWSTR path_raw;
			SHGetKnownFolderPath(folder, KF_FLAG_DEFAULT, nullptr, &path_raw);

			std::wstring w_str(path_raw);
			int const length = WideCharToMultiByte(CP_UTF8, 0, w_str.c_str(), cast<i32>(w_str.size()), nullptr, 0, nullptr, nullptr);

			std::string str(length, '\0');

			WideCharToMultiByte(CP_UTF8, 0, w_str.c_str(), cast<i32>(w_str.size()), &str[0], cast<i32>(str.size()), nullptr, nullptr);

			CoTaskMemFree(path_raw);

			return cppfs::FilePath(str);
		}
#endif

		static cppfs::FilePath appdata_directory_impl() {
#if defined(FOUNDATIONAL_WINDOWS)
			logger->log(foundational::logging::Level::Debug, "Retrieving appdata directory from FOLDERID_RoamingAppData");

			auto path = get_windows_file_path(FOLDERID_RoamingAppData);
#elif defined(FOUNDATIONAL_OSX)
#	error "Todo(cwfitzgerald): Find appdata directory on mac"
#elif defined(FOUNDATIONAL_LINUX)
			const char* config;

			cppfs::FilePath path;
			if ((config = getenv("XDG_CONFIG_HOME"))) {
				logger->log(foundational::logging::Level::Debug, "Retrieving appdata directory from $XDG_CONFIG_HOME");
				path = cppfs::FilePath(config);
			}
			else {
				logger->log(foundational::logging::Level::Debug, "Retrieving appdata directory relative to home directory");
				path = home_directory().resolve(".config/");
			}
#endif

			logger->log(foundational::logging::Level::Info, "User appdata directory: {:s}", path.fullPath());

			return path;
		}

		static cppfs::FilePath home_directory_impl() {
#if defined(FOUNDATIONAL_WINDOWS)
			logger->log(foundational::logging::Level::Debug, "Retrieving home directory from FOLDERID_Profile");

			auto path = get_windows_file_path(FOLDERID_Profile);
#elif defined(FOUNDATIONAL_OSX)
#	error "Todo(cwfitzgerald): Find home directory on mac"
#elif defined(FOUNDATIONAL_LINUX)
			const char* homedir;

			cppfs::FilePath path;
			if ((homedir = getenv("HOME"))) {
				logger->log(foundational::logging::Level::Debug, "Retrieving home directory from $HOME");
				path = cppfs::FilePath(homedir);
			}
			else {
				logger->log(foundational::logging::Level::Debug, "Retrieving home directory from getpwuid");

				uSize buf_size = sysconf(_SC_GETPW_R_SIZE_MAX);
				if (buf_size == -1ULL) {
					buf_size = 16384;
				}

				char* buf = cast<char*>(malloc(buf_size));
				passwd pwd;
				passwd* result;
				getpwuid_r(getuid(), &pwd, buf, buf_size, &result);

				path = cppfs::FilePath(result->pw_dir);

				free(buf);
			}
#endif

			logger->log(foundational::logging::Level::Info, "User home directory: {:s}", path.fullPath());

			return path;
		}

		static cppfs::FilePath data_directory_impl() {
			logger->log(foundational::logging::Level::Debug, "Retrieving home directory relative to appdata");

			auto path = appdata_directory().resolve("openBVE/");

			auto handle = cppfs::fs::open(path.path());

			if (!handle.exists() || !handle.isDirectory()) {
				logger->log(foundational::logging::Level::Info, "OpenBVE data directory at {:s} not found or not a directory",
				            path.fullPath());
			}
			else {
				logger->log(foundational::logging::Level::Info, "OpenBVE data directory found at {:s}", path.fullPath());
			}

			return path;
		}
	} // namespace detail

	cppfs::FilePath appdata_directory() {
		static cppfs::FilePath result = detail::appdata_directory_impl();
		return result;
	}
	cppfs::FilePath home_directory() {
		static cppfs::FilePath result = detail::home_directory_impl();
		return result;
	}
	cppfs::FilePath data_directory() {
		static cppfs::FilePath result = detail::data_directory_impl();
		return result;
	}
} // namespace bve::openbve
