#include "plugin.h"

#include <wx/brush.h>
#include <wx/dcmemory.h>
#include <wx/pen.h>

#include "ui/app_style.h"
#include "version.h"

extern "C" DECL_EXP opencpn_plugin *create_pi(void *pluginManager) {
  return new {{PLUGIN_CLASS}}(pluginManager);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *plugin) { delete plugin; }

{{PLUGIN_CLASS}}::{{PLUGIN_CLASS}}(void *pluginManager)
    : opencpn_plugin_118(pluginManager) {}

int {{PLUGIN_CLASS}}::Init() {
  RebuildToolbarBitmaps();
  m_pluginBitmap = m_toolbarActiveBitmap;
  m_toolbarId = InsertPlugInTool(
      wxEmptyString, &m_toolbarActiveBitmap, &m_toolbarActiveBitmap,
      wxITEM_CHECK, wxString::FromUTF8("{{PLUGIN_NAME}}"),
      wxString::FromUTF8("Toggle {{PLUGIN_NAME}}"), nullptr, -1, 0, this);
  UpdateToolbarVisual();

  return INSTALLS_TOOLBAR_TOOL | WANTS_TOOLBAR_CALLBACK;
}

bool {{PLUGIN_CLASS}}::DeInit() {
  if (m_toolbarId >= 0) {
    RemovePlugInTool(m_toolbarId);
    m_toolbarId = -1;
  }
  return true;
}

int {{PLUGIN_CLASS}}::GetAPIVersionMajor() {
  return DEVKIT_PLUGIN_API_VERSION_MAJOR;
}

int {{PLUGIN_CLASS}}::GetAPIVersionMinor() {
  return DEVKIT_PLUGIN_API_VERSION_MINOR;
}

int {{PLUGIN_CLASS}}::GetPlugInVersionMajor() {
  return DEVKIT_PLUGIN_VERSION_MAJOR;
}

int {{PLUGIN_CLASS}}::GetPlugInVersionMinor() {
  return DEVKIT_PLUGIN_VERSION_MINOR;
}

int {{PLUGIN_CLASS}}::GetPlugInVersionPatch() {
  return DEVKIT_PLUGIN_VERSION_PATCH;
}

int {{PLUGIN_CLASS}}::GetPlugInVersionPost() {
  return DEVKIT_PLUGIN_VERSION_POST;
}

int {{PLUGIN_CLASS}}::GetToolbarToolCount() { return 1; }

wxBitmap *{{PLUGIN_CLASS}}::GetPlugInBitmap() { return &m_pluginBitmap; }

wxString {{PLUGIN_CLASS}}::GetCommonName() {
  return wxString::FromUTF8("{{PLUGIN_NAME}}");
}

wxString {{PLUGIN_CLASS}}::GetShortDescription() {
  return wxString::FromUTF8("Minimal OpenCPN plugin template");
}

wxString {{PLUGIN_CLASS}}::GetLongDescription() {
  return wxString::FromUTF8(
      "Replace this placeholder with the plugin's real description.");
}

void {{PLUGIN_CLASS}}::OnToolbarToolCallback(int id) {
  if (id != m_toolbarId) return;
  m_active = !m_active;
  UpdateToolbarVisual();
}

void {{PLUGIN_CLASS}}::SetColorScheme(PI_ColorScheme scheme) {
  m_colorScheme = scheme;
  RebuildToolbarBitmaps();
  UpdateToolbarVisual();
}

void {{PLUGIN_CLASS}}::RebuildToolbarBitmaps() {
  const plugin_ui::AppPalette palette =
      plugin_ui::AppStyle::PaletteFor(m_colorScheme);

  const auto buildBitmap = [&](bool active) {
    wxBitmap bitmap(32, 32, 32);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(wxBrush(palette.background));
    dc.Clear();

    const wxColour line = active ? palette.accent : palette.inactive;
    dc.SetPen(wxPen(line, active ? 3 : 2));
    dc.SetBrush(active ? wxBrush(palette.accent) : *wxTRANSPARENT_BRUSH);
    dc.DrawRoundedRectangle(6, 6, 20, 20, 3);
    dc.SetPen(wxPen(active ? palette.background : palette.foreground, 2));
    dc.DrawLine(11, 13, 21, 13);
    dc.DrawLine(11, 19, 21, 19);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
  };

  m_toolbarActiveBitmap = buildBitmap(true);
  m_toolbarInactiveBitmap = buildBitmap(false);
  m_pluginBitmap = m_active ? m_toolbarActiveBitmap : m_toolbarInactiveBitmap;
}

void {{PLUGIN_CLASS}}::UpdateToolbarVisual() {
  m_pluginBitmap = m_active ? m_toolbarActiveBitmap : m_toolbarInactiveBitmap;
  if (m_toolbarId < 0) return;

  wxBitmap *bitmap =
      m_active ? &m_toolbarActiveBitmap : &m_toolbarInactiveBitmap;
  SetToolbarToolBitmaps(m_toolbarId, bitmap, bitmap);
  SetToolbarItemState(m_toolbarId, m_active);
}
