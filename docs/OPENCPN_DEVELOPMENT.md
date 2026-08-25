# OpenCPN plugin development model

## How to read this document

This page combines two kinds of material:

- **Upstream fact:** behavior verified in the current sources linked in [`references/README.md`](../references/README.md).
- **DevKit convention:** a compatibility-oriented default for projects using this repository. It is not an OpenCPN requirement.

OpenCPN APIs evolve. As of 2026-08-25, OpenCPN `master` declares plugin API 1.21, while `opencpn-libs` contains multiple versioned API directories. A plugin must be designed against the header and libraries for its actual minimum host version, not automatically against `master`.

## Core and plugin boundary

**Upstream fact.** The plugin API is the contract between an independently built plugin and OpenCPN core. The current header declares base plugin classes, capability flags, host functions, viewport data, overlay callbacks, messaging interfaces, and optional chart-plugin interfaces. On MSVC, the matching import library is also part of the build boundary. The OpenCPN developer manual stresses API and ABI compatibility, including wxWidgets compatibility.

**DevKit convention.** Keep host integration in a small adapter layer. Do not reach into OpenCPN internal headers, globals, storage, or chart internals merely because they are visible in a local source checkout. If a required operation is absent from the supported plugin API, record the gap and investigate upstream rather than guessing a private route.

## API headers and versioning

- Select the API level from the target host compatibility policy.
- Compile with the corresponding `ocpn_plugin.h` and, where required, matching import library supplied by the chosen infrastructure.
- Return accurate plugin API and plugin version values through the applicable base-class interface.
- Re-check current headers and loader source whenever an API level, compiler, wxWidgets ABI, platform target, or minimum OpenCPN version changes.

The versioned `HostApi` interface family beginning at 1.21 and older global API families differ materially. Do not back-port declarations, capability flags, or ownership rules by assumption.

## Lifecycle and host callbacks

**Upstream fact.** The classic plugin class family exposes `Init()` and `DeInit()`. `Init()` returns a bitmask of requested/installed capabilities. Optional callbacks are coupled to those capabilities; the exact class and signature depend on the API level. The current header also includes later initialization and pre-shutdown capabilities.

**DevKit convention.** Use this lifecycle discipline:

1. Construct only cheap, host-independent state before initialization.
2. In `Init()`, acquire host integrations, create required UI resources, load validated settings, and return only capabilities actually supported.
3. Make callback code tolerate partial initialization and shutdown ordering.
4. In `DeInit()` or the version-appropriate shutdown hook, stop work, unbind events, remove registered UI objects where required, release host-related resources, then destroy owned state.
5. Make cleanup idempotent where practical.

Consult the selected header and loader implementation for exact registration, removal, and callback ordering. Do not infer ordering from another plugin.

## Toolbar, menu, and window integration

The current API includes toolbar insertion/removal and chart-context-menu integration. Which functions and SVG variants are available is API-version dependent.

Keep responsibilities distinct:

- **Toolbar:** entry point and concise state indication, not a dense status surface.
- **Context menu:** actions tied to the clicked chart/canvas context.
- **Sidebar or docked panel:** persistent, glanceable task state.
- **Floating dialog/panel:** focused workflows that need room or temporary attention.
- **Canvas overlay:** geographically or visually anchored information only.

Store returned IDs and use the matching removal/visibility calls documented by the target header. Never assume ownership of host-created controls or menus.

## Canvas, viewport, and rendering

**Upstream fact.** `PlugIn_ViewPort` carries the chart-view geometry needed by plugin rendering, and the API supplies coordinate conversion helpers. The classic API has DC and OpenGL overlay callbacks; current headers also contain multi-canvas/priority-aware variants. Exact availability and semantics depend on API level.

Rendering rules:

- Treat the callback viewport as the source of truth for that frame and canvas.
- Support both DC and OpenGL paths when the plugin declares both capabilities; visually compare them in OpenCPN.
- Keep rendering bounded: no network access, unbounded queries, file parsing, blocking locks, or large allocation churn in paint callbacks.
- Cache immutable or slowly changing geometry using an explicit invalidation key such as viewport, scheme, DPI, data revision, and canvas identity.
- Restore graphics state that the applicable callback contract requires the plugin to preserve.
- Request a refresh using the documented host function instead of forcing paint recursion.

OpenGL state, context ownership, render priority, and multi-canvas behavior are particularly version-sensitive. Verify them in the target header and implementation before coding.

## Configuration and preferences

The classic API exposes the OpenCPN configuration object; the current header explicitly says the returned pointer must not be deleted. Plugins can also declare preferences-related capabilities and callbacks.

**DevKit convention.** Put all persistence behind a settings component. Namespace keys by plugin, validate ranges and enum values on read, provide safe defaults, and write only values the plugin owns. UI objects should edit a working copy and commit through the settings component. Never store secrets or private chart data in source control or logs.

## Messaging and data exchange

OpenCPN supports classic string-based plugin messages, and newer APIs add listener/payload interfaces for navigation and plugin messages. Available message types and payload contracts are not interchangeable.

- Use a documented message ID and schema owned by the producer.
- Validate payloads before use and handle unknown fields or versions.
- Retain listener objects for the lifetime required by the selected API.
- Avoid assuming delivery thread, ordering, buffering, or ownership unless the current API/source explicitly guarantees it.
- Keep legacy and newer message paths isolated so compatibility decisions are reviewable.

## Chart providers and chart queries

The API contains `PlugInChartBase` for plugins whose explicit purpose is to implement a chart type. That does not make core chart internals a general query API.

**DevKit convention.** Ordinary feature plugins should consume documented OpenCPN or provider abstractions. They must not parse, decrypt, scrape, or reverse-engineer proprietary chart formats to reproduce behavior owned by a chart provider. Prefer provider-independent request/result interfaces; isolate any unavoidable provider adapter and document its contract. If no supported query exists, surface that limitation instead of fabricating one.

## Threading, lifetime, and ownership

- Assume wxWidgets UI objects are manipulated only on the UI thread unless the applicable API explicitly states otherwise.
- Perform slow pure work on controlled workers, but marshal results back using documented wx event mechanisms.
- Cancel and join workers before destroying the objects they reference.
- Use RAII and smart pointers for plugin-owned objects. For host-returned pointers, follow the header's ownership note exactly.
- Do not retain callback arguments, viewports, device contexts, OpenGL contexts, or host payload references beyond their documented lifetime.
- Use weak references, generation counters, or cancellation tokens when asynchronous results can outlive a view.

## DPI and platform awareness

Use wxWidgets layout, font metrics, DPI conversion helpers, and scalable assets rather than fixed pixels where the target wxWidgets version supports them. Test at the DPI scales, Windows SDK target, architecture, wxWidgets build, and OpenCPN build type that will actually ship. DAY/DUSK/NIGHT changes must invalidate cached colors and graphics.

## Compatibility-first checklist

Before implementation:

1. Identify minimum and test OpenCPN versions, plugin API, wxWidgets ABI, compiler, OS target, and architecture.
2. Inspect the exact upstream header/library and a working plugin with the same build family.
3. Record unavailable or version-gated behavior.

Before handoff:

1. Configure, build, and package with the intended toolchain.
2. Inspect metadata and tarball contents.
3. Load, enable, exercise, disable, and reload the plugin in each required OpenCPN runtime.
4. Compare DC/OpenGL, multiple canvases, DPI settings, and DAY/DUSK/NIGHT where relevant.
5. Report runtime combinations not tested.
