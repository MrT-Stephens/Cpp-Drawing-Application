#include "xmlSerializer.h"

XML_Serializer::XML_Serializer()
{
}

bool XML_Serializer::Serialize(const wxString& path, const std::vector<Squiggle_Path>& paths)
{
	wxXmlDocument doc;

	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "DrawingApp");
	root->AddAttribute("XmlVersion", "1.0");
	root->AddAttribute("Author", "MrT Stephens");
	doc.SetRoot(root);

	for (auto& path : paths)
	{
		wxXmlNode* pathNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Squiggle");
		root->AddChild(pathNode);

		wxXmlNode* colourNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Colour");
		colourNode->AddAttribute("R", std::to_string(path.GetColour().Red()));
		colourNode->AddAttribute("G", std::to_string(path.GetColour().Green()));
		colourNode->AddAttribute("B", std::to_string(path.GetColour().Blue()));
		pathNode->AddChild(colourNode);

		wxXmlNode* widthNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Width");
		widthNode->AddAttribute("Value", std::to_string(path.GetWidth()));
		pathNode->AddChild(widthNode);

		wxXmlNode* pointsNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Points");

		for (const auto& point : path.GetPoints())
		{
			wxXmlNode* pointNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Coordinate");

			pointNode->AddAttribute("X", std::to_string(point.m_x));
			pointNode->AddAttribute("Y", std::to_string(point.m_y));

			pointsNode->AddChild(pointNode);
		}

		pathNode->AddChild(pointsNode);
	}

	return doc.Save(path);
}

bool XML_Serializer::Deserialize(const wxString& path, std::vector<Squiggle_Path>& paths)
{
	wxXmlDocument doc;

	if (!doc.Load(path))
	{
		return false;
	}

	wxXmlNode* root = doc.GetRoot();

	if (root->GetName() != "DrawingApp")
	{
		return false;
	}

	for (wxXmlNode* node = root->GetChildren(); node; node = node->GetNext())
	{
		wxColour colour;
		wxXmlNode* colourNode = node->GetChildren();

		if (colourNode->GetName() == "Colour")
		{
			colour = wxColour(
				wxAtoi(colourNode->GetAttribute("R")),
				wxAtoi(colourNode->GetAttribute("G")),
				wxAtoi(colourNode->GetAttribute("B")));
		}

		int width;
		wxXmlNode* widthNode = colourNode->GetNext();

		if (widthNode->GetName() == "Width")
		{
			width = wxAtoi(widthNode->GetAttribute("Value"));
		}

		paths.emplace_back(width, colour);

		wxXmlNode* pointsNode = widthNode->GetNext();

		for (wxXmlNode* pointNode = pointsNode->GetChildren(); pointNode; pointNode = pointNode->GetNext())
		{
			paths.back().AddPoint(
				{ wxAtof(pointNode->GetAttribute("X")),
				wxAtof(pointNode->GetAttribute("Y")) });
		}
	}

	return true;
}