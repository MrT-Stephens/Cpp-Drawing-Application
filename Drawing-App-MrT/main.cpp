#include "main.h"

bool MrT_App::OnInit()
{
	Main_Frame* mainFrame = new Main_Frame("Drawing App", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);

	mainFrame->Show(true);

	return true;
}

wxIMPLEMENT_APP(MrT_App);

Main_Frame::Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
	SetBackgroundColour(wxSystemSettings::GetAppearance().IsDark() ? wxColour(44, 40, 40) : wxColour(244, 243, 243));
	SetMinSize(FromDIP(wxSize(1000, 800)));

	SetUpMenuBar();
	SetUpSplitterPanels();
	SetUpMenuEvents();

	SelectColourPane(m_ColourPanes[0]);
	SelectPenSizePane(m_PenSizePanes[0]);
}

void Main_Frame::SetUpMenuBar()
{
	m_MenuFile = new wxMenu;
	m_MenuFile->Append(ID_New, "&New\tCtrl-N", "Create a new drawing");
	m_MenuFile->Append(ID_Open, "&Open\tCtrl-O", "Open a drawing");
	m_MenuFile->Append(ID_Save, "&Save\tCtrl-S", "Save the drawing");
	m_MenuFile->Append(ID_SaveAs, "Save &As\tCtrl-Shift-S", "Save the drawing as a new file");
	m_MenuFile->Append(ID_Export, "&Export\tCtrl-E", "Export the drawing as an image");
	m_MenuFile->AppendSeparator();
	m_MenuFile->Append(ID_Quit, "&Quit\tCtrl-Q", "Quit the program");

	m_MenuEdit = new wxMenu;
	m_MenuEdit->Append(ID_Undo, "&Undo\tCtrl-Z", "Undo the last action");
	m_MenuEdit->Append(ID_Redo, "&Redo\tCtrl-Y", "Redo the last action");
	m_MenuEdit->AppendSeparator();
	m_MenuEdit->Append(ID_Cut, "Cu&t\tCtrl-X", "Cut the selected object");
	m_MenuEdit->Append(ID_Copy, "&Copy\tCtrl-C", "Copy the selected object");
	m_MenuEdit->Append(ID_Paste, "&Paste\tCtrl-V", "Paste the selected object");
	m_MenuEdit->Append(ID_Clear, "&Clear\tDel", "Clear the all objects");

	m_MenuView = new wxMenu;
	m_MenuView->Append(ID_ZoomIn, "Zoom &In\tCtrl-+", "Zoom in");
	m_MenuView->Append(ID_ZoomOut, "Zoom &Out\tCtrl--", "Zoom out");
	m_MenuView->Append(ID_ZoomReset, "Reset &Zoom\tCtrl-0", "Reset zoom");

	m_MenuHelp = new wxMenu;
	m_MenuHelp->Append(ID_About, "&About\tF1", "Show info about this program");

	m_MenuBar = new wxMenuBar;
	m_MenuBar->Append(m_MenuFile, "&File");
	m_MenuBar->Append(m_MenuEdit, "&Edit");
	m_MenuBar->Append(m_MenuView, "&View");
	m_MenuBar->Append(m_MenuHelp, "&Help");

	SetMenuBar(m_MenuBar);
}

void Main_Frame::SetUpMenuEvents()
{
	Bind(wxEVT_MENU, &Main_Frame::OnNew, this, ID_New);
	Bind(wxEVT_MENU, &Main_Frame::OnOpen, this, ID_Open);
	Bind(wxEVT_MENU, &Main_Frame::OnSave, this, ID_Save);
	Bind(wxEVT_MENU, &Main_Frame::OnSaveAs, this, ID_SaveAs);
	Bind(wxEVT_MENU, &Main_Frame::OnQuit, this, ID_Quit);

	Bind(wxEVT_MENU, &Main_Frame::OnUndo, this, ID_Undo);
	Bind(wxEVT_MENU, &Main_Frame::OnRedo, this, ID_Redo);
	Bind(wxEVT_MENU, &Main_Frame::OnCut, this, ID_Cut);
	Bind(wxEVT_MENU, &Main_Frame::OnCopy, this, ID_Copy);
	Bind(wxEVT_MENU, &Main_Frame::OnPaste, this, ID_Paste);
	Bind(wxEVT_MENU, &Main_Frame::OnClear, this, ID_Clear);

	Bind(wxEVT_MENU, &Main_Frame::OnZoomIn, this, ID_ZoomIn);
	Bind(wxEVT_MENU, &Main_Frame::OnZoomOut, this, ID_ZoomOut);
	Bind(wxEVT_MENU, &Main_Frame::OnZoomReset, this, ID_ZoomReset);

	Bind(wxEVT_MENU, &Main_Frame::OnAbout, this, ID_About);
}

