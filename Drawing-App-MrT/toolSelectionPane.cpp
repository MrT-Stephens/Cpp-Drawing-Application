#include "toolSelectionPane.h"

Tool_Selection_Pane::Tool_Selection_Pane(wxWindow* parent, wxWindowID id, Tool_Type toolType, wxPoint pos, wxSize size)
	: Selectable_Pane(parent, id, pos, size), m_ToolType(toolType)
{
}

Tool_Type Tool_Selection_Pane::GetToolType() const
{
	return m_ToolType;
}

void Tool_Selection_Pane::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const
{
	gc->SetPen(wxPen(wxSystemSettings::GetAppearance().IsDark() ? *wxBLACK : *wxWHITE));
	gc->SetBrush(wxBrush(wxSystemSettings::GetAppearance().IsDark() ? *wxBLACK : *wxWHITE));

	gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);

	double itemWidth = rect.GetWidth() / 2.0;

	wxAffineMatrix2D transform;
	transform.Translate(rect.GetX(), rect.GetY());
	transform.Translate(rect.GetWidth() / 2, rect.GetHeight() / 2);

	gc->PushState();
	gc->SetTransform(gc->CreateMatrix(transform));

	gc->SetPen(wxPen(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK, FromDIP(3)));
	gc->SetBrush(wxBrush(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK));

	switch (m_ToolType)
	{
	case Tool_Type::Pencil:
	{
		constexpr static int resolution = 100;
		std::array<wxPoint2DDouble, resolution> points = { wxPoint2DDouble(0, 0) };

		for (size_t i = 0; i < resolution; ++i)
		{
			points[i].m_x = (i / (double)resolution - 0.5) * itemWidth;
			points[i].m_y = sin(points[i].m_x / itemWidth * M_PI * 2.0) * itemWidth / 2.0;
		}

		gc->Rotate(M_PI / 4.0);
		gc->StrokeLines(resolution, points.data());

		break;
	}
	case Tool_Type::Line:
	{
		wxPoint2DDouble points[2] = { wxPoint2DDouble(-itemWidth / 2, 0), wxPoint2DDouble(itemWidth / 2, 0) };
		gc->DrawLines(2, points);

		break;
	}
	case Tool_Type::Rectangle:
	{
		gc->DrawRectangle(-itemWidth / 2, -itemWidth / 2, itemWidth, itemWidth);

		break;
	}
	case Tool_Type::Circle:
	{
		gc->DrawEllipse(-itemWidth / 2, -itemWidth / 2, itemWidth, itemWidth);

		break;
	}
	}

	gc->PopState();
}