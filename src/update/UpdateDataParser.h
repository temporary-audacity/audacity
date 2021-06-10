/*!********************************************************************
 Audacity: A Digital Audio Editor

 @file UpdateDataParser.h
 @brief Declare a class that parse update server data format.

 Anton Gerasimov
 **********************************************************************/
#pragma once

#include "VersionPatch.h"

#include "xml/XMLTagHandler.h"

#include <wx/arrstr.h>
#include <map>

/// A class that parse update server data format.
class UpdateDataParser final : public XMLTagHandler
{
public:
    UpdateDataParser();
    ~UpdateDataParser();

    /// <summary>
    /// Parsing from update data format to VersionPatch fields.
    /// </summary>
    /// <param name="updateData">Input data.</param>
    /// <param name="versionPatch">Parsed output data.</param>
    /// <returns>True if success.</returns>
    bool Parse(const VersionPatch::UpdateDataFormat& updateData, VersionPatch* versionPatch);

private:
    enum class XmlParsedTags : int {
        kNotUsedTag,
        kUpdateTag,
        kDescriptionTag,
        kOsTag,
        kWindowsTag,
        kMacosTag,
        kLinuxTag,
        kVersionTag,
        kLinkTag
    };
    XmlParsedTags mXmlParsingState{ XmlParsedTags::kNotUsedTag };

    std::map<XmlParsedTags, wxString> mXmlTagNames{
        { XmlParsedTags::kUpdateTag, wxT("Updates") },
        { XmlParsedTags::kDescriptionTag, wxT("Description") },
        { XmlParsedTags::kOsTag, wxT("OS") },
        { XmlParsedTags::kWindowsTag, wxT("Windows") },
        { XmlParsedTags::kMacosTag, wxT("Macos") },
        { XmlParsedTags::kLinuxTag, wxT("Linux") },
        { XmlParsedTags::kVersionTag, wxT("Version") },
        { XmlParsedTags::kLinkTag, wxT("Link") },
    };

    bool HandleXMLTag(const wxChar* tag, const wxChar** attrs) override;
    void HandleXMLEndTag(const wxChar* tag) override;
    void HandleXMLContent(const wxString& content) override;
    XMLTagHandler* HandleXMLChild(const wxChar* tag) override;

    wxArrayString splitChangelogSentences(const wxString& changelogContent);

    VersionPatch* mVersionPatch{ nullptr };
};
