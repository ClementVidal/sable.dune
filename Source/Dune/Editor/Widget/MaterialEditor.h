#ifndef _DUNE_EDITOR_WIDGET_MATERIALEDITOR_
#define _DUNE_EDITOR_WIDGET_MATERIALEDITOR_

#include <Sable/Core/Common/DataTypes.h>
#include <Dune/Editor/Application/Application.h>

#include <QtGui/QDialog>
#include <QtGui/QTreeWidget>



namespace Dune
{

class CBankObject;

/**
Material custom edit dialog
*/
class CMaterialEditor : public QDialog
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    enum EEvent
    {
        nEvent_Add = 0,
		nEvent_Ok,
		nEvent_Cancel,
        nEvent_Remove,
		nEvent_GridID = 128
    };
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CMaterialEditor( CApplication& app, CAssetMaterial& obj );
    virtual ~CMaterialEditor();
    //@}

    /** @name Accesors*/
    //@{
    CApplication& GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types

    // Methods
	UInt32				ConvertType( const CPropertyInterface& p );
	const Sb::CTypeInfo*	ConvertType( const QString& type );
    Void				Populate();
	Void				UpdateMaterial();
	Void				AddParameter( QString name, QString type );

	Q_SLOT				Bool OnUIAddParameter();
	Q_SLOT				Bool OnUIRemoveParameter();
	Q_SLOT				Bool OnUIOk();
	Q_SLOT				Bool OnUICancel();

    // Attributes
    CAssetMaterial*			m_Object;
	QTreeWidget*			m_Table;
    CApplication*		m_App;
	QStringList				m_TypeList;
};

};

#endif