#include "canvasObject.h"

Canvas_Squiggle::Canvas_Squiggle(int width, const wxColour& colour)
	: Canvas_Object(colour, Tool_Type::Pencil), m_Width(width)
{
}

void Canvas_Squiggle::Draw(wxGraphicsContext* gc) const
{
	if (m_Points.size() > 1)
	{
		gc->SetPen(wxPen(m_Colour, m_Width));
		gc->StrokeLines(m_Points.size(), m_Points.data());
	}
}

void Canvas_Squiggle::HandleCreationByMouseDrag(wxPoint currentDragPoint)
{
	m_Points.push_back(wxPoint2DDouble(currentDragPoint.x, currentDragPoint.y));
}

void Canvas_Squiggle::Serialize(wxXmlNode* node)
{
	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Squiggle);
	root->AddAttribute(Xml_Object_Names::Id, std::to_string((int)this->GetToolId()));

	{
		wxXmlNode* colour = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Colour);
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::R, std::to_string(m_Colour.GetRed())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::G, std::to_string(m_Colour.GetGreen())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::B, std::to_string(m_Colour.GetBlue())));

		root->AddChild(colour);
	}

	{
		wxXmlNode* width = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Width);
		width->AddAttribute(Xml_Object_Names::Value, std::to_string(m_Width));

		root->AddChild(width);
	}

	{
		wxXmlNode* points = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Location);

		for (auto& [x, y] : m_Points)
		{
			wxXmlNode* coordinate = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Coordinate);
			coordinate->AddAttribute(Xml_Object_Names::X, std::to_string(x));
			coordinate->AddAttribute(Xml_Object_Names::Y, std::to_string(y));

			points->AddChild(coordinate);
		}

		root->AddChild(points);
	}

	node->AddChild(root);
}

void Canvas_Squiggle::Deserialize(const wxXmlNode* const node)
{
	wxXmlNode* colour = node->GetChildren();
	m_Colour = wxColour(
		wxAtoi(colour->GetAttribute(Xml_Object_Names::R)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::G)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::B)));

	wxXmlNode* width = colour->GetNext();
	m_Width = wxAtoi(width->GetAttribute(Xml_Object_Names::Value));

	wxXmlNode* points = width->GetNext();
	for (wxXmlNode* coordinate = points->GetChildren(); coordinate != nullptr; coordinate = coordinate->GetNext())
	{
		m_Points.push_back(wxPoint2DDouble(
			wxAtof(coordinate->GetAttribute(Xml_Object_Names::X)),
			wxAtof(coordinate->GetAttribute(Xml_Object_Names::Y))));
	}
}

Canvas_Circle::Canvas_Circle(const wxPoint2DDouble& center, const wxColour& colour)
	: Canvas_Object(colour, Tool_Type::Circle), m_Center(center), m_Radius(0)
{
}

void Canvas_Circle::Draw(wxGraphicsContext* gc) const
{
	gc->SetPen(wxPen(m_Colour));
	gc->SetBrush(wxBrush(m_Colour));
	gc->DrawEllipse(m_Center.m_x - m_Radius, m_Center.m_y - m_Radius, m_Radius * 2, m_Radius * 2);
}

void Canvas_Circle::HandleCreationByMouseDrag(wxPoint currentDragPoint)
{
	m_Radius = std::sqrt(std::pow(currentDragPoint.x - m_Center.m_x, 2) + std::pow(currentDragPoint.y - m_Center.m_y, 2));
}

void Canvas_Circle::Serialize(wxXmlNode* node)
{
	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Circle);
	root->AddAttribute(Xml_Object_Names::Id, std::to_string((int)this->GetToolId()));

	{
		wxXmlNode* colour = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Colour);
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::R, std::to_string(m_Colour.GetRed())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::G, std::to_string(m_Colour.GetGreen())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::B, std::to_string(m_Colour.GetBlue())));

		root->AddChild(colour);
	}

	{
		wxXmlNode* radius = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Radius);
		radius->AddAttribute(Xml_Object_Names::Value, std::to_string(m_Radius));

		root->AddChild(radius);
	}

	{
		wxXmlNode* center = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Center);
		center->AddAttribute(Xml_Object_Names::X, std::to_string(m_Center.m_x));
		center->AddAttribute(Xml_Object_Names::Y, std::to_string(m_Center.m_y));

		root->AddChild(center);
	}

	node->AddChild(root);
}

void Canvas_Circle::Deserialize(const wxXmlNode* const node)
{
	wxXmlNode* colour = node->GetChildren();
	m_Colour = wxColour(
		wxAtoi(colour->GetAttribute(Xml_Object_Names::R)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::G)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::B)));

	wxXmlNode* radius = colour->GetNext();
	m_Radius = wxAtof(radius->GetAttribute(Xml_Object_Names::Value));

	wxXmlNode* center = radius->GetNext();
	m_Center = wxPoint2DDouble(
		wxAtof(center->GetAttribute(Xml_Object_Names::X)),
		wxAtof(center->GetAttribute(Xml_Object_Names::Y)));
}

