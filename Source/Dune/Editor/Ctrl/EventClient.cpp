#include <Dune\Editor\Ctrl\EventClient.h>

#include <Dune\Editor\Ctrl\CommandProcessor.h>
#include <Dune\Model\Project\Project.h>
#include <Dune\Model\Property\Interface.h>
#include <Dune\Model\Bank\Object.h>
#include <Dune\Editor\Application\Application.h>

using namespace Dune;


CEventClient::CEventClient( CApplication& app  )
{
	m_App = &app;

	// Nous sommes obligé pour le moment de faire ce sale test pour être bien sur que le ctrl de projet est bien cré AVANT l'appel a ce constructeur.
    // Ce qui n'est pas le cas quand le Ctrl de projet est instancié ...
    // y a pas mal e trucs a changer pour résoudre ce problème
    CCtrlProject* ctrlProject = m_App->GetProjectCtrlPtr();
    if( !ctrlProject )
        return;

    EVENT_CONNECT_SLOT( ctrlProject->SignalProjectCreated, m_SlotProjectCreated, &CEventClient::OnProjectCreatedInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalProjectDestroyed, m_SlotProjectDestroyed, &CEventClient::OnProjectDestroyedInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalProjectLoaded, m_SlotProjectLoaded, &CEventClient::OnProjectLoadedInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalObjectSelected, m_SlotObjectSelected, &CEventClient::OnObjectSelectedInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalObjectDeSelected, m_SlotObjectDeSelected, &CEventClient::OnObjectDeSelectedInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalObjectAdded, m_SlotAddObject, &CEventClient::OnAddObjectInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalObjectRemove, m_SlotRemoveObject, &CEventClient::OnRemoveObjectInternal );
	EVENT_CONNECT_SLOT( ctrlProject->SignalObjectPropertyChanged, m_SlotPropertyChange, &CEventClient::OnPropertyChangeInternal );
    EVENT_CONNECT_SLOT( ctrlProject->SignalCurrentBankChanged, m_SlotCurrentBankChanged, &CEventClient::OnCurrentBankChangedInternal );
	EVENT_CONNECT_SLOT( ctrlProject->SignalObjectChildIndexChanged, m_SlotChildIndexChanged, &CEventClient::OnChildIndexChanged );

}

CEventClient::~CEventClient()
{
}

Void CEventClient::OnChildIndexChanged( const CHandle& parent, Int32 oldIndex, Int32 newIndex )
{

}

CCommandProcessor&	CEventClient::GetCommandProcessor()
{
	return m_App->GetCommandProcessor();
}

CProject* CEventClient::GetProject() const
{
	return GetApp().GetProject();
}

CApplication& CEventClient::GetApp() const
{
    return *m_App;
}


/**
This method is called any time a new project is created thus resetting all component of the application
*/
Void CEventClient::OnProjectDestroyed()
{

}

Void CEventClient::OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue )
{
}

Void CEventClient::OnProjectCreated( CProject& r )
{
}

Void CEventClient::OnProjectLoaded( CProject& r )
{
}

Void CEventClient::OnRemoveObject( const CHandle& h )
{
}

Void CEventClient::OnAddObject( const CHandle& parent, const CHandle& h )
{
}


Void CEventClient::OnObjectDeSelected( const CHandle& current )
{

}

Void CEventClient::OnCurrentBankChanged( const CUuid& uuid )
{

}

Void CEventClient::OnObjectSelected( const CHandle& current )
{

}

Void CEventClient::OnCurrentBankChangedInternal( const CUuid& uuid )
{
    OnCurrentBankChanged( uuid );
}

Void CEventClient::OnObjectDeSelectedInternal( const CHandle& current )
{
	OnObjectDeSelected( current );
}

Void CEventClient::OnObjectSelectedInternal( const CHandle& current )
{
	OnObjectSelected( current );
}

Void CEventClient::OnPropertyChangeInternal( const CPropertyInterface& prop, const Sb::CString& oldValue )
{
	CHandle h = prop.GetObj()->GetHandle();
	if( h.IsValid() )
	{
		OnPropertyChanged( prop, oldValue );
	}

}

Void CEventClient::OnProjectLoadedInternal( CProject& prj )
{
	OnProjectLoaded( prj );
}

Void CEventClient::OnProjectDestroyedInternal( )
{
	OnProjectDestroyed( );
}

Void CEventClient::OnProjectCreatedInternal( CProject& r )
{

	OnProjectCreated( r );
}

Void CEventClient::OnAddObjectInternal( const CHandle& parent, const CHandle& h )
{
	if( h.IsValid() )
	{
		OnAddObject( parent, h );
	}
}

Void CEventClient::OnRemoveObjectInternal( const CHandle& h )
{
	if( h.IsValid() )
	{
		OnRemoveObject( h );
	}
}
