# Digital maritime instruments

## Status and scope

This page defines the DevKit's reusable pattern for **native digital marine data presentation**. It intentionally avoids decorative simulations of analogue gauges. It is an unofficial design convention informed by professional maritime HMI principles; it is **not** an implementation of ECDIS requirements and does not demonstrate IMO/IEC compliance or type approval.

Read this together with `MARITIME_HMI.md` and `DESIGN_SYSTEM.md`.

## Design intent

Present vessel data as information, not as pictures of instruments. Prefer direct numeric values, explicit units, state, trend and context over round dials, needles, bezels, fake LCDs, chrome, shadows or skeuomorphic dashboards.

The canonical instrument vocabulary is:

1. `Value` — one primary numeric or textual value.
2. `Level` — bounded quantity with a linear fill/extent representation.
3. `Tape` — ordered or cyclic scale moving past a fixed reference marker.
4. `Trend` — recent history that gives change and stability context.
5. `State` — operational, validity, warning or alarm condition.

A composite instrument may combine these primitives, but each visual element must have an explicit information purpose.

## Canonical examples

### Value

Use for speed, depth, voltage, current, temperature, RPM and similar point measurements.

```text
SOG
5.4 kn
```

Rules:

- the operational value is visually dominant;
- unit remains adjacent and unambiguous;
- label states what the value actually represents (`SOG`, not generic `Speed` when source semantics are SOG);
- precision reflects operational value, not merely source precision;
- no circular scale is added unless the circular relationship itself carries information.

### Level

Use for bounded quantities such as state of charge, tank level or capacity usage.

```text
HOUSE BATTERY
82 %
━━━━━━━━━━━━━━━░░░
12.7 V   -8.4 A
```

Rules:

- use a horizontal or vertical linear representation by default;
- do not use automotive fuel-pump artwork or simulated mechanical gauges as the primary representation;
- show absolute quantity as secondary information where useful (`82 L`, `145 Ah`);
- clipping at min/max must not hide an out-of-range state;
- thresholds and color meanings belong to the state model, not to decorative styling.

### Tape

Use when relative position within an ordered/cyclic neighbourhood materially helps perception, especially heading, course or angular quantities.

```text
HEADING                     248° T
        230   240   ▲   250   260
─────────────────────────────────
COG 251° T             SOG 5.4 kn
```

Rules:

- keep the reference marker fixed and move the scale/value context;
- clearly distinguish `HDG`, `COG`, `BRG`, `TWA`, etc.;
- expose `TRUE`/`MAGNETIC` or another reference when relevant;
- avoid an ornamental compass rose if the local tape provides the required context;
- for cyclic wraparound, preserve continuity through 359°/000°.

### Trend

Use when rate, oscillation or recent development matters.

```text
DEPTH
8.7 m
▇▆▅▅▄▃▃▂▂▃▄▅
```

Rules:

- trend is supporting information and does not replace the current authoritative value;
- define or expose the time window where interpretation depends on it;
- represent data gaps; never interpolate across missing data in a way that implies continuity without justification;
- do not animate history for decoration;
- stale/invalid samples must not appear as current valid observations.

### State

Use explicit text and/or symbol in addition to semantic color.

Examples:

```text
CHARGING
LOW
ALARM
STALE
NO DATA
INVALID
```

Color alone is never sufficient to express the state.

## Required data model

Digital instrument widgets should receive a structured presentation model rather than raw ad-hoc strings. The implementation language may vary, but the conceptual fields are:

```text
label
value
unit
reference / qualifier
validity
age / timestamp
source
state
threshold context
optional secondary values
optional trend samples
```

Keep acquisition/protocol parsing separate from presentation. Signal K, NMEA 0183, NMEA 2000 or plugin-internal sources should normalize into the same presentation semantics before rendering.

## Data validity states

At minimum distinguish:

- `valid` — current value suitable for normal display;
- `stale` — previously valid but older than the documented freshness limit;
- `unavailable` / `no-data` — no usable value is available;
- `invalid` — source explicitly marks the value invalid or validation failed;
- `out-of-range` — value is present but violates the supported measurement range;
- `unknown` — an enum/state cannot be interpreted safely.

`0` is a valid numeric value and must never be conflated with missing data.

Recommended neutral rendering for invalid data:

```text
DEPTH
—
NO DATA
```

Do not leave the last valid value visually indistinguishable from a current one. A stale value may be retained only when its stale condition is explicit and operationally useful.

## Operational and alert semantics

The instrument layer must separate **measurement state** from **alert priority**.

Examples:

- a battery at 35% may be valid data and normal for one installation;
- the same value may trigger a configured caution in another installation;
- a failed battery sensor is a validity problem, not automatically a battery alarm;
- an alarm state must originate from a documented alert model, not merely from a designer choosing red.

Never use `alarm` as a generic error style.

### Color discipline

Follow `MARITIME_HMI.md` and `DESIGN_SYSTEM.md`:

- normal data should normally remain neutral;
- cool blue/cyan may be used for ordinary interaction, focus and selection when it does not conflict with authoritative symbology;
- amber/yellow is reserved for documented caution/warning semantics;
- red is reserved for documented alarm/emergency semantics;
- green must not be sprayed across normal values merely to mean "everything is fine";
- every safety-significant color cue must have a non-color companion such as text, shape, icon or pattern;
- ordinary values and interactions never blink.

If a verified product alert model requires attention-getting motion, implement it separately from ordinary instrument rendering and review it against the applicable requirements.

## Typography and number formatting

- use the DevKit typography roles, not hard-coded widget font sizes;
- numeric values should use stable alignment where changing digits would otherwise cause visual jitter;
- labels, values and units must remain readable under high DPI and OpenCPN scaling;
- units should be explicit and standardized within the plugin family;
- distinguish angle references (`°T`, `°M`) where required;
- avoid false precision;
- avoid ambiguous abbreviations that can be confused with another navigation quantity.

## Layout rules

- value comes before decoration;
- use restrained borders and cards only for grouping;
- do not draw bezel rings around every value;
- maintain stable component geometry during updates;
- keep related values spatially grouped, e.g. `HDG + COG + SOG`, or `SOC + V + A`;
- responsive layouts may change orientation but should preserve information priority;
- chart-facing overlays must avoid masking hazards, alerts, own-ship context or essential OpenCPN controls.

## Day, dusk and night

Every digital instrument must support OpenCPN DAY/DUSK/NIGHT behavior via semantic style tokens.

- large-area luminance must reduce in dusk/night;
- normal information should not become saturated simply to remain visible;
- warning/alarm/state distinctions must survive all schemes;
- trend graphics, inactive scale ticks and secondary labels should reduce emphasis before primary values do;
- no separate faux-night overlay/filter should be applied over an otherwise daytime instrument.

## Templates by data type

| Data | Preferred primitive | Required qualifiers / context |
| --- | --- | --- |
| SOG | Value | `kn`; source/validity when relevant |
| STW | Value | explicitly `STW`; do not label simply Speed |
| Heading | Tape + Value | heading type/source; true/magnetic reference |
| COG | Value or Tape | true/magnetic reference as applicable |
| Depth | Value + optional Trend | transducer/reference offset context when operationally relevant |
| Battery SOC | Level + Value + State | `%`; optionally V/A/time remaining |
| Battery voltage | Value + optional Trend | `V`, bank/source |
| Battery current | Value | `A`, sign convention documented |
| Fuel/fresh/waste tank | Level + Value | `%` and/or volume, tank identity |
| RPM | Value + optional Trend | `rpm`, engine identity |
| Temperature | Value + optional Trend | unit and sensor identity |
| Wind angle | Tape/Value | apparent/true and reference |
| Wind speed | Value | apparent/true and unit |

## Reusable widget contract

A DevKit-compatible implementation should make the five primitive types reusable rather than creating bespoke visuals for every plugin.

Suggested conceptual API:

```text
DigitalValue(model, style)
DigitalLevel(model, range, style)
DigitalTape(model, scale, style)
DigitalTrend(model, history, style)
DigitalState(state, style)
```

Composites such as `BatteryInstrument` or `NavigationInstrument` should be compositions of these primitives.

The reusable layer must not know whether a value came from Signal K, NMEA or another provider.

## Anti-patterns

Do not use by default:

- round gauges copied from mechanical instruments;
- fake needles;
- fake seven-segment/LCD styling;
- gradients, gloss, shadows or 3D bezels;
- decorative arcs that duplicate a numeric percentage;
- permanently green normal-state dashboards;
- red for ordinary invalid form/input states where it could be mistaken for an operational alarm;
- unexplained icons instead of labels/units;
- silent hold-last-value behavior after the source is stale;
- gauges whose scale or unit changes without a clear visible indication.

## Review checklist

For every new digital instrument verify:

- primitive choice (`Value`, `Level`, `Tape`, `Trend`, `State`) is justified by the information task;
- label and unit are unambiguous;
- zero, no-data, stale, invalid and unknown are distinguishable;
- safety-significant states are not conveyed by color alone;
- normal data does not consume warning/alarm colors;
- DAY/DUSK/NIGHT are implemented using semantic tokens;
- high DPI, resize and long translations do not corrupt hierarchy;
- update cadence does not cause distracting flicker or layout jitter;
- any trend handles gaps and stale data honestly;
- navigation-facing layouts are checked in a real OpenCPN runtime.

## Regulatory positioning

These patterns are deliberately aligned with the usability direction of professional shipborne HMI guidance, including IMO MSC.191(79), IMO MSC.1/Circ.1609 and IEC 62288, and with the DevKit's broader monitoring/control design references. They remain **design guidance only**.

A plugin must not claim that following this template makes it compliant, approved, certified, ECDIS-equivalent or suitable as a mandatory carriage system. Any such claim requires the complete applicable standard set, product scope, hardware/environmental requirements, verification, testing and approval process.
