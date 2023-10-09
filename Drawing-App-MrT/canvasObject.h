#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/xml/xml.h>

#include "toolSelectionPane.h"

namespace Xml_Object_Names
{
	// Object Names
	constexpr const char* const Squiggle = "Squiggle";
	constexpr const char* const Circle = "Circle";
	constexpr const char* const Rectangle = "Rectangle";
	constexpr const char* const Line = "Line";

	// Child Names
	constexpr const char* const Colour = "Colour";
	constexpr const char* const Center = "Center";
	constexpr const char* const Radius = "Radius";
	constexpr const char* const Origin = "Origin";
	constexpr const char* const Start = "Start";
	constexpr const char* const End = "End";
	constexpr const char* const Height = "Height";
	constexpr const char* const Width = "Width";
	constexpr const char* const Location = "Location";
	constexpr const char* const LeftTop = "LeftTop";
	constexpr const char* const BottomRight = "BottomRight";
	constexpr const char* const Coordinate = "Coordinate";

	// Attribute Names
	constexpr const char* const X = "X";
	constexpr const char* const Y = "Y";
	constexpr const char* const R = "R";
	constexpr const char* const G = "G";
	constexpr const char* const B = "B";
	constexpr const char* const Id = "Id";
	constexpr const char* const Value = "Value";
}

class Canvas_Object
{
private:
	Tool_Type m_ToolId;

protected:
	wxColour m_Colour;

public:
	Canvas_Object() = default;
	Canvas_Object(const wxColour& colour, Tool_Type toolId) : m_Colour(colour), m_ToolId(toolId) { };

	virtual ~Canvas_Object() noexcept { };

	virtual void Draw(wxGraphicsContext* gc) const = 0;
	virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) = 0;

	virtual void Serialize(wxXmlNode* node) = 0;
	virtual void Deserialize(const wxXmlNode* const node) = 0;

	const wxColour& GetColour() const { return m_Colour; }
	Tool_Type GetToolId() const { return m_ToolId; }
};

class Canvas_Squiggle : public Canvas_Object
{
private:
	int m_Width;
	std::vector<wxPoint2DDouble> m_Points;

public:
	Canvas_Squiggle() = default;
	Canvas_Squiggle(int width, const wxColour& colour);

	~Canvas_Squiggle() noexcept = default;

	virtual void Draw(wxGraphicsContext* gc) const override;
	virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) override;

	virtual void Serialize(wxXmlNode* node) override;
	virtual void Deserialize(const wxXmlNode* const node) override;
};

class Canvas_Circle : public Canvas_Object
{
private:
	double m_Radius;
	wxPoint2DDouble m_Center;

public:
	Canvas_Circle() = default;
	Canvas_Circle(const wxPoint2DDouble& center, const wxColour& colour);

	~Canvas_Circle() noexcept = default;

	virtual void Draw(wxGraphicsContext* gc) const override;
	virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) override;

	virtual void Serialize(wxXmlNode* node) override;
	virtual void Deserialize(const wxXmlNode* const node) override;
};

class Canvas_Rectangle : public Canvas_Object
{
private:
	wxRect2DDouble m_Rect;

public:
	Canvas_Rectangle() = default;
	Canvas_Rectangle(const wxPoint2DDouble& origin, const wxColour& colour);

	~Canvas_Rectangle() noexcept = default;

	virtual void Draw(wxGraphicsContext* gc) const override;
	virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) override;

	virtual void Serialize(wxXmlNode* node) override;
	virtual void Deserialize(const wxXmlNode* const node) override;
};

class Canvas_Line : public Canvas_Object
{
private:
	wxPoint2DDouble m_Start;
	wxPoint2DDouble m_End;
	int m_Width;

public:
	Canvas_Line() = default;
	Canvas_Line(const wxPoint2DDouble& start, const wxPoint2DDouble& end, int width, const wxColour& colour);

	~Canvas_Line() noexcept = default;

	virtual void Draw(wxGraphicsContext* gc) const override;
	virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) override;

	virtual void Serialize(wxXmlNode* node) override;
	virtual void Deserialize(const wxXmlNode* const node) override;
};