#ifndef _DUNE_EDITOR_VIEW_PROPERTYGRID_
#define _DUNE_EDITOR_VIEW_PROPERTYGRID_

#include <Sable/Core/Common/DataTypes.h>
#include <Dune/Editor/Ctrl/EventClient.h>

#include <QtGui/QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>



namespace Dune
{

class CBankObject;
class CPropertyInterface;
class CApplication;
class CCtrlProject;
class CViewPropertyHandle;

/**
*/
class CViewPropertyGrid : public QTreeWidget
{

public:

    // Data Types

    /** @name Constructor/Destructor*/
    //@{
    CViewPropertyGrid( CViewProperty& view );
    virtual ~CViewPropertyGrid();
    //@}

    /** @name Accessors*/
    //@{
    CApplication&	GetApp();
    Void				SetPropertyObject( CBankObject* obj );
    CBankObject*		GetPropertyObject() const;    
    //@}

    /** @name Manipulator*/
    //@{
	//@}

	//BUG Sale hack, voir CViewBank::OnUISelectionChanged
	Bool	CanChangeSelection;

private:

    // Types
 
    // Methods
	Void	AddProperty( const CPropertyInterface& p );
	Void	OnObjectPropertyAdded( const CPropertyInterface& p );
	Void	OnObjectPropertyRemoved( const CPropertyInterface& p );
	Void	FillProperties();

    // Attributes
	Sb::CEventSlot< CViewPropertyGrid, const CPropertyInterface& >	m_SlotObjectPropertyAdded;
	Sb::CEventSlot< CViewPropertyGrid, const CPropertyInterface& >	m_SlotObjectPropertyRemoved;
    CBankObject*			m_PropertyObject;
    CApplication*       m_App;
    CViewPropertyHandle*    m_EditedAsset;
	CViewProperty*			m_ViewProperty;
	Sb::CMap<Sb::CString, QTreeWidgetItem*> m_GroupTable;

};

};

#endif