#pragma once

#include "wx/wx.h"

#include "wx/graphics.h"
#include "wx/settings.h"
#include "wx/dcbuffer.h"

class Selectable_Pane : public wxWindow
{
public:
	Selectable_Pane(wxWindow* parent, wxWindowID id = wxID_ANY, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize);

	wxSize DoGetBestSize() const override;

	void SetSelected(bool selected);

	bool IsSelected() const;

protected:
	virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) = 0;

private:
	bool m_Selected = false;

	void OnPaint(wxPaintEvent& event);
};