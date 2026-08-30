# Shared plugin design system

## Status

This page defines a **DevKit convention** for centralized styling. OpenCPN's current plugin header verifies that plugins receive color-scheme changes and can request named colors from the global color table. Exact names, availability, and values are upstream/version dependent; inspect the target source and handle lookup failure.

For vessel data presentation, pair this page with `MARITIME_HMI.md` and `DIGITAL_INSTRUMENTS.md`.

## Resolution order

1. Use OpenCPN global colors for base window, text, and other host-defined roles where the target API supplies a suitable color.
2. Map those base values into plugin semantic tokens.
3. Derive plugin interaction tokens per DAY/DUSK/NIGHT in one style module.
4. Use literal chart/navigation colors only for the encoded meaning they represent, bypassing decorative transformations.
5. Fall back to centrally declared, scheme-specific values only when a suitable host color is unavailable.

Never scatter RGB literals through widgets and renderers.

## Semantic token set

Every plugin should define, at minimum:

| Token | Purpose |
| --- | --- |
| `background` | primary plugin window/panel background |
| `surface` / `card` | grouped or elevated content surface |
| `border` | structural separation without excessive contrast |
| `text-primary` | main labels and values |
| `text-secondary` | supporting metadata |
| `interaction-accent` | ordinary actionable emphasis |
| `hover` | transient pointer feedback |
| `persistent-selection` | chosen item/object |
| `focus-halo` | keyboard/input focus independent of hover |
| `warning` | verified warning semantics only |
| `alarm` | verified alarm semantics only |
| `literal-chart-color` | pass-through encoded chart/navigation color |

For digital instruments, also define semantic roles for:

| Token | Purpose |
| --- | --- |
| `instrument-value` | primary numeric/textual operational value |
| `instrument-unit` | unit adjacent to the primary value |
| `instrument-label` | measurement identity such as `SOG`, `HDG`, `DEPTH` |
| `instrument-secondary` | supporting values and qualifiers |
| `instrument-scale` | inactive tape ticks / level track |
| `instrument-fill` | neutral valid level fill |
| `instrument-trend` | neutral valid trend line/area |
| `stale` | stale-data distinction |
| `unavailable` | no-data/unavailable distinction |
| `invalid` | invalid/out-of-range distinction when not an operational alarm |

Add tokens for disabled, success, separators, overlay halo, graphs and other roles only when their semantics are documented. Do not alias warning/alarm colors to generic interaction colors or ordinary instrument fill.

## DAY, DUSK, and NIGHT

Each token has an explicit value or derivation for all three schemes. On a scheme callback:

1. query applicable OpenCPN global colors again;
2. rebuild the immutable token set;
3. invalidate pens, brushes, bitmaps, text metrics, and render caches that embed colors;
4. refresh affected views using the documented host/wx mechanism.

Transformations should reduce large-area luminance and decorative contrast progressively while retaining readable text, focus, selection, warning/alarm distinction and primary instrument values. Secondary tape ticks, inactive level tracks and trend decoration should lose emphasis before primary values do. Validate contrast visually in OpenCPN; do not claim certification from generic contrast ratios alone.

## Interaction family

Chart Inspector's cool blue/cyan family is the current reference for hover, focus, and ordinary selection because it avoids casual use of common maritime safety colors. It is an example, not an immutable RGB mandate. Exact values belong in the centralized, versioned token implementation and may change after real DAY/DUSK/NIGHT testing.

## Typography

- Prefer host/wx system fonts and respect user/platform scaling.
- Define roles such as title, section label, body, navigation value, unit, and metadata instead of point sizes at call sites.
- Give important numeric values adequate size and stable alignment; keep units adjacent and explicit.
- Use tabular/stable digit alignment where changing measurements would otherwise cause visual jitter, provided the chosen system font supports it or equivalent measured alignment is implemented.
- Avoid all-caps body text, decorative fonts, fake seven-segment displays, and weight-only state cues.
- Measure using the actual font and DPI before truncating or aligning.

## Spacing, radius, and density

