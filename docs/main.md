# BVE-Reborn

Welcome to the documentation site of BVE-Reborn, the successor train simulator to OpenBVE. Most of this site will be about how to program against the libraries that make up the project, but there will also be a section about tips and tricks for making routes/plugins to the game.

# Playing

If you just want to play the game, you can download executables on the download site (coming soon). The game will come with a single sample route and train.

# Building

If you want to build the game yourself, either to develop against it, or to contribute, you can go to the [building](@ref building) page for instructions.

# Project Structure

![](project_structure.png)

There are three parts to the description of this project's structure. First is this list of all libraries and dependencies. Second is the graph showing the interdependencies between libraries. Third is each library's internal structure description, found as a readme file within the folder. This description includes libraries not yet implemented in order to show the feature plans of the project.

 - dependencies
   - [bin2c](https://github.com/cwfitzgerald/bin2c) - Used for automated conversion between a file and a c/c++ array. This is the only portable way of including binary files within an executable. 
   - binary
     - boost - used for `boost::optional`, `boost::filesystem`, and `boost::regex`. This is the only current binary dependency. 
   - [bsf framework](https://github.com/GameFoundry/bsf) - Used as the graphics engine and window management.
   - cmake - A single third party cmake file which enables code coverage within builds.
   - [cmake-santiziers](https://github.com/arsenm/sanitizers-cmake/tree/6947cff3a9c9305eb9c16135dd81da3feb4bf87f) - A cmake library which enables use of the the c++ sanitizers.
   - [cmake-utils](https://github.com/cwfitzgerald/cmake-utils/tree/002fbf11a9448bf4c297ac34dd31e277b9f30c1f) - A cmake library which adds a variety of helping features for ease of cmake use.
   - [cotire](https://github.com/sakra/cotire/tree/cotire-1.8.0) - A cmake library which adds compile time saving features like precompiled headers and unity builds.
   - [cpp_guildlines_support_library](https://github.com/Microsoft/GSL/tree/v1.0.0) - A c++ library for assisting the c++ core guildlines. Mainly used for `gsl::span`, `gsl::string_span`, and `gsl::narrow`.
   - [doctest](https://github.com/onqtam/doctest/tree/1.2.9) - Primary testing library.
   - [glm](https://github.com/g-truc/glm/tree/0.9.9-a2) - Used for opengl math helpers and other geometric operations.
   - [mapbox-variant](https://github.com/mapbox/variant/tree/v1.1.5) - Used instead of boost::variant due to better performance and better features.
   - [rapidxml](http://rapidxml.sourceforge.net/) - Fastest xml parser in the land.
 - libcore - Central support libraries for bvereborn. Math functions go in here, as well as other hard to place utility functions.
 - libload - Helps with loading of files which depend on other files.
 - liblog - Thread-safe logging library
 - libparsers - Parses all formats dealt with in the program, contains some helper features for file manipulation.
 - test_main - Main entry point to all the tests throughout the program.
[Global](global.html)

# TODO

 See [TODO.md](TODO.md)

\subpage Development
