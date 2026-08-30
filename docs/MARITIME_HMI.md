# Maritime HMI baseline

## Scope and status

This is an **unofficial DevKit design baseline**, generalized from Chart Inspector's working principles and the DevKit's reusable digital-instrument work. It is not a transcription of a standard, an implementation specification for ECDIS, or evidence of compliance.

The references below inform vocabulary, consistency, usability, color discipline, information priority and digital presentation:

- IMO resolution MSC.191(79), presentation of navigation-related information;
- IMO circular MSC.1/Circ.1609, standardized user-interface design for navigation equipment;
- IEC 62288, presentation requirements and tests for shipborne navigational displays;
- IEC 60092-504, special features - control and instrumentation;
- IHO S-52 and its Presentation Library, ECDIS chart content/display, colors, and symbols.

Use the official current editions and applicable amendments for any regulated product. A plugin following this page must not claim ECDIS compliance, type approval, certification or suitability as a mandatory carriage system.

For native digital instrumentation, also read `DIGITAL_INSTRUMENTS.md`.

## Information hierarchy

1. Preserve the chart/navigation surface as the primary spatial reference.
2. Show navigation-critical values and operational state before technical metadata.
3. Keep units explicit wherever a number has a physical meaning.
4. Put provenance, raw attributes, debug fields, and provider details in secondary disclosure.
5. Avoid panels or overlays that mask chart hazards, own-ship context, alerts, or essential controls.
6. Prefer direct digital presentation over skeuomorphic copies of analogue instruments unless the analogue form itself carries an operationally useful relationship.

## Digital presentation model

For vessel values such as SOG, heading, depth, tank level, battery state, current, voltage, temperature or RPM, use the shared DevKit primitives defined in `DIGITAL_INSTRUMENTS.md`:

- `Value` for the current operational value;
- `Level` for bounded quantities;
- `Tape` for ordered or cyclic neighbourhood context;
- `Trend` for recent history;
- `State` for validity, operating condition, warning or alarm.

A plugin should compose these primitives instead of inventing a different visual grammar for each measurement.

The primary number is not decoration. It must clearly identify what it represents, its unit, and any reference qualifier that changes interpretation (`HDG`, `COG`, `STW`, `SOG`, true/magnetic, apparent/true, etc.).

## Lighting schemes and luminance

- Follow OpenCPN's active DAY, DUSK, or NIGHT scheme and update all plugin surfaces together.
- Resolve base window/text colors from OpenCPN's global color table where the target API provides them.
- In DUSK and NIGHT, avoid bright large-area fills, pure white blocks, saturated decorative color, and unnecessary contrast.
- Do not implement night mode as a uniform opacity filter; preserve legibility and state separation using scheme-specific semantic tokens.
- Reduce secondary ticks, graph decoration and inactive structure before reducing the prominence of primary values.
- Verify on the actual display and OpenCPN runtime. A desktop screenshot is not a bridge-environment test.

## Color semantics

Red, amber/yellow, and green can carry safety, alarm, warning, caution, or normal-state meaning in maritime contexts. Do not casually reuse them as generic hover, selection, focus, success, or brand colors where confusion is possible.

- Use a non-safety interaction family, currently cool blue/cyan in Chart Inspector, for ordinary interaction.
- Keep literal encoded navigation/chart colors literal. Do not recolor authoritative information merely to fit a theme.
- Pair status color with text, shape, icon, or pattern; never make color the only cue.
- Keep hover, keyboard focus, transient preview, persistent selection, warning, and alarm visually distinct.
- Normal measurements should normally remain neutral; do not make an entire healthy dashboard green.
- Do not blink or flash for ordinary interaction. Any alert motion requires a documented safety rationale, user control where applicable, and runtime review.

## States and interaction

| State | Required distinction |
| --- | --- |
| Hover | transient pointer location; disappears without changing state |
| Focus | keyboard/input target; remains visible independently of hover |
| Selection | persistent chosen object or row |
| Warning | condition needing attention, below alarm semantics |
| Alarm | safety-significant state governed by the product's verified alert model |
| Stale | previously valid data older than the documented freshness limit |
| No data | no usable value is available |
| Invalid | source or validation says the value is unusable |
| Out of range | value exists but lies outside the supported measurement range |
| Unknown | explicit unsupported/unrecognized value, never silently mapped to normal |

Do not silently interpret unknown enumeration values. Display a neutral `Unknown (<raw value>)`-style result when safe, preserve the raw value for diagnosis, and prevent it from acquiring a known safety meaning.

`0` is a valid value and must never be used as a generic missing-data sentinel.

Do not keep the last valid value on screen without clearly indicating when it has become stale.

## Measurement state versus alert state

Keep **data validity** separate from **alert priority**.

Examples:

- a valid 35% battery state of charge is not automatically a warning;
- a configured low-SOC threshold can create a caution/warning while the underlying value remains valid;
- a disconnected battery sensor is a no-data/invalid state, not automatically a battery alarm;
- red must not be selected merely because a value looks undesirable to the designer.

Alert semantics must come from a documented product/plugin alert model.

## Symbols, motion, and overlays

- Icons and pictograms explain plugin actions; they never replace official chart symbology.
- Do not imitate an official symbol for a different plugin meaning.
- Animations may show transition, age, or direction only as supporting information. They never replace authoritative encoded values, timestamps, labels, or status.
- Keep overlay density proportional to zoom and task. Provide decluttering without hiding an alarm or critical selected state.
- Use stable geometry and restrained transitions so the operator can compare successive frames.
- Digital values must not jump horizontally because digit widths or unit placement changes; use stable alignment where appropriate.

## Content and value formatting

- Put label, value, unit, validity/age, and source together when those qualifiers affect safe interpretation.
- Use locale-aware display carefully while preserving unambiguous coordinates, timestamps, decimal signs, and units.
- Distinguish unavailable, stale, invalid, out-of-range, and zero.
- Avoid false precision. Preserve source precision internally but display what is operationally meaningful.
- Make provider-derived or estimated data distinguishable from authoritative chart encoding.
- Keep true/magnetic and apparent/true qualifiers visible where they materially change meaning.
- Where an installation-specific offset changes interpretation, such as depth reference, expose that context in the instrument or adjacent disclosure.

## Review gate

For any navigation-facing change, review DAY/DUSK/NIGHT, dimming, color-vision legibility, keyboard focus, DPI, small screens, chart obstruction, stale/unknown data, alarm/warning separation, update jitter, data gaps and motion disabled. Record which checks were performed in a real OpenCPN runtime.

For any new digital instrument, additionally verify that the chosen `Value`, `Level`, `Tape`, `Trend` or `State` primitive is justified by the information task and that no analogue-looking decoration is being used merely out of habit.
