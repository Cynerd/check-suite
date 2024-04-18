# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.2] - 2024-04-18
# Changed
- `array` is now optional for `ARRAY_TEST*` and deduced from the test name if
  not specified


## [1.0.1] - 2023-01-06
### Fixed
- linking with libcheck\_suite using pkg-config that resulted in missing check
- release script to correctly extract project version from Meson file


## [1.0.0] - 2022-12-15
- Initial version of Check Suite
