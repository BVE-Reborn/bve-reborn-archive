#include <CLI/CLI.hpp>
#include <EASTL/hash_set.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <atomic>
#include <core/openbve/filesystem.hpp>
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
#include <cppfs/fs.h>
#include <fmt/ostream.h>
#include <foundational/allocation/bytes.hpp>
#include <foundational/allocation/linear_allocator.hpp>
#include <foundational/allocation/tagged_pool.hpp>
#include <foundational/logging/logger.hpp>
#include <foundational/statics/static_global.hpp>
#include <util/allocators.hpp>
#include <util/context.hpp>
#include <util/eastl_fmt.hpp>
#include <util/language.hpp>

using namespace foundational::allocation::operators;
namespace pmr = foundational::container::std_pmr;

using bve::util::Context;
using bve::util::LinearAllocator;
using foundational::allocation::TaggedPool;
using foundational::logging::LoggerBackend;
using foundational::logging::Message;
using foundational::logging::Severity;
using foundational::logging::Trace;
using foundational::statics::StaticGlobal;

class StandardOutBackend final : LoggerBackend {
  public:
	StandardOutBackend() : LoggerBackend(){};
	void write(Message const& msg, std::string const& formatted_content) override {
		if (!trace_enabled_.load(std::memory_order_acquire) && msg.severity == Severity::Trace) {
			return;
		}
		std::cout << formatted_content;
	}
	void flush() override {
		std::cout << std::flush;
	}
	void trace(bool const value) {
		trace_enabled_.store(value, std::memory_order_release);
	}
	~StandardOutBackend() override {
		flushLog();
	}

  private:
	std::atomic<bool> trace_enabled_{false};
};

FOUNDATIONAL_LOGGER_BACKEND(StandardOutBackend, stdout_backend);

FOUNDATIONAL_LOGGER(logger, "stress-test");

StaticGlobal<TaggedPool> alloc_pool("MemoryPool", 8_mb, 128);

eastl::string8 extant(Context const ctx, std::string const& path) {
	iSize i = path.size() - 1;
	for (; i > 0; --i) {
		if (path[i] == '.') {
			break;
		}
	}

	if (i == -1) {
		return eastl::string8(ctx.alloc);
	}
	eastl::string8 out(ctx.alloc);
	std::copy(path.begin() + i + 1, path.end(), std::back_inserter(out));
	return out;
}

cppfs::FileHandle find_folder(Context const ctx, cppfs::FilePath const& bve_folder, cppfs::FilePath const& offset) {
	cppfs::FileHandle handle = cppfs::fs::open(bve_folder.resolve(offset).fullPath());

	cppfs::FilePath path(handle.path());

	if (handle.exists() && handle.isDirectory()) {
		ctx.trace->log(Severity::Info, "{} directory found at {}", path.fileName(), path.fullPath());
	}
	else {
		ctx.trace->log(Severity::Warning, "{} directory not found at {}", path.fileName(), path.fullPath());
	}

	return handle;
}

eastl::vector<cppfs::FileHandle> recursive_enumerate(Context const ctx, cppfs::FileHandle& directory) {
	eastl::vector<cppfs::FileHandle> files(ctx.alloc);

	Trace enum_trace(*ctx.trace, fmt::format("{}-file-enum", directory.fileName()));

	enum_trace.log(Severity::Debug, "Finding files in {}", directory.path());

	directory.traverse(
	    [&](cppfs::FileHandle& file) {
		    files.emplace_back(file);
		    enum_trace.log(Severity::Trace, "Found file {}", file.path());
		    return true;
	    },
	    [&](auto&) { return true; });

	enum_trace.log(Severity::Debug, "Sorting {} files", files.size());

	std::sort(files.begin(), files.end(),
	          [](cppfs::FileHandle const& lhs, cppfs::FileHandle const& rhs) { return lhs.path() < rhs.path(); });

	enum_trace.log(Severity::Info, "Found {} files in {}", files.size(), directory.path());

	return files;
}

