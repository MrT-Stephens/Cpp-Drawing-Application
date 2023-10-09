#include "drawingCanvas.h"

Drawing_Canvas_Base::Drawing_Canvas_Base(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(wxEVT_PAINT, &Drawing_Canvas_Base::OnPaint, this);
	this->Bind(wxEVT_LEFT_DOWN, &Drawing_Canvas_Base::OnMouseDown, this);
	this->Bind(wxEVT_MOTION, &Drawing_Canvas_Base::OnMouseMove, this);
	this->Bind(wxEVT_LEFT_UP, &Drawing_Canvas_Base::OnMouseUp, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Drawing_Canvas_Base::OnMouseLeave, this);
}

Drawing_Canvas_Base::~Drawing_Canvas_Base() noexcept
{
	delete m_ContextMenu;

	for (size_t i = 0; i < m_CanvasObjects.size(); ++i)
	{
		delete m_CanvasObjects[i];
	}
}

void Drawing_Canvas_Base::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		for (const auto& squiggle : m_CanvasObjects)
		{
			squiggle->Draw(gc);
		}

		delete gc;
	}
}

void Drawing_Canvas_Base::OnMouseDown(wxMouseEvent& event)
{
	switch (m_CurrentToolType)
	{
	case Tool_Type::Pencil:
	{
		m_CanvasObjects.push_back(new Canvas_Squiggle(m_CurrentWidth, m_CurrentColour));

		break;
	}
	case Tool_Type::Line:
	{
		m_CanvasObjects.push_back(new Canvas_Line(event.GetPosition(), event.GetPosition(), m_CurrentWidth, m_CurrentColour));

		break;
	}
	case Tool_Type::Rectangle:
	{
		m_CanvasObjects.push_back(new Canvas_Rectangle(event.GetPosition(), m_CurrentColour));

		break;
	}
	case Tool_Type::Circle:
	{
		m_CanvasObjects.push_back(new Canvas_Circle(event.GetPosition(), m_CurrentColour));

		break;
	}
	}

	m_IsDrawing = true;
}

void Drawing_Canvas_Base::OnMouseMove(wxMouseEvent& event)
{
	if (m_IsDrawing)
	{
		m_CanvasObjects.back()->HandleCreationByMouseDrag(event.GetPosition());
		this->Refresh();
	}
}

void Drawing_Canvas_Base::OnMouseUp(wxMouseEvent& event)
{
	m_IsDrawing = false;
}

void Drawing_Canvas_Base::OnMouseLeave(wxMouseEvent& event)
{
	m_IsDrawing = false;
}

void Drawing_Canvas_Base::OnContextMenuEvent(wxContextMenuEvent& event)
{
	auto clientPosition = event.GetPosition() == wxDefaultPosition
		? wxPoint(this->GetSize().GetWidth() / 2, this->GetSize().GetHeight() / 2)
		: this->ScreenToClient(event.GetPosition());

	this->PopupMenu(m_ContextMenu);
}

void Drawing_Canvas_Base::SetCurrentWidth(int width)
{
	m_CurrentWidth = width;
}

void Drawing_Canvas_Base::SetCurrentColour(const wxColour& colour)
{
	m_CurrentColour = colour;
}

void Drawing_Canvas_Base::SetCurrentToolType(Tool_Type toolType)
{
	m_CurrentToolType = toolType;
}

void Drawing_Canvas_Base::ClearCanvas()
{
	for (size_t i = 0; i < m_CanvasObjects.size(); ++i)
	{
		delete m_CanvasObjects[i];
	}

	m_CanvasObjects.clear();
	this->Refresh();
}

void Drawing_Canvas_Base::SetPopUpContextMenu(wxMenu* menu)
{
	m_ContextMenu = menu;

	if (m_ContextMenu != nullptr)
	{
		this->Bind(wxEVT_CONTEXT_MENU, &Drawing_Canvas::OnContextMenuEvent, this);
	}
}

void Drawing_Canvas_Base::SaveCanvas(const wxString& path)
{
	XML_Serializer serializer;

	if (!serializer.Serialize(path, m_CanvasObjects))
	{
		wxMessageBox("Failed to save file", "Error", wxICON_ERROR | wxOK);
	}
}

void Drawing_Canvas_Base::LoadCanvas(const wxString& path)
{
	this->ClearCanvas();

	XML_Serializer serializer;

	if (!serializer.Deserialize(path, m_CanvasObjects))
	{
		wxMessageBox("Failed to load file", "Error", wxICON_ERROR | wxOK);
		return;
	}

	this->Refresh();
}

bool Drawing_Canvas_Base::IsCanvasEmpty() const
{
	return (m_CanvasObjects.empty()) ? true : false;
}

Drawing_Canvas::Drawing_Canvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: Drawing_Canvas_Base(parent, id, pos, size)
{
}

void Drawing_Canvas::Undo()
{
	if (this->CanUndo())
	{
		m_RedoStates.push_back(m_CanvasObjects.back());
		m_CanvasObjects.pop_back();
		this->Refresh();
	}
}

void Drawing_Canvas::Redo()
{
	if (this->CanRedo())
	{
		m_CanvasObjects.push_back(m_RedoStates.back());
		m_RedoStates.pop_back();
		this->Refresh();
	}
}

bool Drawing_Canvas::CanUndo() const
{
	return (!m_CanvasObjects.empty()) ? true : false;
}

bool Drawing_Canvas::CanRedo() const
{
	return (!m_RedoStates.empty()) ? true : false;
}

void Drawing_Canvas::ClearCanvas()
{
	size_t startSize = m_CanvasObjects.size();

	for (size_t i = 0; i < startSize; ++i)
	{
		m_RedoStates.push_back(m_CanvasObjects.back());
		m_CanvasObjects.pop_back();
	}

	this->Refresh();
}

Drawing_Canvas::~Drawing_Canvas() noexcept
{
	for (size_t i = 0; i < m_RedoStates.size(); ++i)
	{
		delete m_RedoStates[i];
	}
}