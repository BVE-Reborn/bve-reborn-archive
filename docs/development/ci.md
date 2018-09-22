# Continuous Integration {#dev-ci}

We use the following for providing out continuous integration and coverage testing. If you want your commit to skip continuous integration (because it only fixes superficial issues), append `[ci skip]` to the commit message.

- [travis-ci](https://travis-ci.org/BVE-Reborn/bve-reborn) - Does mac/linux continuous integration testing, as well as documenation building and running the coverage analysis. The code is also run against the gcc/clang sanitizers.
- [ci.appveyor](https://ci.appveyor.com/project/cwfitzgerald/bve-reborn) - Builds and tests code on Windows.
- [converalls.io](https://coveralls.io/github/BVE-Reborn/bve-reborn) - Reports on coverage analysis and adds comments to pull requests.
