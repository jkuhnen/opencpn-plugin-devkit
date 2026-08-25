# Curated upstream references

This directory records links and short provenance notes. It must not contain copied standards, vendored manuals, private chart material, or large excerpts. Link to the authoritative source and summarize only what the DevKit needs.

## Source snapshot

Checked 2026-08-25. Branch links remain useful for updates; commit links record what was inspected for issue #1.

### OpenCPN core and developer documentation

- [`OpenCPN/OpenCPN` at `d2b255f`](https://github.com/OpenCPN/OpenCPN/tree/d2b255f1ba745ad969d70f78e663580c14a89438) — current core source inspected for plugin API declarations, loader/host implementation, viewport/render callbacks, color schemes, messaging, configuration, and chart-plugin boundaries.
- [`include/ocpn_plugin.h`](https://github.com/OpenCPN/OpenCPN/blob/d2b255f1ba745ad969d70f78e663580c14a89438/include/ocpn_plugin.h) — authoritative declarations and ownership/version notes for the API represented by that commit. It declares API 1.21; release/older headers may differ.
- [Current plugin API guidance](https://opencpn-manuals.github.io/main/opencpn-dev/plugin-api.html) — API/ABI design and evolution, including the versioned API interface introduced at API 1.21.
- [Current generated API documentation](https://opencpn-manuals.github.io/main/api-docs/index.html) — cross-reference to current headers/source; verify incomplete areas in source.
- [Compiling OpenCPN on Windows](https://opencpn-manuals.github.io/main/opencpn-dev/windows.html) — current upstream Windows/MSVC build and debug guidance. Plugin projects can use different build scripts.
- [Plugin API overview](https://opencpn-manuals.github.io/main/ocpn-dev-manual/0.1/pm-plugin-api-overview.html) — current plugin-oriented API overview; confirm examples against the chosen API header.
- [Manual source revisions](https://opencpn-manuals.github.io/main/manuals/sources.html) — identifies commits used to assemble the published manuals and helps detect stale pages.

### API libraries and working plugin infrastructure

- [`OpenCPN/opencpn-libs` at `af6fc2b`](https://github.com/OpenCPN/opencpn-libs/tree/af6fc2b616bf21066d81e1991983c42a73b2e2d8) — common plugin libraries and versioned API headers/import libraries, commonly consumed as a submodule.
- [`api-18/CMakeLists.txt`](https://github.com/OpenCPN/opencpn-libs/blob/af6fc2b616bf21066d81e1991983c42a73b2e2d8/api-18/CMakeLists.txt) — verifies API 1.18, its declared minimum OpenCPN version, the `ocpn::api` target, and the MSVC wx32 import library used by the first real template.
- [`api-21/CMakeLists.txt`](https://github.com/OpenCPN/opencpn-libs/blob/af6fc2b616bf21066d81e1991983c42a73b2e2d8/api-21/CMakeLists.txt) — verifies the `ocpn::api` target, API/minimum-host variables, include path, and current MSVC import library selection for API 1.21.
- [`jkuhnen/chartinspector_pi` at `1a037c5`](https://github.com/jkuhnen/chartinspector_pi/tree/1a037c5b86d7d7c41e10cdd48eb8f6433c3e651f) — working API-18 plugin inspected for CMake, lifecycle, centralized style, DAY/DUSK/NIGHT, metadata/TGZ packaging, and `.devkit` guidance. Its experimental vector-query code is explicitly excluded from the generic template.
- [`jkuhnen/latituderuler_pi` at `71b5bd0`](https://github.com/jkuhnen/latituderuler_pi/tree/71b5bd02ca8cca611c61766ef2f310c8d19f7336) — small working API-18 plugin inspected to distinguish the minimum adapter/build/package structure from Chart Inspector-specific complexity.
- [`jongough/testplugin_pi` at `dfde2b8`](https://github.com/jongough/testplugin_pi/tree/dfde2b83c94da31b27bc32ad9743d1aad41b8a6e) — current FE2 example inspected for multi-platform build/package structure. It is one community workflow, not the OpenCPN API specification.
- [`Rasbats/shipdriver_pi` at `d6d6023`](https://github.com/Rasbats/shipdriver_pi/tree/d6d6023d32e94fd9ffe09c243d5afe5eaa811383) — maintained alternative workflow example inspected for plugin setup, API library selection, dependencies, and distribution configuration.

The examples intentionally differ. Future templates must choose and validate a proven family rather than combining files by guesswork.

### Plugin installer, metadata, and catalog

- [`OpenCPN/plugins` at `6726fc3`](https://github.com/OpenCPN/plugins/tree/6726fc3c653b790183713bfb0856b6412d2a2592) — catalog project inspected for current metadata, schema, validation, and FE2/Cloudsmith flow.
- [`ocpn-plugin.xsd`](https://github.com/OpenCPN/plugins/blob/6726fc3c653b790183713bfb0856b6412d2a2592/ocpn-plugin.xsd) — authoritative schema for individual metadata files at the inspected commit.
- [`README.md`](https://github.com/OpenCPN/plugins/blob/6726fc3c653b790183713bfb0856b6412d2a2592/README.md) and [`TESTING.md`](https://github.com/OpenCPN/plugins/blob/6726fc3c653b790183713bfb0856b6412d2a2592/TESTING.md) — catalog contribution, schema/URL checks, tarball inspection, import, and private-catalog testing.
- [Plugin tarball documentation](https://opencpn-manuals.github.io/main/plugin-installer/Tarballs.html) — installer archive concepts and platform layouts. Always use the current page for new packaging work.
- [Plugin compatibility documentation](https://opencpn-manuals.github.io/development/ocpn-dev-manual/5.3.1/plugin-compat.html) — background on host/plugin target matching; verify legacy examples against current infrastructure.

### Maritime design references

These are design references, not source code or certification evidence:

- [IMO MSC.191(79)](https://wwwcdn.imo.org/localresources/en/KnowledgeCentre/IndexofIMOResolutions/MSCResolutions/MSC.191%2879%29.pdf) — performance standards for presentation of navigation-related information on shipborne displays.
- [IMO MSC.1/Circ.1609](https://wwwcdn.imo.org/localresources/en/OurWork/Safety/Documents/IMO%20Documents%20related%20to/MSC.1-Circ.1609.pdf) — guidelines for standardized user-interface design for navigation equipment.
- [IEC 62288:2021](https://webstore.iec.ch/en/publication/64659) — official catalog description and edition/status information; the full standard is copyrighted and is not reproduced here.
- [IHO standards and specifications](https://iho.int/en/standards-and-specifications) — official current status and editions for S-52 and the ECDIS Presentation Library.
- [IHO S-52 FAQs](https://iho.int/en/s-52-faq-s) — IHO-maintained overview of S-52 scope and maintenance.

Before using these references for a regulated or safety-critical product, confirm current editions, amendments, applicability, licensed standard text, and the actual conformity-assessment route with qualified experts.

## Adding references

For each new entry, record the owning organization, direct URL, revision/commit and access date where available, what claim it supports, and whether it is normative, explanatory, generated, community-maintained, or a local convention. Prefer stable official URLs and commit-pinned source links.
