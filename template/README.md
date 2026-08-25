# Reusable OpenCPN plugin template

`plugin/` is the first real DevKit plugin template. It is a deliberately small C++11/OpenCPN API 1.18 skeleton derived from two working plugins and cross-checked against current upstream infrastructure.

## Provenance snapshot

Inspected on 2026-08-25:

- [Chart Inspector `main` at `1a037c5`](https://github.com/jkuhnen/chartinspector_pi/tree/1a037c5b86d7d7c41e10cdd48eb8f6433c3e651f) for API-18/CMake integration, toolbar lifecycle, centralized scheme-aware style, packaging, and development-only `.devkit` guidance;
- [Latitude Ruler `main` at `71b5bd0`](https://github.com/jkuhnen/latituderuler_pi/tree/71b5bd02ca8cca611c61766ef2f310c8d19f7336) for the minimal API-18 adapter, deterministic toolbar teardown, compact build, and package structure;
- [`opencpn-libs` `main` at `af6fc2b`](https://github.com/OpenCPN/opencpn-libs/tree/af6fc2b616bf21066d81e1991983c42a73b2e2d8), including `api-18` and `WindowsHeaders`;
- [OpenCPN `master` at `d2b255f`](https://github.com/OpenCPN/OpenCPN/tree/d2b255f1ba745ad969d70f78e663580c14a89438) and the current plugin API declarations;
- [`OpenCPN/plugins` `master` at `6726fc3`](https://github.com/OpenCPN/plugins/tree/6726fc3c653b790183713bfb0856b6412d2a2592) for current metadata schema and packaging guidance;
- [ShipDriver `master` at `d6d6023`](https://github.com/Rasbats/shipdriver_pi/tree/d6d6023d32e94fd9ffe09c243d5afe5eaa811383) as a maintained alternative build-family cross-check.

Chart Inspector and Latitude Ruler share the selected CMake 3.15, C++11, API 1.18 through `opencpn-libs/api-18`, MSVC definitions, Windows headers, generated metadata, and TGZ packaging pattern. ShipDriver and current upstream sources were compatibility and infrastructure cross-checks; they do not all use this same baseline. The template keeps the behavior common to the two local proven references and removes feature-specific code and dependencies.

## Intentional scope

The placeholder registers one checkable toolbar tool, redraws its generated bitmap from a centralized OpenCPN DAY/DUSK/NIGHT palette, and removes the tool during `DeInit()`. This is enough to prove the skeleton without pretending to be an application.

The template does not contain Chart Inspector's experimental vector-query API, Latitude Ruler's overlay/rendering logic, provider-specific code, chart parsing, OpenGL, persistence, networking, device access, CI workflows, release automation, or production distribution configuration.

API 1.18 is the initial compatibility baseline because both reference plugins use it successfully and pin the same `opencpn-libs` commit. It is not the newest upstream API and is not universally preferred. A project may deliberately upgrade after checking its minimum OpenCPN version, new base class/header, import library, ABI, metadata, and runtime matrix.

## Maturity

This is a reviewed bootstrap template, not a certified or universally portable plugin framework. Its static/template structure and locally available Windows build path should be validated for every change. Every generated plugin still requires installation, load/enable/disable/unload, restart, and DAY/DUSK/NIGHT testing in the intended OpenCPN runtime before release.

See [`plugin/README.md`](plugin/README.md) for copying, placeholders, submodule bootstrap, build/package commands, artifact inspection, and project-specific decisions.
