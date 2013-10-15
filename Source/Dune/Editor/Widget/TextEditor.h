#ifndef _DUNE_EDITOR_WIDGET_TEXTEDITOR_
#define _DUNE_EDITOR_WIDGET_TEXTEDITOR_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Application\Application.h>

#include <QtGui\QDialog>
#include <QtGui\QTextEdit>


namespace Dune
{

/**
Material custom edit dialog
*/
class CTextEditor : public QDialog
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CTextEditor( CApplication& app );
    ~CTextEditor();
    //@}

    /** @name Accesors*/
    //@{
    CApplication&	GetApp();
	Void			SetText( const Sb::CString& s );
	Sb::CString		GetText() const;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types

    // Methods
	Q_SLOT				void OnUIOk();
	Q_SLOT				void OnUICancel();

    // Attributes
    QTextEdit*				m_TextEdit;
    CApplication*			m_App;
};

};

#endif