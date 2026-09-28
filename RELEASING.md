# Release Guide for Bencode_Lib

This document details the standardized release process for `Bencode_Lib`.

## Prerequisites

Before cutting a release, ensure:
1. You have a clean working tree on `main` branch.
2. The latest continuous integration run is green across all platforms (Linux GCC/Clang, macOS, Windows).
3. Local test suites pass with sanitizers and strict warnings enabled.
4. Downstream consumer integration test passes.

## Step-by-Step Release Checklist

### 1. Verification
Run the complete local verification pipeline:
```bash
# Build with strict warnings, tests, examples, and benchmarks
cmake -B build_release -S . \
  -DCMAKE_BUILD_TYPE=Release \
  -DBENCODE_WARNINGS_AS_ERRORS=ON \
  -DBENCODE_BUILD_TESTS=ON \
  -DBENCODE_BUILD_EXAMPLES=ON \
  -DBENCODE_BUILD_BENCHMARKS=ON

cmake --build build_release -j$(nproc)
ctest --test-dir build_release --output-on-failure

# Verify Downstream Package Installation & Integration
cmake --build build_release --target Bencode_Lib_PackageCheck

# Verify Sanitizers (AddressSanitizer & UndefinedBehaviorSanitizer)
cmake -B build_san -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBENCODE_ENABLE_SANITIZERS=ON \
  -DBENCODE_BUILD_TESTS=ON \
  -DBENCODE_BUILD_FUZZERS=ON

cmake --build build_san -j$(nproc)
ctest --test-dir build_san --output-on-failure
```

### 2. Version Bump
Update the version string in:
1. `CMakeLists.txt`: `project("Bencode_Lib" VERSION X.Y.Z ...)`
2. `vcpkg.json`: `"version": "X.Y.Z"`
3. `conanfile.py`: `version = "X.Y.Z"`

### 3. Update Documentation
1. Update `CHANGELOG.md` with release date and categorized changes under `[X.Y.Z]`.
2. Ensure `docs/` and `README.md` reflect any new public APIs or configuration flags.

### 4. Commit and Tag
```bash
git add CMakeLists.txt vcpkg.json conanfile.py CHANGELOG.md
git commit -m "chore(release): prepare release vX.Y.Z"
git tag -a vX.Y.Z -m "Release vX.Y.Z"
git push origin main --tags
```

### 5. Packaging Distribution
Generate the release distribution archives using CPack:
```bash
cpack --config build_release/CPackConfig.cmake
```
This produces `.tar.gz` and `.zip` archives containing the pre-built library, headers, CMake configs, and pkg-config manifests ready for GitHub Releases or internal repository deployment.
