#pragma once

#include <wx/wx.h>

#include <vector>

class Squiggle_Path
{
private:
	int m_Width;
	wxColour m_Colour;
	std::vector<wxPoint2DDouble> m_Points;

public:
	Squiggle_Path(int width, wxColour colour) 
		: m_Width(width), m_Colour(colour) { }

	void AddPoint(wxPoint2DDouble point) { m_Points.push_back(point); }

	int GetWidth() const { return m_Width; }

	wxColour GetColour() const { return m_Colour; }

	size_t GetPointCount() const { return m_Points.size(); }

	const std::vector<wxPoint2DDouble>& GetPoints() const { return m_Points; }
};