#pragma once

#include "selectablePane.h"

class Colour_Pane : public Selectable_Pane
{
public:
	Colour_Pane(wxWindow* parent, wxWindowID id, const wxColour& paneColour, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize);

	wxColour GetPaneColour() const;
private:
	wxColour m_PaneColour;

	virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;
};;