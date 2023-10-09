#include "penSizePane.h"

PenSize_Pane::PenSize_Pane(wxWindow* parent, wxWindowID id, int penSize, wxPoint pos, wxSize size)
	: Selectable_Pane(parent, id, pos, size), m_PenSize(penSize)
{
}

int PenSize_Pane::GetPenSize() const
{
	return m_PenSize;
}

void PenSize_Pane::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const
{
	gc->SetPen(wxPen(wxSystemSettings::GetAppearance().IsDark() ? *wxBLACK : *wxWHITE));
	gc->SetBrush(wxBrush(wxSystemSettings::GetAppearance().IsDark() ? *wxBLACK : *wxWHITE));

	gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);

	gc->SetPen(wxPen(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK));
	gc->SetBrush(wxBrush(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK));

	gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 2 - m_PenSize / 2, rect.GetY() + rect.GetHeight() / 2 - m_PenSize / 2,m_PenSize, m_PenSize);
}