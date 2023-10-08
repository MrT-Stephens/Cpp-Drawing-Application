#pragma once

#include <wx/xml/xml.h>

#include "squigglePath.h"

class XML_Serializer
{
public:
	XML_Serializer();

	bool Serialize(const wxString& filename, const std::vector<Squiggle_Path>& paths);
	bool Deserialize(const wxString& filename, std::vector<Squiggle_Path>& paths);
};