wxMenu* Main_Frame::SetUpCanvasContextMenu()
{
	m_CanvasMenu = new wxMenu;
	m_CanvasMenu->Append(ID_New, "&New\tCtrl-N", "Create a new drawing");
	m_CanvasMenu->Append(ID_Save, "&Save\tCtrl-S", "Save the drawing");
	m_CanvasMenu->Append(ID_SaveAs, "Save &As\tCtrl-Shift-S", "Save the drawing as a new file");
	m_CanvasMenu->Append(ID_Export, "&Export\tCtrl-E", "Export the drawing as an image");
	m_CanvasMenu->AppendSeparator();
	m_CanvasMenu->Append(ID_Undo, "&Undo\tCtrl-Z", "Undo the last action");
	m_CanvasMenu->Append(ID_Redo, "&Redo\tCtrl-Y", "Redo the last action");
	m_CanvasMenu->Append(ID_Cut, "Cu&t\tCtrl-X", "Cut the selected object");
	m_CanvasMenu->Append(ID_Copy, "&Copy\tCtrl-C", "Copy the selected object");
	m_CanvasMenu->Append(ID_Paste, "&Paste\tCtrl-V", "Paste the selected object");
	m_CanvasMenu->Append(ID_Clear, "&Clear\tDel", "Clear the all objects");
	m_CanvasMenu->AppendSeparator();
	m_CanvasMenu->Append(ID_Quit, "&Quit\tCtrl-Q", "Quit the program");

	return m_CanvasMenu;
}

