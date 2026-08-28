#ifndef DEVKIT_PLUGIN_UI_APP_STYLE_H
#define DEVKIT_PLUGIN_UI_APP_STYLE_H

#include <wx/colour.h>

#include "ocpn_plugin.h"

namespace plugin_ui {

struct AppPalette {
  wxColour background;
  wxColour foreground;
  wxColour inactive;
  wxColour accent;
};

class AppStyle {
public:
  static AppPalette PaletteFor(PI_ColorScheme scheme);
};

}  // namespace plugin_ui

#endif
