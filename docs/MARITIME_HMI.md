# Maritime HMI baseline

## Scope and status

This is an **unofficial DevKit design baseline**, generalized from Chart Inspector's working principles. It is not a transcription of a standard, an implementation specification for ECDIS, or evidence of compliance.

The references below inform vocabulary, consistency, usability, color discipline, and information priority:

- IMO resolution MSC.191(79), presentation of navigation-related information;
- IMO circular MSC.1/Circ.1609, standardized user-interface design for navigation equipment;
- IEC 62288, presentation requirements and tests for shipborne navigational displays;
- IHO S-52 and its Presentation Library, ECDIS chart content/display, colors, and symbols.

Use the official current editions and applicable amendments for any regulated product. A plugin following this page must not claim ECDIS compliance, type approval, or certification.

## Information hierarchy

1. Preserve the chart/navigation surface as the primary spatial reference.
2. Show navigation-critical values and operational state before technical metadata.
3. Keep units explicit wherever a number has a physical meaning.
4. Put provenance, raw attributes, debug fields, and provider details in secondary disclosure.
5. Avoid panels or overlays that mask chart hazards, own-ship context, alerts, or essential controls.

## Lighting schemes and luminance

- Follow OpenCPN's active DAY, DUSK, or NIGHT scheme and update all plugin surfaces together.
- Resolve base window/text colors from OpenCPN's global color table where the target API provides them.
- In DUSK and NIGHT, avoid bright large-area fills, pure white blocks, saturated decorative color, and unnecessary contrast.
- Do not implement night mode as a uniform opacity filter; preserve legibility and state separation using scheme-specific semantic tokens.
- Verify on the actual display and OpenCPN runtime. A desktop screenshot is not a bridge-environment test.

## Color semantics

Red, amber/yellow, and green can carry safety, alarm, warning, caution, or normal-state meaning in maritime contexts. Do not casually reuse them as generic hover, selection, focus, success, or brand colors where confusion is possible.

- Use a non-safety interaction family, currently cool blue/cyan in Chart Inspector, for ordinary interaction.
- Keep literal encoded navigation/chart colors literal. Do not recolor authoritative information merely to fit a theme.
- Pair status color with text, shape, icon, or pattern; never make color the only cue.
- Keep hover, keyboard focus, transient preview, persistent selection, warning, and alarm visually distinct.
- Do not blink or flash for ordinary interaction. Any alert motion requires a documented safety rationale, user control where applicable, and runtime review.

## States and interaction

| State | Required distinction |
| --- | --- |
| Hover | transient pointer location; disappears without changing state |
| Focus | keyboard/input target; remains visible independently of hover |
| Selection | persistent chosen object or row |
| Warning | condition needing attention, below alarm semantics |
| Alarm | safety-significant state governed by the product's verified alert model |
| Unknown | explicit unsupported/unrecognized value, never silently mapped to normal |

Do not silently interpret unknown enumeration values. Display a neutral `Unknown (<raw value>)`-style result when safe, preserve the raw value for diagnosis, and prevent it from acquiring a known safety meaning.

## Symbols, motion, and overlays

- Icons and pictograms explain plugin actions; they never replace official chart symbology.
- Do not imitate an official symbol for a different plugin meaning.
- Animations may show transition, age, or direction only as supporting information. They never replace authoritative encoded values, timestamps, labels, or status.
- Keep overlay density proportional to zoom and task. Provide decluttering without hiding an alarm or critical selected state.
- Use stable geometry and restrained transitions so the operator can compare successive frames.

## Content and value formatting

- Put label, value, unit, validity/age, and source together when those qualifiers affect safe interpretation.
- Use locale-aware display carefully while preserving unambiguous coordinates, timestamps, decimal signs, and units.
- Distinguish unavailable, stale, invalid, out-of-range, and zero.
- Avoid false precision. Preserve source precision internally but display what is operationally meaningful.
- Make provider-derived or estimated data distinguishable from authoritative chart encoding.

## Review gate

For any navigation-facing change, review DAY/DUSK/NIGHT, dimming, color-vision legibility, keyboard focus, DPI, small screens, chart obstruction, stale/unknown data, alarm/warning separation, and motion disabled. Record which checks were performed in a real OpenCPN runtime.