eastl::hash_set<eastl::string8> find_all_extants(Context const ctx, eastl::vector<cppfs::FileHandle> const& in) {
	eastl::hash_set<eastl::string8> out(ctx.alloc);

	Trace extant_finder(*ctx.trace, "extant-er");

	extant_finder.log(Severity::Debug, "Finding extants of {} files", in.size());

	for (auto const& f : in) {
		out.insert(extant(Context{&extant_finder, ctx.alloc}, f.fileName()));
	}

	extant_finder.log(Severity::Debug, "{} extants found", out.size());

	return out;
}

int main(int argc, char** argv) {
	foundational::statics::StaticGlobals::initialize();
	atexit(foundational::statics::StaticGlobals::deinitialize);

	Trace ctx_trace(*logger, "stress-test");
	LinearAllocator alloc(*alloc_pool, 1);

	Context ctx{&ctx_trace, &alloc};

	cppfs::FilePath cwd = bve::openbve::cwd();
	ctx.trace->log(Severity::Info, "CWD is {}", cwd.fullPath());

	CLI::App app("Stress test runner for bve-reborn.");

	cppfs::FilePath path = bve::openbve::data_directory();
	app.add_option("-i,--openbve-folder", path, "OpenBVE data directory to stress against. Must contain a LegacyContent.");
	bool trace = false;
	app.add_flag("--trace", trace, "Enable trace level messages");

	CLI11_PARSE(app, argc, argv)

	stdout_backend->trace(trace);

	cppfs::FileHandle bve_folder = cppfs::fs::open(bve::openbve::absolute(path).fullPath());
	if (!bve_folder.exists() || !bve_folder.isDirectory()) {
		ctx.trace->log(Severity::Error, "OpenBVE data directory \"{}\" not found.", bve_folder.path());
		ctx.trace->log(Severity::Error, "Aborting.");
		return 1;
	}

	cppfs::FileHandle object_folder = find_folder(ctx, bve_folder.path(), "LegacyContent/Railway/Object");
	cppfs::FileHandle route_folder = find_folder(ctx, bve_folder.path(), "LegacyContent/Railway/Route");
	cppfs::FileHandle sound_folder = find_folder(ctx, bve_folder.path(), "LegacyContent/Railway/Sound");
	cppfs::FileHandle train_folder = find_folder(ctx, bve_folder.path(), "LegacyContent/Train");

	auto object_files = recursive_enumerate(ctx, object_folder);
	auto route_files = recursive_enumerate(ctx, route_folder);
	auto sound_files = recursive_enumerate(ctx, sound_folder);
	auto train_files = recursive_enumerate(ctx, train_folder);

	eastl::hash_set<eastl::string8> object_extants = find_all_extants(ctx, object_files);
	eastl::hash_set<eastl::string8> route_extants = find_all_extants(ctx, route_files);
	eastl::hash_set<eastl::string8> sound_extants = find_all_extants(ctx, sound_files);
	eastl::hash_set<eastl::string8> train_extants = find_all_extants(ctx, train_files);

	ctx.trace->log(Severity::Info, "Extants found in Object:");
	for (auto& extant : object_extants) {
		ctx.trace->log(Severity::Info, "{}", extant);
	}
	ctx.trace->log(Severity::Info, "Extants found in Route:");
	for (auto& extant : route_extants) {
		ctx.trace->log(Severity::Info, "{}", extant);
	}
	ctx.trace->log(Severity::Info, "Extants found in Sound:");
	for (auto& extant : sound_extants) {
		ctx.trace->log(Severity::Info, "{}", extant);
	}
	ctx.trace->log(Severity::Info, "Extants found in Train:");
	for (auto& extant : train_extants) {
		ctx.trace->log(Severity::Info, "{}", extant);
	}

	ctx.trace->log(Severity::Debug, "Bytes allocated: {}", alloc_pool->get_bytes_allocated());
	ctx.trace->log(Severity::Debug, "Blocks allocated: {}", alloc_pool->get_blocks_allocated());
}
