# Building plugins on Windows

## Supported DevKit baseline

The primary DevKit environment is Windows 11 with an MSVC/Visual Studio C++ toolchain, CMake, the wxWidgets ABI required by the target OpenCPN build, and versioned plugin API libraries from [`OpenCPN/opencpn-libs`](https://github.com/OpenCPN/opencpn-libs).

This is a **local convention**, not a promise that one configuration works for every OpenCPN/plugin release. Current upstream OpenCPN and maintained plugin build systems have their own setup scripts and matrices. Match the intended host and chosen plugin infrastructure.

## Inspect before configuring

Before writing a new command, inspect a known-good local OpenCPN/plugin build:

```powershell
git submodule status --recursive
cmake -S <plugin-source> -B <existing-build> -N -L
cmake --build <existing-build> --target help --config RelWithDebInfo
```

Also inspect:

- `<existing-build>/CMakeCache.txt` for generator, platform, toolset, wxWidgets paths, SDK, API, and project options;
- the target OpenCPN build's CMake cache and runtime directory;
- the plugin repository's `README`, `INSTALL`, build scripts, `CMakePresets.json`, and CI configuration;
- the `opencpn-libs/api-*` directory selected by the project;
- a package known to load on the same host.

Do not copy a cache or hard-code a developer's absolute path. Extract the intentional settings and reproduce them with placeholders, presets, or documented variables.

## Three separate phases

### 1. Configure

Configuration chooses the generator, MSVC platform/toolset, dependencies, API level, wxWidgets build, and packaging rules. A generic shape is:

```powershell
cmake -S <plugin-source> -B <plugin-build> `
  -G "<verified Visual Studio generator>" `
  -A <verified platform> `
  <verified project options>
```

The generator, `-A` value, toolset, and project options are deliberately placeholders. Derive them from the target OpenCPN build and the selected working plugin family. Confirm that CMake selected the intended compiler, SDK, wxWidgets configuration, API header, and import library.

### 2. Build

Build one explicit configuration:

```powershell
cmake --build <plugin-build> --config RelWithDebInfo
```

Use `Debug`, `Release`, or `RelWithDebInfo` only when compatible with the host build and dependency ABI. Upstream's Windows guide notes that managed release plugins are not compatible with a debug OpenCPN build; for joint debugging, build host and plugin compatibly.

### 3. Package

Discover the targets exposed by the chosen infrastructure, then invoke the documented tarball/package target:

```powershell
cmake --build <plugin-build> --target help --config RelWithDebInfo
cmake --build <plugin-build> --target <verified-package-target> --config RelWithDebInfo
```

Some maintained workflows use CPack/TGZ plus post-processing; target names and steps are not universal. Packaging is complete only after the resulting archive and metadata pass [`PACKAGING.md`](PACKAGING.md).

## `opencpn-libs`

**Upstream fact.** `opencpn-libs` is commonly consumed as a Git submodule. Its libraries export CMake targets such as `ocpn::api`. The current `api-21` CMake file exposes the header and, for MSVC, links its `msvc-wx32/opencpn.lib` import library. Other API directories and older build families differ.

Initialize the exact pinned submodule commit:

```powershell
git submodule update --init --recursive
git submodule status --recursive
```

Do not silently advance the submodule during an unrelated feature.

## Common failure modes

### Architecture or target mismatch

The plugin DLL, OpenCPN executable, import library, wxWidgets libraries, and dependent DLLs must share the required architecture and ABI. Do not infer architecture from the machine OS. Verify CMake generator platform, binary headers, package metadata, and a runtime load.

### wxWidgets target naming

Current catalog metadata contains targets such as `msvc-wx32`; older records may use `msvc`. Treat names as compatibility identifiers defined by current OpenCPN/plugin infrastructure, not informal labels. Match the working build family and current catalog schema.

### Windows SDK target version

The selected Windows SDK version can appear in compiler settings and metadata `target-version`. A newer installed SDK does not automatically make an artifact compatible with every host. Compare the intended build matrix and known-good metadata before changing it.

### Line endings in shell scripts

CI/package helpers often run under POSIX shells even for a Windows-hosted checkout. Preserve LF for shell scripts and executable bits in Git. A script that configures under PowerShell may still fail in Git Bash or Linux CI because of CRLF.

### Metadata compatibility

A successful DLL link says nothing about catalog selection or installability. Verify `target`, `build-target` where used, `target-arch`, `target-version`, API version, archive layout, and checksum/URL using the current schema and host.

### Missing submodules

A shallow clone or ZIP download can omit `opencpn-libs` and nested dependencies. Check `git submodule status --recursive`; a leading `-` indicates an uninitialized submodule.

## Handoff evidence

Record the OpenCPN commit/version, API directory, `opencpn-libs` commit, wxWidgets version/ABI, Visual Studio toolset, Windows SDK, generator/platform, CMake configuration, package target, artifact names, and runtime test result. Explicitly mark anything not tested in OpenCPN.
