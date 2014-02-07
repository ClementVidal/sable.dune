#ifndef _DUNE_EDITOR_APPLICATION_EVENTCLIENT_
#define _DUNE_EDITOR_APPLICATION_EVENTCLIENT_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Event/Header.h>
#include <Sable/Core/String/String.h>



namespace Dune
{

class CApplication;
class CPropertyInterface;
class CHandle;
class CProject;
class CCommandProcessor;
class CCmd;
class CPropertyInterface;
class CUuid;

/** 
Base controller class
*/
class CEventClient
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CEventClient( CApplication& app );
    virtual ~CEventClient();
    //@}

    /** @name Accessor*/
    //@{
	CCommandProcessor&	    GetCommandProcessor();
	CApplication&			GetApp( ) const;
	CProject*			    GetProject() const;
	//@}

    /** @name Manipulator*/
    //@{
	virtual Void OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue );
	virtual Void OnProjectCreated( CProject& r );
	virtual Void OnProjectLoaded( CProject& r );
    virtual Void OnProjectDestroyed();
	virtual Void OnAddObject( const CHandle& parent, const CHandle& h );
	virtual Void OnRemoveObject( const CHandle& h );
	virtual Void OnObjectSelected( const CHandle& current );
	virtual Void OnObjectDeSelected( const CHandle& current );
	virtual Void OnCurrentBankChanged( const CUuid& uuid );
	virtual Void OnChildIndexChanged( const CHandle& parent, Int32 oldIndex, Int32 newIndex );
    //@}

private:

    // Methods
	Void OnPropertyChangeInternal( const CPropertyInterface& prop, const Sb::CString& oldValue );
	Void OnProjectCreatedInternal( CProject& r );
	Void OnProjectLoadedInternal( CProject& r );
	Void OnProjectDestroyedInternal( );
	Void OnAddObjectInternal( const CHandle& parent, const CHandle& h );
	Void OnRemoveObjectInternal( const CHandle& h );
	Void OnObjectSelectedInternal( const CHandle& current );
	Void OnObjectDeSelectedInternal( const CHandle& current );
    Void OnCurrentBankChangedInternal( const CUuid& uuid );

    // Attributes
	CApplication*																	m_App;
	Sb::CEventSlot< CEventClient >													m_SlotProjectDestroyed;
	Sb::CEventSlot< CEventClient, CProject& >										m_SlotProjectCreated;
	Sb::CEventSlot< CEventClient, CProject& >										m_SlotProjectLoaded;
	Sb::CEventSlot< CEventClient, const CHandle& >									m_SlotRemoveObject;
	Sb::CEventSlot< CEventClient, const CHandle&, const CHandle& >					m_SlotAddObject;
	Sb::CEventSlot< CEventClient, const CPropertyInterface&, const Sb::CString& >	m_SlotPropertyChange;
	Sb::CEventSlot< CEventClient, const CHandle& >									m_SlotObjectSelected;
	Sb::CEventSlot< CEventClient, const CHandle& >									m_SlotObjectDeSelected;
	Sb::CEventSlot< CEventClient, const CUuid& >								    m_SlotCurrentBankChanged;
	Sb::CEventSlot< CEventClient, const CHandle& >									m_SlotObjectDependencyAdded;
	Sb::CEventSlot< CEventClient, const CHandle& >									m_SlotObjectDependencyRemoved;
	Sb::CEventSlot< CEventClient, const CHandle&, Int32, Int32 >					m_SlotChildIndexChanged;

};

}

#endif