void Main_Frame::SetUpSplitterPanels()
{
	m_SplitterWindow = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE | wxSP_BORDER);

	m_ToolsPanel = new wxScrolled<wxPanel>(m_SplitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_ToolsPanel->SetBackgroundColour(this->GetBackgroundColour());
	m_ToolsPanel->SetScrollRate(0, FromDIP(10));

	m_ToolsMainSizer = new wxBoxSizer(wxVERTICAL);

	auto colourLabel = new wxStaticText(m_ToolsPanel, wxID_ANY, "Colour", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	colourLabel->SetForegroundColour(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);
	colourLabel->SetFont(wxFontInfo(FromDIP(10)).Bold());

	m_ToolsMainSizer->Add(colourLabel, 0, wxLEFT | wxTOP, FromDIP(5));

	m_ColourPanesSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpColourPanes(m_ToolsPanel, m_ColourPanesSizer);

	m_ToolsMainSizer->Add(m_ColourPanesSizer, 0, wxALL, FromDIP(5));

	auto penSizeLabel = new wxStaticText(m_ToolsPanel, wxID_ANY, "Pen Size", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	penSizeLabel->SetForegroundColour(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);
	penSizeLabel->SetFont(wxFontInfo(FromDIP(10)).Bold());

	m_ToolsMainSizer->Add(penSizeLabel, 0, wxLEFT | wxTOP, FromDIP(5));

	m_PenSizePanesSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpPenSizePanes(m_ToolsPanel, m_PenSizePanesSizer);

	m_ToolsMainSizer->Add(m_PenSizePanesSizer, 0, wxALL, FromDIP(5));

	auto toolsLabel = new wxStaticText(m_ToolsPanel, wxID_ANY, "Tools", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	toolsLabel->SetForegroundColour(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);
	toolsLabel->SetFont(wxFontInfo(FromDIP(10)).Bold());

	m_ToolsMainSizer->Add(toolsLabel, 0, wxLEFT | wxTOP, FromDIP(5));

	m_ToolsPanel->SetSizer(m_ToolsMainSizer);

	m_DrawingCanvas = new Drawing_Canvas(m_SplitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_DrawingCanvas->SetPopUpContextMenu(SetUpCanvasContextMenu());

	m_SplitterWindow->SetMinimumPaneSize(225);
	m_SplitterWindow->SplitVertically(m_ToolsPanel, m_DrawingCanvas, 225);
}

void Main_Frame::SetUpColourPanes(wxWindow* parent, wxSizer* sizer)
{
	for (size_t i = 0; i < s_ColourPaneAmount; ++i)
	{
		m_ColourPanes[i] = new Colour_Pane(parent, wxID_ANY, m_ColourPalette[i]);

		m_ColourPanes[i]->Bind(wxEVT_LEFT_DOWN, [this, i](wxMouseEvent& event) 
			{
				SelectColourPane(m_ColourPanes[i]);
			}
		);

		sizer->Add(m_ColourPanes[i], 0, wxRIGHT | wxBOTTOM, FromDIP(5));
	}
}

void Main_Frame::SetUpPenSizePanes(wxWindow* parent, wxSizer* sizer)
{
	for (size_t i = 0; i < s_PenSizePaneAmount; ++i)
	{
		m_PenSizePanes[i] = new PenSize_Pane(parent, wxID_ANY, i * FromDIP(4) + FromDIP(1));

		m_PenSizePanes[i]->Bind(wxEVT_LEFT_DOWN, [this, i](wxMouseEvent& event)
			{
				SelectPenSizePane(m_PenSizePanes[i]);
			}
		);

		sizer->Add(m_PenSizePanes[i], 0, wxRIGHT | wxBOTTOM, FromDIP(5));
	}
}

void Main_Frame::SelectColourPane(Colour_Pane* pane)
{
	for (size_t i = 0; i < s_ColourPaneAmount; ++i)
	{
		m_ColourPanes[i]->SetSelected(m_ColourPanes[i] == pane ? true : false);
	}

	m_DrawingCanvas->SetCurrentColour(pane->GetPaneColour());
}

void Main_Frame::SelectPenSizePane(PenSize_Pane* pane)
{
	for (size_t i = 0; i < s_PenSizePaneAmount; ++i)
	{
		m_PenSizePanes[i]->SetSelected(m_PenSizePanes[i] == pane ? true : false);
	}

	m_DrawingCanvas->SetCurrentWidth(pane->GetPenSize());
}

void Main_Frame::OnNew(wxCommandEvent& event)
{

}

void Main_Frame::OnOpen(wxCommandEvent& event)
{
	m_DrawingCanvas->LoadCanvas();
}

void Main_Frame::OnSave(wxCommandEvent& event)
{
	wxMessageBox("Save");
}

void Main_Frame::OnSaveAs(wxCommandEvent& event)
{
	m_DrawingCanvas->SaveCanvas();
}

void Main_Frame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void Main_Frame::OnUndo(wxCommandEvent& event)
{
	wxMessageBox("Undo");
}

void Main_Frame::OnRedo(wxCommandEvent& event)
{
	wxMessageBox("Redo");
}

void Main_Frame::OnCut(wxCommandEvent& event)
{
	wxMessageBox("Cut");
}

void Main_Frame::OnCopy(wxCommandEvent& event)
{
	wxMessageBox("Copy");
}

void Main_Frame::OnPaste(wxCommandEvent& event)
{
	wxMessageBox("Paste");
}

void Main_Frame::OnClear(wxCommandEvent& event)
{
	m_DrawingCanvas->ClearCanvas();
}

void Main_Frame::OnZoomIn(wxCommandEvent& event)
{
	wxMessageBox("Zoom In");
}

void Main_Frame::OnZoomOut(wxCommandEvent& event)
{
	wxMessageBox("Zoom Out");
}

void Main_Frame::OnZoomReset(wxCommandEvent& event)
{
	wxMessageBox("Zoom Reset");
}

void Main_Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("About");
}

Main_Frame::~Main_Frame()
{
}