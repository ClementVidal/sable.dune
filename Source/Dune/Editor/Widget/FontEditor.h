#ifndef _DUNE_EDITOR_WIDGET_FONTEDITOR_
#define _DUNE_EDITOR_WIDGET_FONTEDITOR_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Graphics/Text/Font.h>

#include <Dune/Editor/Utilities/IconTable.h>

#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QPixmap>
#include <QtGui/QFontComboBox>
#include <QtGui/QLabel>
#include <QtGui/QDockWidget>

namespace Dune
{

class CFontEditorWindow;

/**
Font editor widget
*/
class CFontEditor : public QDockWidget
{
    
    Q_OBJECT()

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CFontEditor( CApplication& app, CAssetFont& asset );
    virtual ~CFontEditor();
    //@}

    /** @name Accesors*/
    //@{
    CApplication& GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
    Void SetFont( CAssetFont& font );
    //@}

private:

    // Types
	enum EDrawOption
	{
		nDrawOption_BoundingRect = ( 1 << 0 ),
		nDrawOption_CharWidth = ( 1 << 1 ),
		nDrawOption_Offset = ( 1 << 2 ),
	};

	struct SUnicodeScript
	{
		UInt32				GetCodePointCount() const;
		UInt32				LowCodePoint;
		UInt32				HighCodePoint;
		WChar				Name[64];
	};

    // Methods
	Void		DrawTable( Int32 options, QPixmap& bitmap  );
	Void		SaveFile( );
	Void		SaveFile( const Sb::CFilePath& path );
	Int32		ReadFontInfo( const Sb::CFilePath& path );
	Void		SetupControls();
	Void		DrawTable( );

	Q_SLOT		Bool OnUISave();
	Q_SLOT		Bool OnUIOpen();
	Q_SLOT		Bool OnUISpinChange( int i );
	Q_SLOT		Bool OnUIFontChanged( const QFont& );
	Q_SLOT		Bool OnUIResolutionChanged( const QString& );
	Q_SLOT		Bool OnUIScriptChanged( int index );

    // Attributes
	Sb::CTextFont::SInfo		m_FontInfo;
	CIconTable				m_IconTable;
    CApplication*       m_App;
    CAssetFont*             m_FontAsset;
	QScrollArea*			m_ScrollArea;
	QLabel*					m_ImageContainer;
	QSpinBox*				m_FontSize;
	QComboBox*				m_BitmapSize;
	QComboBox*				m_ScriptCombo;
	QFontComboBox*			m_FontName;

	Bool					m_IsDocumentModified;
	
	static SUnicodeScript			m_UnicodeScriptTable[];
	SUnicodeScript*					m_CurrentUnicodeScript;

	Sb::CArray< Sb::CTextFont::SChar >	m_UVOffsetTable;
};

};

#endif