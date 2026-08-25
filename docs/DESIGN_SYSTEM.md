# Shared plugin design system

## Status

This page defines a **DevKit convention** for centralized styling. OpenCPN's current plugin header verifies that plugins receive color-scheme changes and can request named colors from the global color table. Exact names, availability, and values are upstream/version dependent; inspect the target source and handle lookup failure.

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

Add tokens for disabled, unavailable, stale, success, separators, overlay halo, and graphs only when their semantics are documented. Do not alias warning/alarm colors to generic interaction colors.

## DAY, DUSK, and NIGHT

Each token has an explicit value or derivation for all three schemes. On a scheme callback:

1. query applicable OpenCPN global colors again;
2. rebuild the immutable token set;
3. invalidate pens, brushes, bitmaps, text metrics, and render caches that embed colors;
4. refresh affected views using the documented host/wx mechanism.

Transformations should reduce large-area luminance and decorative contrast progressively while retaining readable text, focus, selection, and warning/alarm distinction. Validate contrast visually in OpenCPN; do not claim certification from generic contrast ratios alone.

## Interaction family

Chart Inspector's cool blue/cyan family is the current reference for hover, focus, and ordinary selection because it avoids casual use of common maritime safety colors. It is an example, not an immutable RGB mandate. Exact values belong in the centralized, versioned token implementation and may change after real DAY/DUSK/NIGHT testing.

## Typography

- Prefer host/wx system fonts and respect user/platform scaling.
- Define roles such as title, section label, body, navigation value, unit, and metadata instead of point sizes at call sites.
- Give important numeric values adequate size and stable alignment; keep units adjacent and explicit.
- Avoid all-caps body text, decorative fonts, and weight-only state cues.
- Measure using the actual font and DPI before truncating or aligning.

## Spacing, radius, and density

Centralize a small spacing scale based on logical/DIP units. Use compact maritime-tool density without crowding targets or values. Corner radii should be restrained and consistent; they communicate grouping, not decoration. Adapt padding and control size for touch only when the platform/task requires it.

## Icons and pictograms

- Prefer scalable assets supported by the target build/API.
- Maintain legibility in every scheme and DPI; provide text/tooltips for non-obvious actions.
- Use icons as explanatory aids, never replacements for official chart symbols or authoritative values.
- Do not recolor literal navigation symbols through the generic icon pipeline.

## Accessibility and legibility

- Never rely on color alone.
- Preserve visible keyboard focus and a logical tab order.
- Check text/background and state boundaries in each OpenCPN scheme.
- Support resizing, long translations, high DPI, and platform font differences.
- Distinguish hover, focus, selection, disabled, stale, warning, alarm, and unknown states by at least one non-color cue.
- Respect reduced-motion preferences where the platform exposes them; ordinary interactions do not blink.

## Implementation shape

Use one style service/value object per plugin. Views request semantic colors, fonts, metrics, pens, and icons; they never choose raw values. Give the token set a small schema version so snapshots, cache keys, and migrations can identify changes. Unit-test token completeness and state distinctness; validate final appearance in the actual OpenCPN runtime.
