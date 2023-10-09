#pragma once

#include "selectablePane.h"

class PenSize_Pane : public Selectable_Pane
{
public:
	PenSize_Pane(wxWindow* parent, wxWindowID id, int penSize, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize);

	int GetPenSize() const;
private:
	int m_PenSize;

	virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;
};