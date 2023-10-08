#include "drawingCanvas.h"

Drawing_Canvas::Drawing_Canvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(wxEVT_PAINT, &Drawing_Canvas::OnPaint, this);
	this->Bind(wxEVT_LEFT_DOWN, &Drawing_Canvas::OnMouseDown, this);
	this->Bind(wxEVT_MOTION, &Drawing_Canvas::OnMouseMove, this);
	this->Bind(wxEVT_LEFT_UP, &Drawing_Canvas::OnMouseUp, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Drawing_Canvas::OnMouseLeave, this);
}

Drawing_Canvas::~Drawing_Canvas() noexcept
{
	delete m_ContextMenu;
}

void Drawing_Canvas::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		for (const auto& squiggle : m_Squiggles)
		{
			if (squiggle.GetPointCount() > 1)
			{
				gc->SetPen(wxPen(squiggle.GetColour(), squiggle.GetWidth()));
				gc->StrokeLines(squiggle.GetPointCount(), squiggle.GetPoints().data());
			}
		}

		delete gc;
	}
}

void Drawing_Canvas::OnMouseDown(wxMouseEvent& event)
{
	m_Squiggles.push_back(Squiggle_Path(m_CurrentWidth, m_CurrentColour));
	m_IsDrawing = true;
}

void Drawing_Canvas::OnMouseMove(wxMouseEvent& event)
{
	if (m_IsDrawing)
	{
		m_Squiggles.back().AddPoint(event.GetPosition());
		this->Refresh();
	}
}

void Drawing_Canvas::OnMouseUp(wxMouseEvent& event)
{
	m_IsDrawing = false;
}

void Drawing_Canvas::OnMouseLeave(wxMouseEvent& event)
{
	m_IsDrawing = false;
}

void Drawing_Canvas::OnContextMenuEvent(wxContextMenuEvent& event)
{
	auto clientPosition = event.GetPosition() == wxDefaultPosition
		? wxPoint(this->GetSize().GetWidth() / 2, this->GetSize().GetHeight() / 2)
		: this->ScreenToClient(event.GetPosition());

	this->PopupMenu(m_ContextMenu);
}

void Drawing_Canvas::SetCurrentWidth(int width)
{
	m_CurrentWidth = width;
}

void Drawing_Canvas::SetCurrentColour(const wxColour& colour)
{
	m_CurrentColour = colour;
}

void Drawing_Canvas::ClearCanvas()
{
	m_Squiggles.clear();
	this->Refresh();
}

void Drawing_Canvas::SetPopUpContextMenu(wxMenu* menu)
{
	m_ContextMenu = menu;

	if (m_ContextMenu != nullptr)
	{
		this->Bind(wxEVT_CONTEXT_MENU, &Drawing_Canvas::OnContextMenuEvent, this);
	}
}

void Drawing_Canvas::SaveCanvas()
{
	wxFileDialog saveFileDialog(this, _("Save file"), "", "", "XML (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	XML_Serializer serializer;

	if (!serializer.Serialize(saveFileDialog.GetPath(), m_Squiggles))
	{
		wxMessageBox("Failed to save file", "Error", wxICON_ERROR | wxOK);
	}
}

void Drawing_Canvas::LoadCanvas()
{
	wxFileDialog openFileDialog(this, _("Open file"), "", "", "XML (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	this->ClearCanvas();

	XML_Serializer serializer;

	if (!serializer.Deserialize(openFileDialog.GetPath(), m_Squiggles))
	{
		wxMessageBox("Failed to load file", "Error", wxICON_ERROR | wxOK);
		return;
	}

	this->Refresh();
}