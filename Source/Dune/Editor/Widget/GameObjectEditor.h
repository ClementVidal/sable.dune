#ifndef _DUNE_EDITOR_WIDGET_GAMEOBJECTEDITOR_
#define _DUNE_EDITOR_WIDGET_GAMEOBJECTEDITOR_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Utilities\IconTable.h>

#include <QtGui\QTextEdit>
#include <QtGui\QListWidget>
#include <QtGui\QTreeWidget>
#include <QtGui\QDialog>

namespace Dune
{

class CGameObjectPrototype;

/**
Game Object editor
*/
class CGameObjectEditor : public QDialog
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CGameObjectEditor( CApplication& app, CGameObjectPrototype& proto );
    virtual ~CGameObjectEditor();
    //@}

    /** @name Accesors*/
    //@{
	Void			SetGameObjectPrototype( CGameObjectPrototype& proto );
    CApplication&	GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{

	//@}

private:

    // Types

    // Methods
	Q_SLOT				void OnUIOk();
	Q_SLOT				void OnUICancel();
	Q_SLOT				void OnUIAddParam();
	Q_SLOT				void OnUIAddService();
	Q_SLOT				void OnUIRemoveParam();
	Q_SLOT				void OnUIRemoveService();
	Q_SLOT				void OnUISelectionChanged();
	Q_SLOT				void OnUIItemChanged( QTreeWidgetItem* item, int column );

	Void				Populate();

    // Attributes
    CApplication*       m_App;

	Bool						m_ProcessItemChanged;
	CGameObjectPrototype*		m_GameObjectProto;
	QListWidget*				m_ParameterTypeList;
	QListWidget*				m_ServiceTypeList;
	QTreeWidget*				m_ObjectTree;
	QTreeWidgetItem*			m_RootNode;
	QTreeWidgetItem*			m_ServiceNode;
	QTreeWidgetItem*			m_ParameterNode;
	QPushButton*				m_RemoveService;
	QPushButton*				m_RemoveParam;
	QStringList					m_TypeList;
};

};

#endif