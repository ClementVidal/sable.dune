#include <Dune/Editor/View/ProjectSettings.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Editor/Application/Application.h>

using namespace Dune;

CViewProjectSettings::CViewProjectSettings( CViewMainFrame& view ) :
    QDialog( &view, Qt::Tool  )
{
    setWindowTitle( "Project settings" );
    m_App = &view.GetApp();
/*
    wxBoxSizer* vSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* hSizer;

    CreateWidget();

    m_TreeCtrl = new wxTreeCtrl( this, 1, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT | wxTR_NO_BUTTONS );

    hSizer = new wxBoxSizer( wxHORIZONTAL );
    m_TreeCtrl->SetInitialSize( wxSize( 200,100 ) );
    hSizer->Add( m_TreeCtrl, 0.5f, wxALL  | wxEXPAND );
    hSizer->Add( m_ProjectPanel);
    vSizer->Add( hSizer, 1.0f, wxALIGN_CENTER | wxEXPAND);

    hSizer = new wxBoxSizer( wxHORIZONTAL );
    hSizer->Add(
    new wxButton( this, wxID_CANCEL, WT("Ok") ), 0, wxALL, 10 ); 

    hSizer->Add(
    new wxButton( this, wxID_CANCEL, WT("Cancel") ), 0, wxALL, 10 ); 
    vSizer->Add( hSizer, 0.0f, wxALIGN_RIGHT);

    PopulateTree();

    SetSizer( vSizer );
    vSizer->SetMinSize( 500, 300 );
    vSizer->SetSizeHints( this );
*/
}

CViewProjectSettings::~CViewProjectSettings()
{

}

Void CViewProjectSettings::CreateWidget()
{/*
    m_ProjectPanel = new wxPanel( this, -1 );

    wxGridSizer* sizer = new wxFlexGridSizer( 2, 10, 10 );

    sizer->Add( new wxStaticText( m_ProjectPanel, -1, WT("Name:") ) ); 
    m_ProjectName = new wxTextCtrl( m_ProjectPanel, -1, WT("Test") );
    sizer->Add( m_ProjectName, 0, wxEXPAND ); 

    sizer->Add( new wxStaticText( m_ProjectPanel, -1, WT("Description:") ) ); 
    m_ProjectDesc = new wxTextCtrl( m_ProjectPanel, -1, WT(""), wxDefaultPosition, wxSize( 300, 100 ), wxTE_MULTILINE );
    sizer->Add( m_ProjectDesc, 0, wxEXPAND ); 

    m_ProjectPanel->SetSizer( sizer );*/
}
/*
Void CViewProjectSettings::OnEditElement( wxTreeEvent& event )
{

}

Void CViewProjectSettings::OnUISelectionChanged( wxTreeEvent& event )
{
}
*/
Void CViewProjectSettings::PopulateTree()
{/*
    wxTreeItemId idRoot, id;

    idRoot = m_TreeCtrl->AddRoot( WT("Root") );

    m_TreeCtrl->AppendItem( idRoot, WT("Project") );

    id = m_TreeCtrl->AppendItem( idRoot, WT("File System") );
    m_TreeCtrl->AppendItem( id, WT("Mapping") );

    id = m_TreeCtrl->AppendItem( idRoot, WT("Platform") );


    m_TreeCtrl->ExpandAll();*/
}

CApplication& CViewProjectSettings::GetApp()
{
    return *m_App;
}