#include <Dune/Editor/Ctrl/Game.h>

#include <Dune/Editor/View/Property.h>
#include <Dune/Model/Game/Header.h>
#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Common/LibraryObject.h>
#include <Dune/Editor/Ctrl/Cmd/Header.h>

using namespace Dune;

CCtrlGame::CCtrlGame( CApplication& app ) :
    CCtrl( app )
{
}

CCtrlGame::~CCtrlGame()
{
}

/**
Use this method to add Controller related processing when an object is created
*/
Void CCtrlGame::OnAddObject( const CHandle& parent, const CHandle& h )
{
	
}

Void CCtrlGame::ExportToLibrary( CBankObject& obj, const Sb::CFilePath& path )
{
	CLibraryObject libObj( obj, "" );

	libObj.Save( path );

}