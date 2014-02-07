#ifndef _DUNE_EDITOR_UTILITIES_ICONTABLE_
#define _DUNE_EDITOR_UTILITIES_ICONTABLE_

#include <Sable/Core/Common/DataTypes.h>

#include <QtGui/QPixmap>
#include <QtGui/QIcon>



namespace Dune
{

/**
Icon table
*/
class CIconTable
{

public:

    /** @name Constructor/Destructor*/
    //@{
	CIconTable();
	CIconTable( const char** xpmData );
	~CIconTable();
    //@}

	/** @name Accesors*/
	//@{
	QIcon	GetIcon( UInt32 i ) const;
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Attributes
	QPixmap	m_Pixmap;
};


}


#endif