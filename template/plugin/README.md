# {{PLUGIN_NAME}}

This repository was created from the OpenCPN Plugin DevKit's minimal API 1.18 template. Its placeholder behavior is a checkable toolbar tool whose generated bitmap follows OpenCPN DAY/DUSK/NIGHT colors. It proves lifecycle, callback, styling, and teardown structure; it is not a feature-complete plugin or a separate plugin framework.

The template contains no experimental vector-query API, chart/provider parsing, OpenGL renderer, network/device integration, CI workflow, signing, or release publishing.

## Why API 1.18

Chart Inspector and Latitude Ruler both use API 1.18 successfully with the same pinned `opencpn-libs` commit. That common behavior is the compatibility evidence for this initial baseline. Current upstream OpenCPN has newer APIs; API 1.18 is not a universal recommendation.

An intentional upgrade must review the target OpenCPN versions, versioned `opencpn-libs/api-*` directory, plugin base class and callbacks, MSVC import library/wxWidgets ABI, metadata, package target, and runtime matrix together.

## Customize the finite placeholder set

Replace every token below across the copied template before configuring:

| Placeholder | Meaning | Example |
| --- | --- | --- |
| `{{PLUGIN_NAME}}` | user-facing name | `Example Overlay` |
| `{{PLUGIN_ID}}` | CMake target/package ID; valid identifier | `exampleoverlay_pi` |
| `{{PLUGIN_CLASS}}` | C++ class identifier | `ExampleOverlayPi` |
| `{{PLUGIN_VERSION}}` | four-part CMake project version | `0.1.0.0` |
| `{{AUTHOR}}` | author/maintainer shown in metadata | `Example Maintainer` |
| `{{SOURCE_URL}}` | canonical source/info URL | `https://github.com/example/exampleoverlay_pi` |
| `{{TARBALL_URL}}` | final immutable package URL | `https://example.invalid/exampleoverlay_pi-0.1.0.0.tar.gz` |
| `{{LICENSE_SPDX}}` | chosen SPDX license identifier | `GPL-2.0-or-later` |

Then verify that no double-brace placeholder token remains. The source filenames are intentionally generic and do not require renaming.

## Initialize pinned development dependencies

The embedded DevKit cannot contain a recursive `.devkit` gitlink, so `.gitmodules.template` is reference input only. In the newly copied plugin repository, run:

```powershell
git init
git submodule add -b main https://github.com/jkuhnen/opencpn-plugin-devkit.git .devkit
git -C .devkit checkout f37eec1e7327bcec54fab0cc8e0bbc8b270b8e1c
git submodule add -b main https://github.com/OpenCPN/opencpn-libs.git opencpn-libs
git -C opencpn-libs checkout af6fc2b616bf21066d81e1991983c42a73b2e2d8
git add .gitmodules .devkit opencpn-libs
git commit -m "build: pin DevKit and OpenCPN libraries"
```

Those commits match the two inspected reference plugins. The generated repository's gitlinks pin the dependencies; `branch = main` is only an update hint. Review and commit pointer changes explicitly—never update them silently during unrelated work. `.devkit` is guidance only and must not become a runtime/build/package dependency. `opencpn-libs` supplies the build-time API header and MSVC import library.

After the real `.gitmodules` and gitlinks exist, `.gitmodules.template` may be removed from the generated repository.

## Configure and build on Windows

Use the Visual Studio generator, architecture, wxWidgets build, and Windows SDK that match the intended OpenCPN host. Inspect a known-good local OpenCPN/plugin build instead of guessing values.

```powershell
cmake -S . -B build `
  -G "<Visual Studio generator matching OpenCPN>" `
  -A <platform matching OpenCPN> `
  -DwxWidgets_ROOT_DIR="<wxWidgets source/root>" `
  -DwxWidgets_LIB_DIR="<matching wxWidgets library directory>"

cmake --build build --config Release
```

The first proven template target is MSVC `msvc-wx32`; confirm that `OCPN_PACKAGE_TARGET`, `OCPN_TARGET_ARCH`, and `OCPN_TARGET_VERSION` printed by CMake match the produced binary and intended host. Override the cache values only with verified target information.

## Package and inspect

```powershell
cmake --build build --config Release --target package
Get-Content build/metadata.xml
tar -tf build/<generated-package>.tar.gz
```

The TGZ must contain `metadata.xml` at its root and the plugin binary under `plugins/`. Confirm the final filename, API version, `target`, `target-version`, `target-arch`, source URL, tarball URL, and extracted binary architecture. The template intentionally does not generate deprecated `build-target` or `build-gtk` metadata fields.

A successful compile or CPack run does not prove Plugin Manager compatibility. Validate generated metadata against the current `OpenCPN/plugins` XSD/tooling and verify the final uploaded artifact URL/checksum as required by the release workflow.

## Mandatory OpenCPN runtime validation

Before release, using each supported OpenCPN/runtime combination:

1. import/install the generated TGZ;
2. start OpenCPN and confirm the plugin loads without loader/ABI errors;
3. enable the plugin and confirm the toolbar tool is visible;
4. toggle it and confirm checked/unchecked visuals remain coherent;
5. switch DAY, DUSK, and NIGHT and inspect the bitmap in every scheme;
6. disable/unload the plugin and confirm the tool is removed cleanly;
7. restart OpenCPN and check load/disable behavior again;
8. uninstall the package and inspect the OpenCPN log for errors.

## Project-specific decisions

Do not blindly inherit the placeholder name/description, license, API baseline, minimum OpenCPN version, compiler/wxWidgets ABI, package target/architecture/Windows version, tarball URL, icon design, persistence behavior, dependencies, localization, tests, supported platforms, catalog channel, or release process. Add domain/data/query, wx UI, resources, or rendering modules only when real plugin behavior needs those boundaries.
