#include "colourPane.h"

Colour_Pane::Colour_Pane(wxWindow* parent, wxWindowID id, const wxColour& paneColour, wxPoint pos, wxSize size)
	: Selectable_Pane(parent, id, pos, size), m_PaneColour(paneColour)
{
}

wxColour Colour_Pane::GetPaneColour() const
{
	return m_PaneColour;
}

void Colour_Pane::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness)
{
	gc->SetPen(wxPen(m_PaneColour));
	gc->SetBrush(wxBrush(m_PaneColour));

	gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);
}