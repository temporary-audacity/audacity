/**********************************************************************

  Audacity: A Digital Audio Editor

  ApplicationPrefs.h

  Anton Gerasimov

**********************************************************************/

#ifndef __AUDACITY_APPLICATION_PREFS__
#define __AUDACITY_APPLICATION_PREFS__

#include <wx/defs.h>

#include "PrefsPanel.h"
#include "Prefs.h"

class ShuttleGui;

namespace ApplicationPrefsSettings {
    extern AUDACITY_DLL_API BoolSetting DefaultUpdatesCheckingFlag;
}

class ApplicationPrefs final : public PrefsPanel
{
 public:
   ApplicationPrefs(wxWindow * parent, wxWindowID winid);
   ~ApplicationPrefs();
   ComponentInterfaceSymbol GetSymbol() override;
   TranslatableString GetDescription() override;

   bool Commit() override;
   wxString HelpPageName() override;

 private:
   void Populate();
   void PopulateOrExchange(ShuttleGui & S) override;
};

#endif
