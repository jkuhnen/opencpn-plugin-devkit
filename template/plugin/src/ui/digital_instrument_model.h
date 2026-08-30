#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace devkit {
namespace ui {

// Presentation semantics for native digital marine instruments.
//
// Keep protocol/source parsing outside this model. Signal K, NMEA 0183,
// NMEA 2000 and plugin-internal data should be normalized before reaching
// the UI layer.
//
// See docs/DIGITAL_INSTRUMENTS.md in the OpenCPN Plugin DevKit.

enum class InstrumentValidity {
  Valid,
  Stale,
  NoData,
  Invalid,
  OutOfRange,
  Unknown,
};

enum class InstrumentAlertState {
  None,
  Caution,
  Warning,
  Alarm,
};

enum class InstrumentPrimitive {
  Value,
  Level,
  Tape,
  Trend,
  State,
};

struct InstrumentSecondaryValue {
  std::string label;
  std::string value;
  std::string unit;
};

struct InstrumentTrendSample {
  double value = 0.0;
  std::int64_t timestamp_ms = 0;
  bool valid = true;
};

struct DigitalInstrumentModel {
  // User-facing identity, e.g. "SOG", "HEADING", "HOUSE BATTERY".
  std::string label;

  // Display-ready value. Formatting/precision should already be decided by
  // the presentation layer before rendering.
  std::string value;
  std::string unit;

  // Interpretation qualifier, e.g. "TRUE", "MAG", "APPARENT", "PORT".
  std::string qualifier;

  InstrumentValidity validity = InstrumentValidity::NoData;
  InstrumentAlertState alert = InstrumentAlertState::None;

  // Optional source/provenance and freshness context.
  std::string source;
  std::int64_t timestamp_ms = 0;

  // Optional bounded-range context for Level/Tape primitives.
  bool has_range = false;
  double numeric_value = 0.0;
  double range_min = 0.0;
  double range_max = 0.0;

  std::vector<InstrumentSecondaryValue> secondary_values;
  std::vector<InstrumentTrendSample> trend;
};

// Rendering guidance:
// - normal/valid values remain visually neutral;
// - warning/alarm colors are driven by alert semantics, not by arbitrary
//   numeric thresholds inside the widget;
// - stale/no-data/invalid states must remain visibly distinct from Valid;
// - zero is a valid value and must not be treated as missing data;
// - color is never the only state cue;
// - use semantic style tokens and OpenCPN DAY/DUSK/NIGHT schemes;
// - prefer Value/Level/Tape/Trend/State composition over analogue-looking
//   gauges or skeuomorphic dashboards.

}  // namespace ui
}  // namespace devkit
