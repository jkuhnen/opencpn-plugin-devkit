# Future reusable plugin template

This directory intentionally contains no plugin source template yet.

A future reference template is expected to provide:

- verified CMake configuration based on one currently working OpenCPN plugin build family;
- pinned `opencpn-libs` integration and explicit plugin API/minimum-host policy;
- target-aware `metadata.xml.in` generation and packaging;
- a separated adapter, domain/data/query, wxWidgets UI, overlay, and resource layout;
- a centralized semantic style layer with DAY/DUSK/NIGHT support;
- repository-specific `AGENTS.md` instructions;
- documented configure/build/test/package scripts for supported targets;
- a CI skeleton that builds and retains review artifacts without releasing ordinary PRs;
- a minimal placeholder plugin implementation sufficient to prove lifecycle, packaging, and load/unload behavior.

The template must be derived from proven, current OpenCPN plugin infrastructure and tested against its declared OpenCPN/API/wxWidgets/toolchain matrix. It must not invent a parallel build, packaging, metadata, provider, or API ecosystem. FE2, ShipDriver/Alternative Workflow, current in-tree plugins, `opencpn-libs`, the catalog schema, and the target API headers should be compared before selecting a base.

Before adding template files, open a focused issue that defines the chosen upstream commit(s), supported targets, ownership/lifetime model, exact build/package commands, validation matrix, licensing, and migration/update strategy. A compiling placeholder alone is not enough: the produced archive must install, load, disable, and unload in the intended OpenCPN runtimes.
