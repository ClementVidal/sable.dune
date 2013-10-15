#ifndef _DUNE_EDITOR_WIDGET_SHADEREDITOR_
#define _DUNE_EDITOR_WIDGET_SHADEREDITOR_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Utilities\IconTable.h>

#include <QtGui\QTextEdit>
#include <QtGui\QListWidget>
#include <QtGui\QSyntaxHighlighter>
#include <QtGui\QDockWidget>

namespace Dune
{

/**
Font editor widget
*/
class CShaderEditor : public QDockWidget
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    enum EEvent
    {
		nEvent_Open = 0,
		nEvent_Save,
		nEvent_Undo,
		nEvent_Redo,
		nEvent_Compile
    };
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CShaderEditor( CApplication& app, CAssetShader& shader );
    virtual ~CShaderEditor();
    //@}

    /** @name Accesors*/
    //@{
	Void				SetShader( CAssetShader& asset );
    CApplication&	GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{

	//@}

private:

    // Types

    // Methods
    Void ClearLog( );
	Void Log( String txt );
	Void SaveFilesAndBake();
    Void GoTo( EShaderProgramType program, Int32 lineNo, Int32 columnNo );
    Bool LoadFile ( const Sb::CFilePath& filename, EShaderProgramType programType  );
    Bool SaveFile (	const Sb::CFilePath& filename, EShaderProgramType programType  );
	Q_SLOT Bool OnUIOpen( );
	Q_SLOT Bool OnUISave( );
	Q_SLOT Bool OnUIUndo( );
	Q_SLOT Bool OnUIRedo( );
	Q_SLOT Bool OnUICompile( );
    Q_SLOT void OnUITabChanged( int i );
    Q_SLOT void OnUISelectError( QListWidgetItem * item );

    // Attributes
    CApplication*       m_App;
	QTextEdit*				m_TextEdit[nShaderProgramType_Count];

	CAssetShader*			m_Shader;
	QListWidget*			m_LogWindow;

	CIconTable				m_IconTable;
	QWidget*				m_Frame;

    QTabWidget*             m_TabWidget;

    EShaderProgramType      m_CurrentProgramEdited;
};

};

#endif