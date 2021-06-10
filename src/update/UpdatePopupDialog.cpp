/*!********************************************************************
 Audacity: A Digital Audio Editor

 @file UpdatePopupDialog.cpp
 @brief Define a dialog for notifying users about new version available.

 Anton Gerasimov
 **********************************************************************/

#include "update/UpdatePopupDialog.h"

#include "ShuttleGui.h"
#include "widgets/HelpSystem.h"

#include <wx/debug.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>

enum { DontShowID = wxID_HIGHEST + 1 };

BEGIN_EVENT_TABLE (UpdatePopupDialog, wxDialogWrapper)
    EVT_BUTTON (wxID_YES, UpdatePopupDialog::OnUpdate)
    EVT_BUTTON (wxID_NO, UpdatePopupDialog::OnSkip)
    EVT_CHECKBOX (DontShowID, UpdatePopupDialog::OnDontShow)
END_EVENT_TABLE()

IMPLEMENT_CLASS (UpdatePopupDialog, wxDialogWrapper)

UpdatePopupDialog::UpdatePopupDialog (wxWindow* parent, UpdateManager* updateManager)
    : wxDialogWrapper (parent, -1, XO ("Update Audacity"),
        wxDefaultPosition, wxDefaultSize,
        wxCAPTION),
      mUpdateManager (updateManager)
{
    wxASSERT(mUpdateManager);

    ShuttleGui S (this, eIsCreating);
    S.SetBorder (5);
    S.StartVerticalLay (wxEXPAND, 1);
    {
        S.AddWindow (AddHtmlContent (S.GetParent()));

        S.StartHorizontalLay (wxEXPAND, 0);
        {
            S.SetBorder (5);

            S.Id (DontShowID).AddCheckBox (
                XO ("Don't show this again at start up"), !mUpdateManager->isTrackingEnabled());

            S.Prop(1).AddSpace(1, 0, 1);

            S.Id (wxID_NO).AddButton (XO ("Skip"));
            S.Id (wxID_YES).AddButton (XO ("Install update"));

            S.SetBorder (5);
        }
        S.EndHorizontalLay();
    }
    S.EndVerticalLay();

    Layout();
    Fit();
    Center();
}

UpdatePopupDialog::~UpdatePopupDialog()
{
    ;
}

void UpdatePopupDialog::OnUpdate (wxCommandEvent&)
{
    EndModal (wxID_YES);
}

void UpdatePopupDialog::OnSkip (wxCommandEvent&)
{
    EndModal (wxID_NO);
}

void UpdatePopupDialog::OnDontShow (wxCommandEvent& event)
{
    mUpdateManager->enableTracking(!event.IsChecked());
}

HtmlWindow* UpdatePopupDialog::AddHtmlContent (wxWindow* parent)
{
    wxStringOutputStream o;
    wxTextOutputStream informationStr (o);

    static const auto title = XO("Audacity %s is available!")
        .Format(mUpdateManager->getVersionPatch().version.getString());

    informationStr
        << wxT("<html><body><h3>")
        << title.Translation()
        << wxT("</h3><h5>")
        << XO("Changelog")
        << wxT("</h5><p>");

    informationStr << wxT("<ul>");
    for (auto& logLine : mUpdateManager->getVersionPatch().changelog)
    {
        informationStr << wxT("<li>");
        // We won't to translate downloaded text.
        informationStr << logLine;
        informationStr << wxT("</li>");
    }
    informationStr << wxT("</ul></p>");

    informationStr << wxT("<p>");
    informationStr << XO("<a href = \"https://github.com/audacity/audacity/releases\">Read more on GitHub</a>");
    informationStr << wxT("</p>");

    informationStr << wxT("</body></html>");

    HtmlWindow* html = safenew LinkingHtmlWindow (parent, -1,
        wxDefaultPosition,
        wxSize (500, -1),
        wxHW_SCROLLBAR_AUTO | wxSUNKEN_BORDER);

    html->SetBorders (20);
    html->SetPage (o.GetString());

    wxHtmlContainerCell* cell = html->GetInternalRepresentation();

    cell->Layout (500);

    const wxSize size = wxSize (500, cell->GetHeight());

    html->SetMinSize (size);
    html->SetMaxSize (size);
    html->SetSize (size);

    return html;
}
