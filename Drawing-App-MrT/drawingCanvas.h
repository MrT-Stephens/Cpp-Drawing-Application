#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

#include <vector>

#include "canvasObject.h"
#include "xmlSerializer.h"
#include "toolSelectionPane.h"

class Drawing_Canvas_Base : public wxWindow
{
public:
	Drawing_Canvas_Base(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);

	virtual ~Drawing_Canvas_Base() noexcept;

	void SetCurrentWidth(int width);
	void SetPopUpContextMenu(wxMenu* menu);
	void SetCurrentColour(const wxColour& colour);
	void SetCurrentToolType(Tool_Type toolType);

	virtual void ClearCanvas();

	void SaveCanvas(const wxString& path);
	void LoadCanvas(const wxString& path);

	bool IsCanvasEmpty() const;

protected:
	void OnPaint(wxPaintEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnContextMenuEvent(wxContextMenuEvent& event);

protected:
	bool m_IsDrawing;
	wxMenu* m_ContextMenu = nullptr;

	Tool_Type m_CurrentToolType = Tool_Type::Pencil;
	int m_CurrentWidth = 1;
	wxColour m_CurrentColour = *wxBLACK;
	std::vector<Canvas_Object*> m_CanvasObjects;
};

class Drawing_Canvas : public Drawing_Canvas_Base
{
public:
	Drawing_Canvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);

	virtual ~Drawing_Canvas() noexcept;

	void Undo();
	void Redo();

	bool CanUndo() const;
	bool CanRedo() const;

	virtual void ClearCanvas() override;

private:
	std::vector<Canvas_Object*> m_RedoStates;
};