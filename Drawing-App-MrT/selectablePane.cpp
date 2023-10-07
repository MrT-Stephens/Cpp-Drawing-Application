#include "selectablePane.h"

Selectable_Pane::Selectable_Pane(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size)
	: wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	Bind(wxEVT_PAINT, &Selectable_Pane::OnPaint, this);
}

wxSize Selectable_Pane::DoGetBestSize() const
{
	return FromDIP(wxSize(60, 60));
}

void Selectable_Pane::SetSelected(bool selected)
{
	this->m_Selected = selected;
	this->Refresh();
}

bool Selectable_Pane::IsSelected() const
{
	return this->m_Selected;
}

void Selectable_Pane::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.SetBackground(wxBrush(this->GetParent()->GetBackgroundColour()));
	dc.Clear();

	auto gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		wxRect selectedRect{ 0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight() };
		selectedRect.Deflate(FromDIP(1));

		wxRect contentRect{ 0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight() };
		contentRect.Deflate(FromDIP(2));

		const auto roundness = FromDIP(4);

		DrawContent(gc, contentRect, roundness);

		if (m_Selected)
		{
			gc->SetPen(wxSystemSettings::GetAppearance().IsDark() ? wxPen(*wxWHITE, FromDIP(2)) : wxPen(*wxBLACK, FromDIP(2)));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);

			gc->DrawRoundedRectangle(selectedRect.GetX(), selectedRect.GetY(), selectedRect.GetWidth(), selectedRect.GetHeight(), roundness);
		}

		delete gc;
	}
}