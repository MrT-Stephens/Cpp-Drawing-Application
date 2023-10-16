#include <array>

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/wrapsizer.h>

#include "colourPane.h"
#include "penSizePane.h"
#include "drawingCanvas.h"

#include "Drawing-App-Icon.xpm"

#define MAIN_FONT_TEXT(size) wxFont(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "", wxFONTENCODING_DEFAULT)

enum MenuItem_Ids
{
    ID_None = 100,
    ID_New = 101,
    ID_Open = 102,
    ID_Save = 103,
    ID_SaveAs = 104,
    ID_Export = 105,
    ID_Quit = 106,
    ID_Undo = 107,
    ID_Redo = 108,
    ID_Cut = 109,
    ID_Copy = 110,
    ID_Paste = 111,
    ID_Clear = 112,
    ID_About = 116
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
    wxMenu* m_MenuFile, * m_MenuEdit, * m_MenuHelp;

    // Main Items
    std::wstring m_CurrentFilePath;
    wxSplitterWindow* m_SplitterWindow;
    wxScrolled<wxPanel>* m_ToolsPanel;
    wxBoxSizer* m_ToolsMainSizer;

    // Colour Panes Items
    wxStaticText* m_ColourPaneText;
    wxWrapSizer* m_ColourPanesSizer;

    constexpr static int s_ColourPaneAmount = 12;
    std::array<Colour_Pane*, s_ColourPaneAmount> m_ColourPanes;
    const std::array<wxColour, s_ColourPaneAmount> m_ColourPalette = {
        wxColour(0, 0, 0), wxColour(255, 255, 255),
        wxColour(253, 127, 111), wxColour(126, 176, 213),
        wxColour(178, 224, 97), wxColour(189, 126, 190),
        wxColour(255, 181, 90), wxColour(255, 238, 101),
        wxColour(190, 185, 219), wxColour(253, 204, 229),
        wxColour(139, 211, 199), wxColour(255, 42, 0 )
    };

    // Tool Selection Pane Items
    wxStaticText* m_ToolPaneText;
    wxWrapSizer* m_ToolSelectionPanesSizer;

    constexpr static int s_ToolSelectionPaneAmount = 5;
    std::array<Tool_Selection_Pane*, s_ToolSelectionPaneAmount> m_ToolSelectionPanes;

    // Pen Size Pane Items
    wxStaticText* m_PenSizePaneText;
    wxWrapSizer* m_PenSizePanesSizer;

    constexpr static int s_PenSizePaneAmount = 6;
    std::array<PenSize_Pane*, s_PenSizePaneAmount> m_PenSizePanes;

    // Canvas Panel Items
    wxMenu* m_CanvasMenu;
    Drawing_Canvas* m_DrawingCanvas;

private:
    void SetUpMenuBar();
    void SetUpMenuEvents();
    void SetUpSplitterPanels();
    wxMenu* SetUpCanvasContextMenu();

    void SetUpColourPanes(wxWindow* parent, wxSizer* sizer);
    void SetUpPenSizePanes(wxWindow* parent, wxSizer* sizer);
    void SetUpToolSelectionPanes(wxWindow* parent, wxSizer* sizer);

    void SelectColourPane(Colour_Pane* pane);
    void SelectToolSelectionPane(Tool_Selection_Pane* pane);
    void SelectPenSizePane(PenSize_Pane* pane);

    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExport(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

public:
    Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    ~Main_Frame();
};