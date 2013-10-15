#ifndef _DUNE_EDITOR_CTRL_GAME_
#define _DUNE_EDITOR_CTRL_GAME_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Event\Header.h>

namespace Dune
{


/** 
Main project class 
*/
class CCtrlGame : public CCtrl
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CCtrlGame( CApplication& app );
    virtual ~CCtrlGame();
    //@}

    /** @name Accessor*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
	Void				OnAddObject( const CHandle& parent, const CHandle& h );
	Void				ExportToLibrary( CBankObject& obj, const Sb::CFilePath& path );
	//@}

private:

    // Methods

    // Attributes

};

}

#endif
