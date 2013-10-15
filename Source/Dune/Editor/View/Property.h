#ifndef _DUNE_EDITOR_VIEW_PROPERTY_
#define _DUNE_EDITOR_VIEW_PROPERTY_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\ManagedObject\Ref.h>

#include <Dune\Editor\Ctrl\EventClient.h>
#include <Dune\Editor\Utilities\IconTable.h>

#include <QtGui\QDockWidget>
#include <QtGui\QScrollArea>
#include <QtGui\QToolBar>
#include <QtGui\QFrame>
#include <QtGui\QVBoxLayout>


namespace Dune
{

class CBankObject;
class CViewPropertyGrid;
class CCtrlProject;
class CViewMainFrame;

/**
Container window for property window
*/
class CViewProperty : public QDockWidget, public CEventClient
{

	Q_OBJECT 

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CViewProperty( CViewMainFrame& mainFrame );
    virtual ~CViewProperty();
    //@}

    /** @name Accessors*/
    //@{
	CViewPropertyGrid&	GetPropertyGrid();
	//@}

    /** @name Manipulator*/
    //@{
	Void			OnUIPropertyChange(  CPropertyInterface& p, String oldValue, String newValue );
    CBankObject*	GetPropertyObject( );
    //@}

	/** @name Signal*/
	//@{
	//@}

private:

    // Types
 
    // Methods
	Void	OnProjectDestroyed();
	Void	OnObjectSelected( const CHandle& current );
	Void	OnObjectDeSelected( const CHandle& current );
	Void	OnRemoveObject( const CHandle& obj );

    Q_SLOT Bool    OnUIShowDependency();

	Void	SetPropertyObject( CBankObject* obj );

    // Attributes
    CViewPropertyGrid*		m_PropertyGrid;
	CIconTable				m_IconTable;
	QFrame*					m_Frame;
	QToolBar*				m_ToolBar;
	QScrollArea*			m_ScrollArea;
	QVBoxLayout*			m_Layout;

};

};

#endif