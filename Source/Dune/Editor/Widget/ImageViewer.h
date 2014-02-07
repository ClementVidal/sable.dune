#ifndef _DUNE_EDITOR_WIDGET_IMAGEVIEWER_
#define _DUNE_EDITOR_WIDGET_IMAGEVIEWER_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Utilities/IconTable.h>

#include <QtGui/Qlabel>
#include <QtGui/QScrollArea>
#include <QtGui/QDockWidget>

namespace Sable
{
    class CImageFile;
}

namespace Dune
{

/**
Font editor widget
*/
class CImageViewer : public QDockWidget
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CImageViewer( CApplication& app, CAssetTexture2D& texture );
    virtual ~CImageViewer();
    //@}

    /** @name Accesors*/
    //@{
    CApplication&	GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
	//@}

private:

    // Types

    // Methods
    Void Load();
    Void Load( Sb::CImageFile& image );

    // Attributes
    CApplication*       m_App;
    QWidget*                m_Frame;
    QLabel*                 m_ImageLabel;
    QScrollArea*            m_ScrollArea;
    CAssetTexture2D*        m_Texture2D;
};

};

#endif