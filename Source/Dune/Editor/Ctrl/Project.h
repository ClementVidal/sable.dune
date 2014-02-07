#ifndef _DUNE_EDITOR_CTRL_PROJECT_PROJECT_
#define _DUNE_EDITOR_CTRL_PROJECT_PROJECT_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Event/Header.h>
#include <Sable/Core/Collection/Array.h>

#include <Dune/Editor/Ctrl/Ctrl.h>
#include <Dune/Model/Bank/Bank.h>

namespace Dune
{

class CFBXDoc;
class CFBXObject;
class CViewProject;
class CProject;
class CPropertyArray;

/** 
Main project class 
*/
class CCtrlProject : public CCtrl
{

public:

	/** @name DataTypes*/
	//@{
	typedef Sb::CList<CHandle> ObjectList;
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CCtrlProject( CApplication& app );
    virtual ~CCtrlProject();
    //@}

    /** @name Accessor*/
    //@{
	Bool						IsObjectSelected( const CHandle& obj ) const;
	const ObjectList&			GetSelectedObjects()const;
	CProject*					GetProject() const;
    //@}

    /** @name Manipulator*/
    //@{    
	Void            Destroy();
    CProject*       Create( const Sb::CFilePath& path );
    CProject*       Save( const Sb::CFilePath& path );
    CProject*       Load( const Sb::CFilePath& path );

	Void		    SelectObjects( const CHandle& current, Bool selectOrDeselect );
	Void		    SelectObjects( const Sb::CArray<CHandle>& current, Bool selectOrDeselect );
	Void		    SelectObjects( Bool selectOrDeselect );

	Void            RemoveObject( const CHandle& h, Bool freeHandle = TRUE );
	CHandle         AddObject( const CHandle& parent, const Sb::CTypeInfo& type );
	CHandle         AddObject( const CHandle& parent, CHandle& reservedHandle, CBankObject& obj );
	CHandle		    AddObjectFromFBX( const CHandle& parent, const Sb::CTypeInfo& type, CFBXObject& fbxObject ) ;

    Void            RemoveBank( const CUuid& h );
    Bool            SetCurrentBank( const Dune::CBank* h );
    Bool            SetCurrentBank( const CUuid& uuid );
    Dune::CBank*    GetCurrentBank() const;

	Void			OnEditElement( const CHandle& h );
	//@}

    /** @name Signals*/
    //@{
    Sb::CEventSignal< CProject& >										SignalProjectCreated;
    Sb::CEventSignal< CProject& >										SignalProjectLoaded;
    Sb::CEventSignal< >													SignalProjectDestroyed;
    Sb::CEventSignal< const CHandle& >									SignalObjectSelected;
    Sb::CEventSignal< const CHandle& >									SignalObjectDeSelected;
    Sb::CEventSignal< const CHandle&, const CHandle& >					SignalObjectAdded;
    Sb::CEventSignal< const CHandle& >									SignalObjectRemove;
	Sb::CEventSignal< const CPropertyInterface&, const Sb::CString& >	SignalObjectPropertyChanged;
	Sb::CEventSignal< const CHandle&, Int32, Int32 >                    SignalObjectChildIndexChanged;
	Sb::CEventSignal< const CHandle& >									SignalObjectChildAdded;
    Sb::CEventSignal< const CUuid& >                                    SignalCurrentBankChanged;
	//@}

private:

	// Methods
	Bool		Serialize( Sb::CPersistentArchive& ar );
    Void        RebuildParentRelation( const CHandle& parent );
    Void        OnBankAdded( CBank& b );
    Void        OnBankRemoved( CBank& b );
    Void        OnObjectAdded( CBankObject& b );
    Void        OnObjectRemoved( CBankObject& b );

    // Attributes
    Sb::CString             m_Directory;
    Sb::CString             m_FilePath;
	Sb::CRef<CProject>      m_Model;
    CUuid                   m_CurrentBank;
	ObjectList			    m_SelectedObject;

    Sb::CEventSlot< CCtrlProject, CBankObject& >  m_SlotObjectAdded;
    Sb::CEventSlot< CCtrlProject, CBankObject& >  m_SlotObjectRemoved;
    Sb::CEventSlot< CCtrlProject, CBank& >        m_SlotBankAdded;
    Sb::CEventSlot< CCtrlProject, CBank& >        m_SlotBankRemoved;

    // Method

};

}

#endif
