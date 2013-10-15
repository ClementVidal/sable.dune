#ifndef _DUNE_EDITOR_VIEW_SCENETREE_
#define _DUNE_EDITOR_VIEW_SCENETREE_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Utilities\IconTable.h>
#include <Dune\Editor\View\BankObjectTree.h>

namespace Dune
{

class CCtrlScene;
class CProject;
class CBankObjectDirectory;

/**
UI representation of a project
*/
class CViewSceneTree : public CViewBankObjectTree
{
    
	Q_OBJECT

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewSceneTree( CViewScene& viewScene, CApplication& app  );
    virtual ~CViewSceneTree();
    //@}

    /** @name Accesors*/
    //@{
    Bool            HasIcon( const Sb::CTypeInfo& type ) const;
    QIcon           GetIcon( const Sb::CTypeInfo& type ) const;
    Bool            CanDropOn( const CHandle& h ) const;
	CCtrlScene&     GetCtrl();
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types
 
    // Methods
    Q_SLOT Bool		OnUIEditElement( );
    Q_SLOT Bool		OnUIRemoveElement( );
    Q_SLOT Bool		OnUIAddElement( );

	Void			contextMenuEvent(QContextMenuEvent * event );
	UInt32			GetIconIdFromType( const Sb::CTypeInfo& info ) const;
    Void			OnProjectLoaded( CProject& p );
    Void            OnProjectCreated( CProject& p );
    Bool            ConfigureAddMenu( QMenu& subMenu );
    CHandle         GetRootObject( CProject& p );

    // Attributes
 	Sb::CRef<CCtrlScene>	    m_CtrlScene;
	CIconTable			        m_IconTable;

    // Static methods

};

};

#endif