Canvas_Rectangle::Canvas_Rectangle(const wxPoint2DDouble& origin, const wxColour& colour)
	: Canvas_Object(colour, Tool_Type::Rectangle), m_Rect(origin.m_x, origin.m_y, 0, 0)
{
}

void Canvas_Rectangle::Draw(wxGraphicsContext* gc) const
{
	gc->SetPen(wxPen(m_Colour));
	gc->SetBrush(wxBrush(m_Colour));
	gc->DrawRectangle(m_Rect.m_x, m_Rect.m_y, m_Rect.m_width, m_Rect.m_height);
}

void Canvas_Rectangle::HandleCreationByMouseDrag(wxPoint currentDragPoint)
{
	m_Rect.SetRightBottom(currentDragPoint);
}

void Canvas_Rectangle::Serialize(wxXmlNode* node)
{
	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Rectangle);
	root->AddAttribute(Xml_Object_Names::Id, std::to_string((int)this->GetToolId()));

	{
		wxXmlNode* colour = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Colour);
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::R, std::to_string(m_Colour.GetRed())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::G, std::to_string(m_Colour.GetGreen())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::B, std::to_string(m_Colour.GetBlue())));

		root->AddChild(colour);
	}

	{
		wxXmlNode* rect = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Location);

		{
			wxXmlNode* coordinate = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::LeftTop);
			coordinate->AddAttribute(Xml_Object_Names::X, std::to_string(m_Rect.GetLeftTop().m_x));
			coordinate->AddAttribute(Xml_Object_Names::Y, std::to_string(m_Rect.GetLeftTop().m_y));

			rect->AddChild(coordinate);
		}

		{
			wxXmlNode* coordinate = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::BottomRight);
			coordinate->AddAttribute(Xml_Object_Names::X, std::to_string(m_Rect.GetRightBottom().m_x));
			coordinate->AddAttribute(Xml_Object_Names::Y, std::to_string(m_Rect.GetRightBottom().m_y));

			rect->AddChild(coordinate);
		}

		root->AddChild(rect);
	}

	node->AddChild(root);
}

void Canvas_Rectangle::Deserialize(const wxXmlNode* const node)
{
	wxXmlNode* colour = node->GetChildren();
	m_Colour = wxColour(
		wxAtoi(colour->GetAttribute(Xml_Object_Names::R)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::G)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::B)));

	wxXmlNode* rect = colour->GetNext();
	wxXmlNode* leftTop = rect->GetChildren();
	wxXmlNode* bottomRight = leftTop->GetNext();

	m_Rect = wxRect2DDouble();
	m_Rect.SetLeftTop({
		wxAtof(leftTop->GetAttribute(Xml_Object_Names::X)),
		wxAtof(leftTop->GetAttribute(Xml_Object_Names::Y)) });

	m_Rect.SetRightBottom({
		wxAtof(bottomRight->GetAttribute(Xml_Object_Names::X)),
		wxAtof(bottomRight->GetAttribute(Xml_Object_Names::Y)) });
}

Canvas_Line::Canvas_Line(const wxPoint2DDouble& start, const wxPoint2DDouble& end, int width, const wxColour& colour)
	: Canvas_Object(colour, Tool_Type::Line), m_Start(start), m_End(end), m_Width(width)
{
}

void Canvas_Line::Draw(wxGraphicsContext* gc) const
{
	gc->SetPen(wxPen(m_Colour, m_Width));
	wxPoint2DDouble points[2] = { m_Start, m_End };
	gc->DrawLines(2, points);
}

void Canvas_Line::HandleCreationByMouseDrag(wxPoint currentDragPoint)
{
	m_End = currentDragPoint;
}

void Canvas_Line::Serialize(wxXmlNode* node)
{
	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Rectangle);
	root->AddAttribute(Xml_Object_Names::Id, std::to_string((int)this->GetToolId()));

	{
		wxXmlNode* colour = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Colour);
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::R, std::to_string(m_Colour.GetRed())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::G, std::to_string(m_Colour.GetGreen())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::B, std::to_string(m_Colour.GetBlue())));

		root->AddChild(colour);
	}

	{
		wxXmlNode* width = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Width);
		width->AddAttribute(Xml_Object_Names::Value, std::to_string(m_Width));

		root->AddChild(width);
	}

	{
		wxXmlNode* location = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Location);

		{
			wxXmlNode* start = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Start);
			start->AddAttribute(Xml_Object_Names::X, std::to_string(m_Start.m_x));
			start->AddAttribute(Xml_Object_Names::Y, std::to_string(m_Start.m_y));

			location->AddChild(start);
		}

		{
			wxXmlNode* start = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::End);
			start->AddAttribute(Xml_Object_Names::X, std::to_string(m_End.m_x));
			start->AddAttribute(Xml_Object_Names::Y, std::to_string(m_End.m_y));

			location->AddChild(start);
		}

		root->AddChild(location);
	}

	node->AddChild(root);
}

