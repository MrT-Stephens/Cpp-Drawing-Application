#include <array>

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/wrapsizer.h>

#include "colourPane.h"
#include "penSizePane.h"

#define MAIN_FONT_TEXT(size) wxFont(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "", wxFONTENCODING_DEFAULT)

enum Menu_Ids
{
    ID_None = 0,
    ID_New = 1,
    ID_Open = 2,
    ID_Save = 3,
    ID_SaveAs = 4,
    ID_Export = 5,
    ID_Quit = 6,
    ID_Undo = 7,
    ID_Redo = 8,
    ID_Cut = 9,
    ID_Copy = 10,
    ID_Paste = 11,
    ID_Delete = 12,
    ID_ZoomIn = 13,
    ID_ZoomOut = 14,
    ID_ZoomReset = 15,
    ID_About = 16
};

class MrT_App : public wxApp
{
public:
    virtual bool OnInit() override;
};

class Main_Frame : public wxFrame
{
private:
    // Menu Bar and Menu Items
    wxMenuBar* m_MenuBar;
    wxMenu* m_MenuFile, * m_MenuEdit, * m_MenuView, * m_MenuHelp;

    // Main Items
    const wxColour m_LightModeColour = wxColour(244, 243, 243);
    const wxColour m_DarkModeColour = wxColour(44, 40, 40);
    wxSplitterWindow* m_SplitterWindow;
    wxScrolled<wxPanel>* m_ToolsPanel;
    wxBoxSizer* m_ToolsMainSizer;

    // Colour Panes Items
    wxWrapSizer* m_ColourPanesSizer;

    constexpr static int s_ColourPaneAmount = 12;
    std::array<Colour_Pane*, s_ColourPaneAmount> m_ColourPanes;
    std::array<wxColour, s_ColourPaneAmount> m_ColourPalette = {
        wxColour(0, 0, 0), wxColour(255, 255, 255),
        wxColour(253, 127, 111), wxColour(126, 176, 213),
        wxColour(178, 224, 97), wxColour(189, 126, 190),
        wxColour(255, 181, 90), wxColour(255, 238, 101),
        wxColour(190, 185, 219), wxColour(253, 204, 229),
        wxColour(139, 211, 199), wxColour(203, 255, 122) 
    };

    // Pen Size Pane Items
    wxWrapSizer* m_PenSizePanesSizer;

    constexpr static int s_PenSizePaneAmount = 6;
    std::array<PenSize_Pane*, s_PenSizePaneAmount> m_PenSizePanes;

    // Canvas Panel Items
    wxPanel* m_CanvasPanel;

private:
    void SetUpMenuBar();
    void SetUpSplitterPanels();

    void SetUpColourPanes(wxWindow* parent, wxSizer* sizer);
    void SetUpPenSizePanes(wxWindow* parent, wxSizer* sizer);

    void SelectColourPane(Colour_Pane* pane);
    void SelectPenSizePane(PenSize_Pane* pane);

    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);
    void OnZoomReset(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

public:
    Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);
};