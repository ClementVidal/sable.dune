#include <Dune\Editor\View\Property.h> 

#include <Dune\Editor\View\MainFrame.h>
#include <Dune\Editor\Ctrl\Cmd\Header.h>
#include <Dune\Editor\Ctrl\CommandProcessor.h>
#include <Dune\Editor\View\PropertyGrid.h> 
#include <Dune\Editor\Widget\ObjectPicker.h>
#include <Dune\Editor\Widget\DependencyViewer.h> 

#include <Dune\Resources\Icons\PropertyView.xpm>

using namespace Dune;

CViewProperty::CViewProperty( CViewMainFrame& mainFrame ) :
    QDockWidget( "Property Inspector", &mainFrame ),
	CEventClient( mainFrame.GetApp() ),
	m_IconTable( (const char**)PropertyView_xpm )
{
	m_Layout = new QVBoxLayout( this );
	m_Frame = new QFrame( this );
	m_ToolBar = new QToolBar( m_Frame  );
	m_ToolBar->setIconSize( QSize( 16, 16 ) );
	m_PropertyGrid = new CViewPropertyGrid( *this );

	QAction* action = NULL;
	action = m_ToolBar->addAction( m_IconTable.GetIcon( 0 ), "Show dependency" );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIShowDependency() ) );

	m_Layout->addWidget( m_ToolBar );
	m_Layout->addWidget( m_PropertyGrid );
	m_Layout->setContentsMargins( 0, 0, 0, 0 );
	m_Frame->setLayout( m_Layout );
	setWidget( m_Frame );

	setMinimumWidth( 250 );
}

CViewProperty::~CViewProperty()
{

}

Void CViewProperty::OnUIPropertyChange(  CPropertyInterface& p, String oldValue, String newValue )
{
	if( Sb::StringCompare( newValue, oldValue ) != 0 )
	{
        CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(), p, Sb::CString( newValue ) ) );
		GetCommandProcessor().Submit( *cmd );
	}
}

Void CViewProperty::OnProjectDestroyed()
{
	SetPropertyObject( NULL );
}

Bool CViewProperty::OnUIShowDependency()
{

    if( GetPropertyObject() )
    {
        CBankObject& obj = * GetPropertyObject();

        CDependencyViewer depDialog( GetApp(), obj );
        depDialog.exec();
    }

    return TRUE;
}

CViewPropertyGrid& CViewProperty::GetPropertyGrid()
{
    return *m_PropertyGrid;
}

Void CViewProperty::OnObjectDeSelected( const CHandle& current )
{
	if( current.IsValid() )
	{
		SetPropertyObject( NULL );
	}
}

Void CViewProperty::OnObjectSelected( const CHandle& current )
{
	CBankObject* obj = GetProject()->GetObj( current );
	if( obj)
	{
		SetPropertyObject( obj );
	}
}

Void CViewProperty::OnRemoveObject( const CHandle& obj )
{
	m_PropertyGrid->SetPropertyObject( NULL );
}

Void CViewProperty::SetPropertyObject( CBankObject* obj )
{
	m_PropertyGrid->SetPropertyObject( obj );
	if( obj )
	{
		m_ToolBar->setEnabled( TRUE );
	}
	else
	{
		m_ToolBar->setEnabled( FALSE );
	}
}

CBankObject* CViewProperty::GetPropertyObject( )
{
	return m_PropertyGrid->GetPropertyObject();
}

