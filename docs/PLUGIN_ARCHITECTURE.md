# Preferred plugin architecture

Everything on this page is a **DevKit convention** unless an upstream source is cited. Adapt it to the selected, verified OpenCPN API rather than forcing OpenCPN into this shape.

## Design goals

- A thin OpenCPN adapter translates lifecycle calls, capabilities, host callbacks, IDs, and version differences.
- Domain, data, and query logic stays independent of wxWidgets when practical.
- UI construction and interaction live outside domain logic.
- One centralized style layer owns semantic tokens, DPI conversions, icons, and DAY/DUSK/NIGHT changes.
- Mouse-move, hover, hit-test, and render paths do bounded work over prepared data.
- Every resource has an explicit owner and shutdown point.
- Dependencies are few, justified, pinned, and compatible with the host ABI.
- Pure transformations and rules are testable without an OpenCPN runtime.

## Responsibility boundaries

The adapter owns OpenCPN-facing registration and translation, not business rules. A panel owns presentation state, not chart access. An overlay owns frame preparation and drawing, not provider parsing. A query service owns provider-independent requests, cancellation, and result normalization.

Do not parse proprietary chart formats when a documented OpenCPN or provider abstraction owns that responsibility. If multiple providers are possible, expose capabilities and provider-neutral data structures instead of scattering provider-name conditionals through the UI.

For UI placement:

- toolbar actions open/toggle the feature;
- sidebar/docked views show persistent operational context;
- floating panels support focused or movable workflows;
- canvas overlays show spatially anchored information;
- context-menu actions operate on a verified click/selection context.

## Suggested future plugin layout

```text
AGENTS.md
CMakeLists.txt
Plugin.cmake                  # only if the chosen proven build family uses it
cmake/
metadata/
  metadata.xml.in
src/
  adapter/                    # OpenCPN plugin class and API-version adapters
  domain/                     # pure types, rules, calculations
  data/                       # repositories, queries, provider adapters
  ui/                         # panels, dialogs, view models/controllers
  overlay/                    # frame preparation, hit testing, DC/GL renderers
  style/                      # semantic tokens, metrics, icons, scheme mapping
resources/
tests/
  unit/
  fixtures/                   # synthetic/public fixtures only
scripts/
docs/
```

Names are illustrative. A future template must derive exact CMake, metadata, entry-point, resource, localization, and packaging files from a currently working OpenCPN plugin infrastructure.

## Dependency direction

```text
OpenCPN/wx callbacks -> adapter -> application/domain <- data/query adapters
                                      |
                                      v
                               UI and overlay models
                                      |
                                      v
                              wx views / renderers
```

Domain code must not depend on OpenCPN globals or wx window classes. Provider adapters may depend on a documented provider contract, but the domain consumes a narrow interface. Views receive immutable snapshots or explicit view models rather than querying in paint handlers.

## Performance and state

- Pre-index hit-test candidates outside mouse-move callbacks.
- Debounce or coalesce hover queries and cancel stale work.
- Bound caches by entries or bytes and include data/scheme/DPI revisions in keys.
- Separate transient hover from persistent selection.
- Snapshot render data so a frame cannot observe half-applied updates.
- Measure before adding threads; a worker is not a substitute for bounded work.

## Ownership record

Each subsystem should document:

| Resource | Owner | Created | Released | Thread/context |
| --- | --- | --- | --- | --- |
| Host tool/menu IDs | adapter | successful registration | API-required removal/shutdown | host/UI |
| Top-level wx views | UI controller or wx parent, as verified | user/initialization action | close/deinit | UI |
| Worker and cancellation | service | first asynchronous request | deinit/service destruction | controlled worker |
| Render cache | overlay model | data preparation | invalidation/deinit | documented access |
| Host/API pointers | no ownership unless header says otherwise | callback/API call | never deleted by plugin unless specified | API-defined |

Replace the examples with exact rules from the target API and wx ownership model during implementation review.
