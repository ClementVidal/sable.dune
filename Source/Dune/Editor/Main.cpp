#include <Dune\Editor\Application\Application.h>

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Common\ObjLoader.h>

#include <Shellapi.h>

using namespace Dune;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* argv, int nShowCmd)
{
	Char buffer[1024];
	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

    Sb::WStringToString( *szArgList, buffer, 1024 );
	CApplication app( argCount, (Char**)&buffer );

	app.Init();



	return app.exec();

}