Centralize a small spacing scale based on logical/DIP units. Use compact maritime-tool density without crowding targets or values. Corner radii should be restrained and consistent; they communicate grouping, not decoration. Adapt padding and control size for touch only when the platform/task requires it.

For instruments:

- value hierarchy is more important than card decoration;
- avoid drawing a separate bezel/card around every scalar value;
- keep geometry stable during live updates;
- reserve enough width for expected value, sign, unit and state text so updates do not continuously reflow nearby controls.

## Digital instrument grammar

The DevKit standardizes five reusable presentation primitives:

- `Value`
- `Level`
- `Tape`
- `Trend`
- `State`

Plugins should compose these primitives rather than inventing bespoke gauges for each sensor. See `DIGITAL_INSTRUMENTS.md` for selection rules, examples, validity handling and anti-patterns.

By default do **not** use simulated round gauges, fake needles, gloss, 3D bezels, fake LCDs or decorative arcs that merely duplicate a number.

## Icons and pictograms

- Prefer scalable assets supported by the target build/API.
- Maintain legibility in every scheme and DPI; provide text/tooltips for non-obvious actions.
- Use icons as explanatory aids, never replacements for official chart symbols or authoritative values.
- Do not recolor literal navigation symbols through the generic icon pipeline.

### Canonical plugin icon assets and embedding

The following is a **DevKit convention derived from verified OpenCPN host behavior**, not an upstream OpenCPN requirement.

- Keep editable/canonical SVG masters under `assets/icons/`. Do not maintain a second hand-copied SVG payload in C++ source or a checked-in generated header.
- When an icon must be embedded in the plugin binary, read the canonical SVG files during CMake configure and use `configure_file(... @ONLY)` to generate a C++11 header containing the SVG payload.
- Add every source SVG used by the generated header to the directory `CMAKE_CONFIGURE_DEPENDS` property. A later icon edit must therefore trigger CMake regeneration on the next build.
- Prefer compile-time embedding when the package does not install loose icon files or when relying on runtime asset paths would make deployment fragile.
- Treat `GetPlugInBitmap()` as a stable plugin identity. OpenCPN copies the bitmap into its plugin container during plugin activation and does not generally re-query it merely because a later `SetColorScheme()` callback occurs. Use one theme-tolerant identity asset rather than assuming this bitmap dynamically tracks DAY/DUSK/NIGHT.
- Treat toolbar imagery separately from plugin identity. Where verified for the target API/core version, a plugin may rebuild or replace toolbar bitmaps from `SetColorScheme()` to provide DAY/DUSK/NIGHT variants.
- Do not infer that a successful build proves icon integration. Validate transparency, checked/unchecked state, DAY/DUSK/NIGHT, 100% and high-DPI scaling, and the plugin-manager identity in a real OpenCPN runtime.

A typical configure-time embedding shape is:

```cmake
set(MYPLUGIN_ICON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/myplugin.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/myplugin-white.svg
)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${MYPLUGIN_ICON_FILES}
)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/myplugin.svg
    MYPLUGIN_ICON_DAY_SVG)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/myplugin-white.svg
    MYPLUGIN_ICON_WHITE_SVG)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/in-files/myplugin_icons.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/myplugin_icons.h
    @ONLY
)
```

The generated directory is a build artifact and must not be committed.

## Accessibility and legibility

- Never rely on color alone.
- Preserve visible keyboard focus and a logical tab order.
- Check text/background and state boundaries in each OpenCPN scheme.
- Support resizing, long translations, high DPI, and platform font differences.
- Distinguish hover, focus, selection, disabled, stale, warning, alarm, unavailable, invalid and unknown states by at least one non-color cue.
- Respect reduced-motion preferences where the platform exposes them; ordinary interactions do not blink.

## Implementation shape

Use one style service/value object per plugin. Views request semantic colors, fonts, metrics, pens, icons and instrument roles; they never choose raw values. Give the token set a small schema version so snapshots, cache keys, and migrations can identify changes. Unit-test token completeness and state distinctness; validate final appearance in the actual OpenCPN runtime.
