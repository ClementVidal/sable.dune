#ifndef _DUNE_EDITOR_VIEW_ASSETTREE_
#define _DUNE_EDITOR_VIEW_ASSETTREE_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\View\BankObjectTree.h>
#include <Dune\Editor\Ctrl\EventClient.h>
#include <Dune\Editor\Utilities\IconTable.h>

#include <QtGui\QTreeWidget>
#include <QtCore\QMimeData>
#include <QtCore\QFileSystemWatcher>

namespace Dune
{

class CCtrlAsset;
class CAsset;
class CApplication;
class CPropertyInterface;
class CProject;
class CHandle;
class CViewAsset;

/**
UI representation of a project
*/
class CViewAssetTree : public CViewBankObjectTree
{

	Q_OBJECT
    
public:

	/** @name Constructor/Destructor*/
    //@{
    CViewAssetTree( CViewAsset& assetView, CApplication& app );
    virtual ~CViewAssetTree();
    //@}

    /** @name Accesors*/
    //@{
    Bool            HasIcon( const Sb::CTypeInfo& type ) const;
    Bool            CanDropOn( const CHandle& h ) const;
	CCtrlAsset&		GetCtrl();
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // friendship
    friend CCtrlAsset;

    // Types

    // Methods
    CHandle         GetRootObject( CProject& p );

private :

	Q_SLOT Bool     OnUIRemoveAsset( );
	Q_SLOT Bool     OnUIEditAnimCurveSet();
	Q_SLOT Bool		OnUIEditFont();
	Q_SLOT Bool		OnUIEditShader();
	Q_SLOT Bool		OnUIEditParticleSystem();
	Q_SLOT Bool		OnUIEditMaterial();
    Q_SLOT Bool     OnUIDisplayTexture();
    Q_SLOT Bool     OnUIUseOwnerBank();
	Q_SLOT void		OnUIExportToLibrary();

	void			contextMenuEvent(QContextMenuEvent * e);
    Void			AddEditEntry( CBankObject& asset, QMenu& subMenu );
    Bool            IsDirectory( const QTreeWidgetItem& item );
    Void            OnProjectCreated( CProject& p );
	QIcon			GetIcon( const Sb::CTypeInfo& type ) const;

    // Attributes   
	Sb::CRef<CCtrlAsset>	m_Ctrl;
	CViewAsset*				m_AssetView;

};

};

#endif