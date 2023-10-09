#pragma once

#include <array>

#include "selectablePane.h"

enum class Tool_Type
{
	Pencil = 1000,
	Line = 1001,
	Rectangle = 1002,
	Circle = 1003
};

class Tool_Selection_Pane : public Selectable_Pane
{
public:
	Tool_Selection_Pane(wxWindow* parent, wxWindowID id, Tool_Type toolType, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize);

	Tool_Type GetToolType() const;
private:
	virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;

	Tool_Type m_ToolType;
};