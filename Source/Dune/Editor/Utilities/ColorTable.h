#ifndef _DUNE_EDITOR_UTILITIES_COLORTABLE_
#define _DUNE_EDITOR_UTILITIES_COLORTABLE_

#include <Sable/Core/Common/DataTypes.h>

#include <QtGui/Qcolor>

#define COLOR_TABLE_SIZE 12



namespace Dune
{

/**
Predefined color Table
*/
class CColorTable
{

public:

    /** @name Accesors*/
    //@{
    static UInt32   GetColorCount();
    //@}

    /** @name Manipulator*/
    //@{
    static  QColor& GetColor( UInt32 i );
    //@}

private:

    // Attributes
    static QColor    m_ColorTable[];

};


}


#endif