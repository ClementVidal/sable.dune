#ifndef _DUNE_EDITOR_WIDGET_DAEOBJECTPICKER_
#define _DUNE_EDITOR_WIDGET_DAEOBJECTPICKER_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Application\Application.h>

#include <QtGui\QDialog>
#include <QtGui\QTreeWidget>
#include <QtGui\QComboBox>
#include <QtGui\QPushButton>

#include <vector>


namespace Dune
{

class CFBXObject;

/**
FBX object picker
*/
class CFBXObjectPicker : public QDialog
{
    
    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
	struct SResult
	{
		Sb::CRef<CFBXObject>	FBXObject;
		const Sb::CTypeInfo*	Type;
	};
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CFBXObjectPicker( CApplication& app, CFBXDoc& dae );
    virtual ~CFBXObjectPicker();
    //@}

    /** @name Accesors*/
    //@{
	const Sb::CArray<SResult>& GetResult( ) const;
	CApplication& GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

	// Types
	struct STreeItemData : public QTreeWidgetItem
	{
	public:
		STreeItemData( CFBXObject* obj, const Sb::CTypeInfo& t );
		Sb::CRef<CFBXObject>	FBXObject;
		const Sb::CTypeInfo*	Type;
	};

    // Methods
	Void PopulateTree( );
	Void PopulateTree( FbxNode* node );
	Void PopulateMesh( FbxNode* node );

	Q_SLOT Bool OnUISelectionChanged();
	Q_SLOT Bool OnUIOk();
	Q_SLOT Bool OnUICancel();

    // Attributes
	Sb::CArray<SResult>		m_Results;
	CFBXDoc&				m_FBXDoc;
	QPushButton*			m_OkButton;
    QTreeWidget*			m_TreeCtrl;
	QComboBox*				m_AssetTypeCombo;

	QTreeWidgetItem*		m_MeshTreeItem;
	
    CApplication*			m_App;
};

};

#endif