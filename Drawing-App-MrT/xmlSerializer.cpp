#include "xmlSerializer.h"

XML_Serializer::XML_Serializer()
{
	wxFileSystem::AddHandler(new wxZipFSHandler);
}

bool XML_Serializer::Serialize(const wxString& path, const std::vector<Canvas_Object*>& paths)
{
	wxXmlDocument doc;

	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "DrawingApp");
	root->AddAttribute("XmlVersion", "1.0");
	root->AddAttribute("Author", "MrT Stephens");
	doc.SetRoot(root);

	for (auto& path : paths)
	{
		path->Serialize(root);
	}

	return this->CompressXml(path, doc);
}

bool XML_Serializer::Deserialize(const wxString& path, std::vector<Canvas_Object*>& paths)
{
	wxXmlDocument doc;

	if (!this->DecompressXml(path, doc))
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
		Tool_Type toolId = (Tool_Type)wxAtoi(node->GetAttribute(Xml_Object_Names::Id));

		switch (toolId)
		{
		case Tool_Type::Pencil:
		{
			paths.push_back(new Canvas_Squiggle());
			break;
		}
		case Tool_Type::Line:
		{
			paths.push_back(new Canvas_Line());
			break;
		}
		case Tool_Type::Rectangle:
		{
			paths.push_back(new Canvas_Rectangle());
			break;
		}
		case Tool_Type::Circle:
		{
			paths.push_back(new Canvas_Circle());
			break;
		}
		default:
			std::runtime_error("Unknown object type. Object Id: " + std::to_string(static_cast<int>(toolId)));
		}

		paths.back()->Deserialize(node);
	}

	return true;
}

bool XML_Serializer::CompressXml(const wxString& filename, const wxXmlDocument& doc)
{
	auto outStream = wxFileOutputStream(filename);
	wxZipOutputStream zipStream(outStream);

	zipStream.PutNextEntry("canvas_objects.xml");
	
	bool result = doc.Save(zipStream);

	zipStream.CloseEntry();
	zipStream.Close();
	outStream.Close();

	return result;
}

bool XML_Serializer::DecompressXml(const wxString& filename, wxXmlDocument& doc)
{
	wxFileSystem fs;
	wxFSFile* zipFile = fs.OpenFile(filename + "#zip:canvas_objects.xml");

	if (!zipFile)
	{
		return false;
	}
	else
	{
		wxInputStream* zipStream = zipFile->GetStream();

		if (!zipStream)
		{
			return false;
		}
		else
		{
			if (!doc.Load(*zipStream))
			{
				return false;
			}
		}
	}

	delete zipFile;
	return true;
}