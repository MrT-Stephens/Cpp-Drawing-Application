#include "main.h"

bool MrT_App::OnInit()
{
	Main_Frame* mainFrame = new Main_Frame("Drawing App", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);

	mainFrame->SetIcon(wxIcon(Drawing_App_Icon));

	mainFrame->Show(true);

	return true;
}

wxIMPLEMENT_APP(MrT_App);

Main_Frame::Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
	SetBackgroundColour(wxSystemSettings::GetAppearance().IsDark() ? wxColour(44, 40, 40) : wxColour(244, 243, 243));
	SetForegroundColour(wxSystemSettings::GetAppearance().IsDark() ? wxColour(244, 243, 243) : wxColour(44, 40, 40));
	SetMinSize(FromDIP(wxSize(1000, 800)));
	SetFont(MAIN_FONT_TEXT(13));

	SetUpMenuBar();
	SetUpSplitterPanels();
	SetUpMenuEvents();

	SelectColourPane(m_ColourPanes[0]);
	SelectPenSizePane(m_PenSizePanes[0]);
	SelectToolSelectionPane(m_ToolSelectionPanes[0]);
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

	m_MenuHelp = new wxMenu;
	m_MenuHelp->Append(ID_About, "&About\tF1", "Show info about this program");

	m_MenuBar = new wxMenuBar;
	m_MenuBar->Append(m_MenuFile, "&File");
	m_MenuBar->Append(m_MenuEdit, "&Edit");
	m_MenuBar->Append(m_MenuHelp, "&Help");

	SetMenuBar(m_MenuBar);
}

