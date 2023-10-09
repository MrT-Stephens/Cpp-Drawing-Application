#pragma once

#include <wx/xml/xml.h>
#include <wx/fs_zip.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>

#include "canvasObject.h"

class XML_Serializer
{
public:
	XML_Serializer();

	bool Serialize(const wxString& filename, const std::vector<Canvas_Object*>& paths);
	bool Deserialize(const wxString& filename, std::vector<Canvas_Object*>& paths);

	bool CompressXml(const wxString& filename, const wxXmlDocument& doc);
	bool DecompressXml(const wxString& filename, wxXmlDocument& doc);
};