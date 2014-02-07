#include <Dune/Editor/Utilities/IconTable.h>

#include <QtGui/QBitmap>

using namespace Dune;

CIconTable::CIconTable(  )
{
}

CIconTable::CIconTable( const char**  xpmData ):
	m_Pixmap( xpmData )
{
	QBitmap mask = m_Pixmap.createMaskFromColor( QColor( 255, 0, 255 ), Qt::MaskInColor );
	m_Pixmap.setMask( mask );
}

CIconTable::~CIconTable()
{

}

QIcon CIconTable::GetIcon( UInt32 i ) const
{
	Int32 height = m_Pixmap.height();
	return m_Pixmap.copy( height * i, 0, height, height );
}