void Main_Frame::SetUpMenuEvents()
{
	Bind(wxEVT_MENU, &Main_Frame::OnNew, this, ID_New);
	Bind(wxEVT_MENU, &Main_Frame::OnOpen, this, ID_Open);
	Bind(wxEVT_MENU, &Main_Frame::OnSave, this, ID_Save);
	Bind(wxEVT_MENU, &Main_Frame::OnSaveAs, this, ID_SaveAs);
	Bind(wxEVT_MENU, &Main_Frame::OnExport, this, ID_Export);
	Bind(wxEVT_MENU, &Main_Frame::OnQuit, this, ID_Quit);

	Bind(wxEVT_MENU, &Main_Frame::OnUndo, this, ID_Undo);
	Bind(wxEVT_MENU, &Main_Frame::OnRedo, this, ID_Redo);
	Bind(wxEVT_MENU, &Main_Frame::OnCut, this, ID_Cut);
	Bind(wxEVT_MENU, &Main_Frame::OnCopy, this, ID_Copy);
	Bind(wxEVT_MENU, &Main_Frame::OnPaste, this, ID_Paste);
	Bind(wxEVT_MENU, &Main_Frame::OnClear, this, ID_Clear);

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

	m_ColourPaneText = new wxStaticText(m_ToolsPanel, wxID_ANY, "Colour");
	m_ColourPaneText->SetForegroundColour(this->GetForegroundColour());
	m_ColourPaneText->SetFont(this->GetFont().Bold());

	m_ToolsMainSizer->Add(m_ColourPaneText, 0, wxALL | wxALIGN_LEFT, FromDIP(10));

	m_ColourPanesSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpColourPanes(m_ToolsPanel, m_ColourPanesSizer);

	m_ToolsMainSizer->Add(m_ColourPanesSizer, 0, wxALL, FromDIP(5));

	m_ToolPaneText = new wxStaticText(m_ToolsPanel, wxID_ANY, "Tool");
	m_ToolPaneText->SetForegroundColour(this->GetForegroundColour());
	m_ToolPaneText->SetFont(this->GetFont().Bold());

	m_ToolsMainSizer->Add(m_ToolPaneText, 0, wxALL | wxALIGN_LEFT, FromDIP(10));

	m_ToolSelectionPanesSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpToolSelectionPanes(m_ToolsPanel, m_ToolSelectionPanesSizer);

	m_ToolsMainSizer->Add(m_ToolSelectionPanesSizer, 0, wxALL, FromDIP(5));

	m_PenSizePaneText = new wxStaticText(m_ToolsPanel, wxID_ANY, "Pen Size");
	m_PenSizePaneText->SetForegroundColour(this->GetForegroundColour());
	m_PenSizePaneText->SetFont(this->GetFont().Bold());

	m_ToolsMainSizer->Add(m_PenSizePaneText, 0, wxALL | wxALIGN_LEFT, FromDIP(10));

	m_PenSizePanesSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpPenSizePanes(m_ToolsPanel, m_PenSizePanesSizer);

	m_ToolsMainSizer->Add(m_PenSizePanesSizer, 0, wxALL, FromDIP(5));

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

void Main_Frame::SetUpToolSelectionPanes(wxWindow* parent, wxSizer* sizer)
{
	for (size_t i = 0; i < s_ToolSelectionPaneAmount; ++i)
	{
		m_ToolSelectionPanes[i] = new Tool_Selection_Pane(parent, wxID_ANY, (Tool_Type)(1000 + i));

		m_ToolSelectionPanes[i]->Bind(wxEVT_LEFT_DOWN, [this, i](wxMouseEvent& event)
			{
				SelectToolSelectionPane(m_ToolSelectionPanes[i]);
			}
		);

		sizer->Add(m_ToolSelectionPanes[i], 0, wxRIGHT | wxBOTTOM, FromDIP(5));
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

void Main_Frame::SelectToolSelectionPane(Tool_Selection_Pane* pane)
{
	for (size_t i = 0; i < s_ToolSelectionPaneAmount; ++i)
	{
		m_ToolSelectionPanes[i]->SetSelected(m_ToolSelectionPanes[i] == pane ? true : false);
	}

	if (pane->GetToolType() == Tool_Type::Pencil || pane->GetToolType() == Tool_Type::Line)
	{
		m_PenSizePaneText->Show(true);
		m_PenSizePanesSizer->ShowItems(true);
	}
	else
	{
		m_PenSizePaneText->Show(false);
		m_PenSizePanesSizer->ShowItems(false);
	}

	m_DrawingCanvas->SetCurrentToolType(pane->GetToolType());
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
	if (m_DrawingCanvas->IsCanvasEmpty())
	{
		return;
	}
	else
	{
		wxMessageDialog newDialog(this, "Do you want to save changes to Untitled?", "Drawing App", wxYES_NO | wxCANCEL | wxCANCEL_DEFAULT | wxICON_WARNING);

		int result = newDialog.ShowModal();

		if (result == wxID_YES)
		{
			if (m_CurrentFilePath.empty())
			{
				wxFileDialog saveFileDialog(this, "Save Drawing", "", "", "Drawing files (*.drw)|*.drw", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

				if (saveFileDialog.ShowModal() == wxID_CANCEL)
					return;

				m_DrawingCanvas->SaveCanvas(saveFileDialog.GetPath());
			}
			else
			{
				m_DrawingCanvas->SaveCanvas(m_CurrentFilePath);
			}
		}
		else if (result == wxID_CANCEL)
		{
			return;
		}

		m_DrawingCanvas->ClearCanvas();
		m_CurrentFilePath = L"";
	}
}

void Main_Frame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open Drawing", "", "", "Drawing files (*.drw)|*.drw", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	m_CurrentFilePath = openFileDialog.GetPath().ToStdWstring();

	m_DrawingCanvas->LoadCanvas(openFileDialog.GetPath());
}

void Main_Frame::OnSave(wxCommandEvent& event)
{
	if (m_CurrentFilePath.empty())
	{
		OnSaveAs(event);
	}
	else
	{
		m_DrawingCanvas->SaveCanvas(m_CurrentFilePath);
	}
}

void Main_Frame::OnSaveAs(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, "Save Drawing", "", "", "Drawing files (*.drw)|*.drw", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	m_CurrentFilePath = saveFileDialog.GetPath().ToStdWstring();

	m_DrawingCanvas->SaveCanvas(saveFileDialog.GetPath());
}

void Main_Frame::OnExport(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save drawing"), "", "",
		"PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	m_DrawingCanvas->ExportCanvas(saveFileDialog.GetPath());
}

void Main_Frame::OnQuit(wxCommandEvent& event)
{
	if (m_DrawingCanvas->IsCanvasEmpty())
	{
		Close(true);
	}
	else
	{
		wxMessageDialog quitDialog(this, "Do you want to save changes to Untitled?", "Drawing App", wxYES_NO | wxCANCEL | wxCANCEL_DEFAULT | wxICON_WARNING);

		int result = quitDialog.ShowModal();

		if (result == wxID_YES)
		{
			if (m_CurrentFilePath.empty())
			{
				wxFileDialog saveFileDialog(this, "Save Drawing", "", "", "Drawing files (*.drw)|*.drw", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

				if (saveFileDialog.ShowModal() == wxID_CANCEL)
					return;

				m_DrawingCanvas->SaveCanvas(saveFileDialog.GetPath());
			}
			else
			{
				m_DrawingCanvas->SaveCanvas(m_CurrentFilePath);
			}
		}
		else if (result == wxID_CANCEL)
		{
			return;
		}

		Close(true);
	}
}

void Main_Frame::OnUndo(wxCommandEvent& event)
{
	m_DrawingCanvas->Undo();
}

void Main_Frame::OnRedo(wxCommandEvent& event)
{
	m_DrawingCanvas->Redo();
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

void Main_Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("About");
}

Main_Frame::~Main_Frame()
{
}