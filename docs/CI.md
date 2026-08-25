# Continuous integration direction

This is a **future DevKit CI policy**, not an implemented workflow. Issue #1 intentionally adds no CI configuration because no plugin template or verified build matrix exists yet.

## Pull-request checks

When a concrete plugin and supported matrix exist, CI should validate where practical:

- repository hygiene, Markdown/YAML/XML/CMake formatting, and targeted static analysis;
- CMake configuration from a clean checkout with recursive submodules;
- compilation for each supported OS/toolchain/API/wxWidgets/architecture combination;
- unit tests for pure domain, parsing, formatting, query, and style-token logic;
- creation of the plugin-manager artifact using the project's documented target;
- XML validation against a pinned/current `OpenCPN/plugins` schema;
- metadata consistency across source, binary, archive, and tag;
- tarball layout, required files, architecture, forbidden files, URL/checksum shape, and dependency sanity;
- retention of logs, generated metadata, checksums, test reports, and inspectable package artifacts.

## Matrix policy

The matrix must be derived from explicit support policy and current OpenCPN/plugin infrastructure. Do not invent targets to make a broad-looking matrix. Pin toolchain/container/action versions where feasible, record `opencpn-libs` and API revisions, and fail clearly when a supported job is skipped.

## Release boundary

Ordinary feature PRs must not automatically publish a public release, update a production catalog, sign artifacts, or upload to production distribution. PR jobs may build retained review artifacts. Promotion requires an explicit, protected release event with scoped credentials, immutable artifacts, checksum verification, and human approval.

Secrets must be least-privilege, unavailable to untrusted fork code, and never printed. Signing and production-upload jobs should be isolated from general compilation.

## Runtime boundary

CI compilation and package checks do not prove that OpenCPN can load or operate the plugin. Where realistic, separate integration jobs may start a controlled OpenCPN build and exercise installation/loading. Visual rendering, device/provider behavior, proprietary charts, GPU paths, DPI, and real DAY/DUSK/NIGHT evaluation often remain manual.

Every PR must state the runtime checks performed and those still required. A green CI result is not a compliance or type-approval claim.

## Adoption prerequisites

Before adding workflows, establish:

1. a proven plugin source/build template;
2. supported OpenCPN/API/wx/toolchain/target tuples;
3. deterministic configure, test, and package commands;
4. artifact and metadata specifications;
5. secret/release ownership and approval rules;
6. maintenance ownership for CI images and third-party actions.
