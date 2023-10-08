#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

#include <vector>

#include "squigglePath.h"
#include "xmlSerializer.h"

class Drawing_Canvas : public wxWindow
{
public:
	Drawing_Canvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);

	virtual ~Drawing_Canvas() noexcept;

	void SetCurrentWidth(int width);
	void SetPopUpContextMenu(wxMenu* menu);
	void SetCurrentColour(const wxColour& colour);

	void ClearCanvas();

	void SaveCanvas();
	void LoadCanvas();

private:
	void OnPaint(wxPaintEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnContextMenuEvent(wxContextMenuEvent& event);

private:
	bool m_IsDrawing;
	wxMenu* m_ContextMenu = nullptr;

	int m_CurrentWidth = 1;
	wxColour m_CurrentColour = *wxBLACK;
	std::vector<Squiggle_Path> m_Squiggles;
};