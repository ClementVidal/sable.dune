#ifndef _DUNE_EDITOR_VIEW_PROJECTSETTINGS_
#define _DUNE_EDITOR_VIEW_PROJECTSETTINGS_

#include <Sable\Core\Common\DataTypes.h>

#include <QtGui\QDialog>



namespace Dune
{

/**
Asset picker
*/
class CViewProjectSettings : public QDialog
{
    
    Q_OBJECT

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewProjectSettings( CViewMainFrame& view );
    virtual ~CViewProjectSettings();
    //@}

    /** @name Accesors*/
    //@{
    CApplication& GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types

    // Methods
    Void PopulateTree();
    //Void OnUISelectionChanged( wxTreeEvent& event );
   // Void OnEditElement( wxTreeEvent& event );
    Void CreateWidget();
    
    //  Attributes
   // wxWindow*           m_ProjectPanel;
   // wxTreeCtrl*         m_TreeCtrl;
    CApplication*    m_App;
   // wxTextCtrl*         m_ProjectName;
   // wxTextCtrl*         m_ProjectDesc;
};

};

#endif