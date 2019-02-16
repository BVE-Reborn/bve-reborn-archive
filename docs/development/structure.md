# Project Structure {#dev-structure}

BVE-Reborn is composed of libraries that provide the common functionallity between all the different executables. This reduces code duplication, but it also allows us to provide a nice interface where other people can easily build upon the work that we have done. 

---

# Dependencies {#dev-structure-dependencies}

We use many different dependencies, which are all held in the dependencies folder as submodules.

## Submodules

Submodules are handled by `dependencies/targets.cmake` unless otherwise noted.

- [abseil](https://github.com/BVE-Reborn/abseil-cpp/tree/bve-tracking-branch) - Google library that adds C++17 features to all C++11 compilers. Used for `absl::optional`, `absl::string_view`, `absl::span` among others.
- [bin2c](https://github.com/cwfitzgerald/bin2c) - Used for automated conversion between a file and a c/c++ array. This is the only portable way of including binary files within an executable.
- cmake - A single third party cmake file which enables code coverage within builds.
- [cmake-santiziers](https://github.com/arsenm/sanitizers-cmake/tree/6947cff3a9c9305eb9c16135dd81da3feb4bf87f) - A cmake library which enables use of the gcc/clang sanitizers.
- [cmake-utils](https://github.com/cwfitzgerald/cmake-utils/tree/002fbf11a9448bf4c297ac34dd31e277b9f30c1f) - A cmake library which adds a variety of helping features to ease cmake use. Notable features: warnings/formatting/linting
- [cotire](https://github.com/sakra/cotire/tree/cotire-1.8.0) - A cmake library which adds compile time saving features like precompiled headers and unity builds.
- [doctest](https://github.com/onqtam/doctest/tree/1.2.9) - Testing library.
- [fmt](https://github.com/fmtlib/fmt/tree/6ebc1a967d9a54f8209b3c415039aff3979dd762) - String formatting library that uses python like syntax and is faster than streams.
- [gsl](https://github.com/Microsoft/GSL/tree/v1.0.0) - A library for assisting with following the c++ core guidelines. Mainly used for `gsl::span`, `gsl::string_span` for non-owning views into arrays, `gsl::narrow` and `gsl::narrow_cast` for safer narrowing conversions.
- [glm](https://github.com/g-truc/glm/tree/0.9.9-a2) - openGL Math library. Easies working with pixel and vertex data with many classes (such as `glm::vecN`) and geometric operations.
- [mapbox-variant](https://github.com/mapbox/variant/tree/v1.1.5) - Provides `mapbox::util::variant`, a type-safe union type. Used instead of boost::variant due to better performance and better features.
- [rapidxml](http://rapidxml.sourceforge.net/) - Fastest xml parser in the land with simple, powerful API. We're using a fork with xml namespace support.

Nova is handled by `dependencies/nova.cmake`

- [nova-renderer](https://github.com/NovaMods/nova-renderer) - Highly parallel cross platform rendering framework


---

# Libraries {#dev-structure-libraries}

The namespace for the library `libbve-X` will be `bve::X::`. So `libbve-util` will become `bve::util::`

- [libbve-log](\ref libbve-log) - Thread-safe logging library
- [libbve-parsers](\ref bve::parsers) - Contains easy to use parsers all formats dealt with in the program. Also includes helper functions for dealing with files and parsing.
- [libbve-util](\ref bve::core) - Central utility and support libraries. Commonly used math functions as well as other hard to place utility functions.
- libbve-load - Higher level loading library, loading whole routes/trains instead of individual files.


---

# Executables {#dev-structure-executables}

---

# Test Runner {#dev-structure-testrunner}

- tests - Main entry point to all the tests throughout the program.
