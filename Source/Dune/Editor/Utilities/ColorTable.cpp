#include <Dune\Editor\Utilities\ColorTable.h>

using namespace Dune;

QColor CColorTable::m_ColorTable[] = 
{
    QColor(255,0,0),
    QColor(0,255,0),
    QColor(0,0,255),
    QColor(255,255,255),
    QColor(255,0,255),
    QColor(0,255,255),
    QColor(128,0,0),
    QColor(0,128,0),
    QColor(0,0,128),
    QColor(128,128,0),
    QColor(128,0,128),
    QColor(0,128,128)
};

UInt32 CColorTable::GetColorCount()
{
    return COLOR_TABLE_SIZE;
}

QColor& CColorTable::GetColor( UInt32 i )
{
    UInt32 modi = i % COLOR_TABLE_SIZE;
    return m_ColorTable[modi];
}