#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Common/Manager.h>
#include <Sable/Media/Dds/Header.h>
#include <Sable/Media/Tga/Header.h>

#include <Dune/Model/Project/Header.h>
#include <Dune/Common/Log.h>
#include <Dune/Bake/Configuration.h>

#include <wx/cmdline.h>
#include <wx/filename.h>
#include <wx/dir.h>

using namespace Dune;
Void Process3DTexture( CTextureInfo& textureInfo, const wxFileName& inputFileName, const wxFileName& outputFileName )
{
	if( inputFileName.IsDir() == FALSE )
	{
		Sb::DebugLogError( "Input must be an existing directory" );
		return;
	}
	if( inputFileName.DirExists() == FALSE)
	{
		Sb::DebugLogError( "Input directory does not exist" );
		return;
	}
	if( outputFileName.IsDir() == TRUE )
	{
		Sb::DebugLogError( "Output must be a file" );
		return;
	}

	Int32 i;
	wxArrayString files;

	Int32 count = wxDir::GetAllFiles( inputFileName.GetPath(), &files, WT("*.tga"), wxDIR_FILES );

	if( count == 0 )
	{
		Sb::DebugLogError( "Input directory does not contain any valid TGA file" );
		return;
	}

	Char tmp[256];
	textureInfo.Depth = count;
	CDDSFile ddsFile; // A METTRE A JOUR !! //nDDSFileTextureType_Volume, textureInfo );
	for(i=0;i<count; i++ )
	{
		CTGAFile tgaFile;

		Sb::WStringToString( files[i].wc_str(), tmp, 256 );
		if( tgaFile.Load( Sb::CFilePath( tmp ) ) )
		{
			if( tgaFile.GetTextureInfo().Format != textureInfo.Format || 
				tgaFile.GetTextureInfo().Width != textureInfo.Width || 
				tgaFile.GetTextureInfo().Height != textureInfo.Height )
			{
				Sb::DebugLogError( "Source file %s, does not have a valid size or format", tmp );
				Sb::DebugLogError( "All files must be %d width and %d height", textureInfo.Width, textureInfo.Height );
				Sb::DebugLogError( "All files must have A8R8G8B8 format" );
				return ;
			}
			DebugError("A mettre a jour");
			UInt32 size;
			const Byte* data = tgaFile.GetSurfaceData( 0, 0, size );
			ddsFile.SetSurfaceData( i, 0, data, size );
		}
		else
		{
			Sb::DebugLogError( "Failled to load %s", tmp );
			Sb::DebugLogError( "File does not exist or is not a valid TGA file" );
			return ;
		}
	}

	Sb::WStringToString( outputFileName.GetFullPath(), tmp, 256 );

	if( ddsFile.Save( Sb::CFilePath( tmp ) ) == FALSE )
	{
		Sb::DebugLogError( "Failled to save output dds file" );
		return;
	}
}

Int32 Process( const wxCmdLineParser& cmdLine )
{
    Char inputDir[256];
    Char inputName[256];
    wxFileName inputFileName( cmdLine.GetParam( 0 ).GetData() );
    Sb::WStringToString( (WString) inputFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR ).GetData(), inputDir, 256 );
    Sb::WStringToString( (WString) inputFileName.GetName().GetData(), inputName, 256 );

	Char outputDir[256];
	Char outputName[256];
	wxFileName outputFileName( cmdLine.GetParam( 1 ).GetData() );
	Sb::WStringToString( (WString) outputFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR ).GetData(), outputDir, 256 );
	Sb::WStringToString( (WString) outputFileName.GetName().GetData(), outputName, 256 );

	wxString textureType;
	CTextureInfo textureInfo;
	if( cmdLine.Found( WT("t"), &textureType ) )
	{
		if( textureType == WT("2d") )
		{

		}
		else if( textureType == WT("3d") )
		{
			long size = 128;
			cmdLine.Found(WT("s"), &size );
			textureInfo.Width = size;
			textureInfo.Height = size;
			textureInfo.MipMapCount = 1;
			textureInfo.Format = nTextureFormat_R8G8B8A8;
			Process3DTexture( textureInfo, inputFileName, outputFileName );
		}
		else if( textureType == WT("cu") )
		{

		}
	}

	return 1;
}

wxCmdLineEntryDesc cmdLineDesc[] =
{
	{ wxCMD_LINE_SWITCH,  "h", "help", "Show help" },
	{ wxCMD_LINE_OPTION,  "t", "type", "Texture type (cu, 2d, 3d) to create or process", wxCMD_LINE_VAL_STRING, 0 },
	{ wxCMD_LINE_OPTION,  "s", "size", "Texture size (both width and height)", wxCMD_LINE_VAL_NUMBER, 0 },
	{ wxCMD_LINE_PARAM,  NULL, NULL, "Input", wxCMD_LINE_VAL_STRING, 0 },
	{ wxCMD_LINE_PARAM,  NULL, NULL, "Output", wxCMD_LINE_VAL_STRING, 0 },

	{ wxCMD_LINE_NONE }
};


Int32 main( Int32 argc, Char** argv )
{
	CCoreManager coreManager;
	CDuneLogHandler	logHandler;
	CDuneLogOutput output;
	wxLog::SetActiveTarget( &output );
	CoreManager.GetLog().AddHandler( logHandler );


	wxCmdLineParser cmdLine( argc, argv );
	cmdLine.SetSwitchChars( WT("-") );
	cmdLine.SetDesc( cmdLineDesc );
	if( cmdLine.Parse( false ) != 0  )
	{
		cmdLine.Usage();
		return -1;
	}
	
	return Process( cmdLine );
}

