#ifndef {{PLUGIN_ID}}_PLUGIN_H
#define {{PLUGIN_ID}}_PLUGIN_H

#include <wx/bitmap.h>
#include <wx/string.h>

#include "ocpn_plugin.h"

class {{PLUGIN_CLASS}} : public opencpn_plugin_118 {
public:
  explicit {{PLUGIN_CLASS}}(void *pluginManager);
  ~{{PLUGIN_CLASS}}() override = default;

  int Init() override;
  bool DeInit() override;

  int GetAPIVersionMajor() override;
  int GetAPIVersionMinor() override;
  int GetPlugInVersionMajor() override;
  int GetPlugInVersionMinor() override;
  int GetPlugInVersionPatch() override;
  int GetPlugInVersionPost() override;
  int GetToolbarToolCount() override;

  wxBitmap *GetPlugInBitmap() override;
  wxString GetCommonName() override;
  wxString GetShortDescription() override;
  wxString GetLongDescription() override;

  void OnToolbarToolCallback(int id) override;
  void SetColorScheme(PI_ColorScheme scheme) override;

private:
  void RebuildToolbarBitmaps();
  void UpdateToolbarVisual();

  wxBitmap m_pluginBitmap;
  wxBitmap m_toolbarActiveBitmap;
  wxBitmap m_toolbarInactiveBitmap;
  int m_toolbarId = -1;
  bool m_active = true;
  PI_ColorScheme m_colorScheme = PI_GLOBAL_COLOR_SCHEME_DAY;
};

#endif