void Canvas_Line::Deserialize(const wxXmlNode* const node)
{
	wxXmlNode* colour = node->GetChildren();
	m_Colour = wxColour(
		wxAtoi(colour->GetAttribute(Xml_Object_Names::R)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::G)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::B)));

	wxXmlNode* width = colour->GetNext();
	m_Width = wxAtoi(width->GetAttribute(Xml_Object_Names::Value));

	wxXmlNode* location = width->GetNext();
	wxXmlNode* start = location->GetChildren();
	wxXmlNode* end = start->GetNext();

	m_Start = wxPoint2DDouble(
		wxAtof(start->GetAttribute(Xml_Object_Names::X)),
		wxAtof(start->GetAttribute(Xml_Object_Names::Y)));

	m_End = wxPoint2DDouble(
		wxAtof(end->GetAttribute(Xml_Object_Names::X)),
		wxAtof(end->GetAttribute(Xml_Object_Names::Y)));
}

Canvas_Rounded_Rectangle::Canvas_Rounded_Rectangle(const wxPoint2DDouble& origin, const wxColour& colour)
	: Canvas_Object(colour, Tool_Type::Rounded_Rectangle), m_Rect(origin.m_x, origin.m_y, 0, 0)
{
}

void Canvas_Rounded_Rectangle::Draw(wxGraphicsContext* gc) const
{
	gc->SetPen(wxPen(m_Colour));
	gc->SetBrush(wxBrush(m_Colour));
	gc->DrawRoundedRectangle(m_Rect.m_x, m_Rect.m_y, m_Rect.m_width, m_Rect.m_height, m_Rect.m_width / 4);
}

void Canvas_Rounded_Rectangle::HandleCreationByMouseDrag(wxPoint currentDragPoint)
{
	m_Rect.SetRightBottom(currentDragPoint);
}

void Canvas_Rounded_Rectangle::Serialize(wxXmlNode* node)
{
	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Rectangle);
	root->AddAttribute(Xml_Object_Names::Id, std::to_string((int)this->GetToolId()));

	{
		wxXmlNode* colour = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Colour);
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::R, std::to_string(m_Colour.GetRed())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::G, std::to_string(m_Colour.GetGreen())));
		colour->AddAttribute(new wxXmlAttribute(Xml_Object_Names::B, std::to_string(m_Colour.GetBlue())));

		root->AddChild(colour);
	}

	{
		wxXmlNode* rect = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::Location);

		{
			wxXmlNode* coordinate = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::LeftTop);
			coordinate->AddAttribute(Xml_Object_Names::X, std::to_string(m_Rect.GetLeftTop().m_x));
			coordinate->AddAttribute(Xml_Object_Names::Y, std::to_string(m_Rect.GetLeftTop().m_y));

			rect->AddChild(coordinate);
		}

		{
			wxXmlNode* coordinate = new wxXmlNode(wxXML_ELEMENT_NODE, Xml_Object_Names::BottomRight);
			coordinate->AddAttribute(Xml_Object_Names::X, std::to_string(m_Rect.GetRightBottom().m_x));
			coordinate->AddAttribute(Xml_Object_Names::Y, std::to_string(m_Rect.GetRightBottom().m_y));

			rect->AddChild(coordinate);
		}

		root->AddChild(rect);
	}

	node->AddChild(root);
}

void Canvas_Rounded_Rectangle::Deserialize(const wxXmlNode* const node)
{
	wxXmlNode* colour = node->GetChildren();
	m_Colour = wxColour(
		wxAtoi(colour->GetAttribute(Xml_Object_Names::R)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::G)),
		wxAtoi(colour->GetAttribute(Xml_Object_Names::B)));

	wxXmlNode* rect = colour->GetNext();
	wxXmlNode* leftTop = rect->GetChildren();
	wxXmlNode* bottomRight = leftTop->GetNext();

	m_Rect = wxRect2DDouble();
	m_Rect.SetLeftTop({
		wxAtof(leftTop->GetAttribute(Xml_Object_Names::X)),
		wxAtof(leftTop->GetAttribute(Xml_Object_Names::Y)) });

	m_Rect.SetRightBottom({
		wxAtof(bottomRight->GetAttribute(Xml_Object_Names::X)),
		wxAtof(bottomRight->GetAttribute(Xml_Object_Names::Y)) });
}