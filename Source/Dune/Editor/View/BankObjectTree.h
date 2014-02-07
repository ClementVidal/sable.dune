#ifndef _DUNE_EDITOR_VIEW_BANKOBJECTTREE_
#define _DUNE_EDITOR_VIEW_BANKOBJECTTREE_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Ctrl/EventClient.h>
#include <Dune/Editor/Utilities/IconTable.h>
#include <Dune/Editor/View/FBXLoader.h>

#include <QtGui/QTreeWidget>
#include <QtCore/QMimeData>



namespace Dune
{

class CApplication;
class CPropertyInterface;
class CProject;
class CHandle;

/**
UI representation of a project
*/
class CViewBankObjectTree : public QTreeWidget, public CEventClient
{

	Q_OBJECT
    
public:

	/** @name Constructor/Destructor*/
    //@{
    CViewBankObjectTree( QWidget& parent, CApplication& app );
    virtual ~CViewBankObjectTree();
    //@}

    /** @name Accesors*/
    //@{
    virtual Bool HasIcon( const Sb::CTypeInfo& type ) const;
    virtual QIcon GetIcon( const Sb::CTypeInfo& type ) const;
    virtual Bool CanDropOn( const CHandle& h ) const;
    CCtrlProject& GetProjectCtrl();
	//@}

    /** @name Manipulator*/
    //@{
    //@}

protected:

    // Types
	struct CTreeWidgetItem : public QTreeWidgetItem
	{
		CTreeWidgetItem( );
		CTreeWidgetItem( const CHandle& elem );
		CHandle BankObject;
	};

	struct CDragAndDropMime : public QMimeData
	{
	public:
		QList<QTreeWidgetItem*>	Items;
	};

protected:

    // Methods
	Void			OnProjectCreated( CProject& p );
	Void			OnProjectLoaded( CProject& p );
	Void			OnProjectDestroyed( );
	Void            OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
	Void			OnObjectSelected( const CHandle& current );
	Void			OnObjectDeSelected( const CHandle& current );
	Void			OnAddObject( const CHandle& parent, const CHandle& h );
	Void			OnRemoveObject( const CHandle& h );
	Void			OnChildIndexChanged( const CHandle& h, Int32 oldIndex, Int32 newIndex );
	Void			Populate( CProject& p );

	virtual CHandle         GetRootObject( CProject& p ) = 0;

    // Attributes
	
private :

    Q_SLOT Bool         OnUILeftMouseButtonDClick( QTreeWidgetItem * item, int column );
    Q_SLOT Bool         OnUISelectionChanged( );

	void				mousePressEvent( QMouseEvent * event );
	void				dropEvent(QDropEvent *de);
	void				dragMoveEvent(QDragMoveEvent *de);
	void				dragEnterEvent(QDragEnterEvent *event);
	void				MakeDrag();

	CTreeWidgetItem*	AppendItem( QTreeWidgetItem& parentId, const CBankObject& b, Int32 index = -1 );
    Void                RemoveItem( const CHandle& h );
    CTreeWidgetItem*    RecursiveGetTreeItem( CTreeWidgetItem* root, const CHandle& asset );
    CTreeWidgetItem*	GetTreeItem( const CHandle& asset );
	
    // Attributes   
	CTreeWidgetItem*	m_DraggedItem;
	CTreeWidgetItem*	m_LastSelectedItem;
	Bool				m_CanProcessSelChangedEvent;
	CViewProject*		m_ViewProject;
};

};

#endif