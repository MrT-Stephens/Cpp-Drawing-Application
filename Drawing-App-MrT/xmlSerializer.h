#pragma once

#include <wx/xml/xml.h>

#include "canvasObject.h"
#include "toolSelectionPane.h"

#include <exception>

class XML_Serializer
{
public:
	XML_Serializer();

	bool Serialize(const wxString& filename, const std::vector<Canvas_Object*>& paths);
	bool Deserialize(const wxString& filename, std::vector<Canvas_Object*>& paths);
};