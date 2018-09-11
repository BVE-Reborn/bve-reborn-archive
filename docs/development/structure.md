# Project Structure {#dev-structure}

BVE-Reborn is composed of libraries that provide the common functionallity between all the different executables. This reduces code duplication, but it also allows us to provide a nice interface where other people can easily build upon the work that we have done. 

---

# Dependencies {#dev-structure-dependencies}

We use many different dependencies, which are all held in the dependencies folder as submodules. There is currently a single binary dependency - boost - which can be built by the dependency building script. See more at \ref dev-building. 

## Binary

All binary dependencies are handled by `dependencies/binary.cmake`

- [boost::optional](https://www.boost.org/doc/libs/1_64_0/libs/optional/doc/html/index.html) - Represents the possibility of a value. Used for places where a value not being set is acceptable.
- [boost::filesystem](https://www.boost.org/doc/libs/1_64_0/libs/filesystem/doc/index.htm) - Provides for many helpful functions for interacting with the filesystem in a sane way. Almost identical to the C++17 library [std::filesystem](https://en.cppreference.com/w/cpp/filesystem).
- [boost::regex](https://en.cppreference.com/w/cpp/regex) - Almost identical to std::regex but with a consistent, faster implementation. Use the std::regex documentation. 

## Submodules

Submodules are handled by `dependencies/targets.cmake` unless otherwise noted.

- [bin2c](https://github.com/cwfitzgerald/bin2c) - Used for automated conversion between a file and a c/c++ array. This is the only portable way of including binary files within an executable.
- cmake - A single third party cmake file which enables code coverage within builds.
- [cmake-santiziers](https://github.com/arsenm/sanitizers-cmake/tree/6947cff3a9c9305eb9c16135dd81da3feb4bf87f) - A cmake library which enables use of the gcc/clang sanitizers.
- [cmake-utils](https://github.com/cwfitzgerald/cmake-utils/tree/002fbf11a9448bf4c297ac34dd31e277b9f30c1f) - A cmake library which adds a variety of helping features to ease cmake use. Notable features: warnings/formatting/linting
- [cotire](https://github.com/sakra/cotire/tree/cotire-1.8.0) - A cmake library which adds compile time saving features like precompiled headers and unity builds.
- [cpp_guildlines_support_library](https://github.com/Microsoft/GSL/tree/v1.0.0) - A library for assisting with following the c++ core guidelines. Mainly used for `gsl::span`, `gsl::string_span` for non-owning views into arrays, `gsl::narrow` and `gsl::narrow_cast` for safer narrowing conversions.
- [doctest](https://github.com/onqtam/doctest/tree/1.2.9) - Testing library.
- [glm](https://github.com/g-truc/glm/tree/0.9.9-a2) - openGL Math library. Easies working with pixel and vertex data with many classes (such as `glm::vecN`) and geometric operations.
- [mapbox-variant](https://github.com/mapbox/variant/tree/v1.1.5) - Provides `mapbox::util::variant`, a type-safe union type. Used instead of boost::variant due to better performance and better features.
- [rapidxml](http://rapidxml.sourceforge.net/) - Fastest xml parser in the land with simple, powerful API.

BSF is handled by `dependencies/bsf.cmake`

- [bs framework](https://github.com/GameFoundry/bsf) - Graphics and window management framework.


---

# Libraries {#dev-structure-libraries}

The actual binary names of each library will be `libbve-X`. So `libcore` will become `libbve-core`. The namespace for the library `libX` will be `X::`. So `libcore` will become `core::`

- [libcore](\ref bvereborn) - Central support libraries. Commonly used math functions as well as other hard to place utility functions.
- [liblog](\ref liblog/include/log.hpp) - Thread-safe logging library
- [libparsers](\ref ::parsers) - Contains easy to use parsers all formats dealt with in the program. Also includes helper functions for dealing with files and parsing.
- libload - Higher level loading library, loading whole routes/trains instead of individual files.


---

# Executables {#dev-structure-executables}

---

# Test Runner {#dev-structure-testrunner}

- test_main - Main entry point to all the tests throughout the program.