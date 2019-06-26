#define DOCTEST_CONFIG_IMPLEMENT
// ReSharper disable CppUnusedIncludeDirective
#include <doctest/doctest.h>
// ReSharper restore CppUnusedIncludeDirective

#include <foundational/logging/logger.hpp>

class StandardOutBackend final : foundational::logging::LoggerBackend {
  public:
	StandardOutBackend() : LoggerBackend(){};
	void write(foundational::logging::Message const&, std::string const& formatted_content) override {
		std::cout << formatted_content;
	}
	void flush() override {
		std::cout << std::flush;
	}
	~StandardOutBackend() override {
		flushLog();
	}
};

FOUNDATIONAL_LOGGER_BACKEND(StandardOutBackend, stdout_back);

int main(int argc, char** argv) {
	foundational::statics::StaticGlobals::initialize();
	atexit(foundational::statics::StaticGlobals::deinitialize);

	doctest::Context context(argc, argv);

	int const res = context.run(); // run

	if (context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
		return res;           // propagate the result of the tests

	return res; // the result from doctest is propagated here as well
}
