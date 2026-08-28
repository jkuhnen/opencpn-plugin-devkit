#include "ui/app_style.h"

#include <algorithm>

#include <wx/settings.h>

namespace plugin_ui {
namespace {

wxColour Scale(const wxColour &colour, double factor) {
  factor = std::max(0.0, std::min(1.0, factor));
  return wxColour(static_cast<unsigned char>(colour.Red() * factor),
                  static_cast<unsigned char>(colour.Green() * factor),
                  static_cast<unsigned char>(colour.Blue() * factor));
}

}  // namespace

AppPalette AppStyle::PaletteFor(PI_ColorScheme scheme) {
  AppPalette palette;
  palette.background = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
  palette.foreground = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  palette.inactive = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);

  // Prefer the host's active color table. Check the documented bool result
  // explicitly so the system fallback does not depend on failure behavior.
  wxColour hostColour;
  if (GetGlobalColor("DILG0", &hostColour)) palette.background = hostColour;
  if (GetGlobalColor("DILG4", &hostColour)) palette.foreground = hostColour;
  if (GetGlobalColor("DILG3", &hostColour)) palette.inactive = hostColour;

  // Cool blue/cyan is a generic interaction example, not an alarm, warning,
  // official chart color, or immutable brand value.
  palette.accent = wxColour(45, 155, 185);
  if (scheme == PI_GLOBAL_COLOR_SCHEME_DUSK) {
    palette.accent = Scale(palette.accent, 0.78);
  } else if (scheme == PI_GLOBAL_COLOR_SCHEME_NIGHT) {
    palette.accent = Scale(palette.accent, 0.55);
  }

  return palette;
}

}  // namespace plugin_ui
