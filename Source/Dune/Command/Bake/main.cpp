
#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Common\Manager.h>

namespace Sb = Sable;

#include <Dune\Model\Project\Header.h>
#include <Dune\Common\Log.h>
#include <Dune\Common\XMLDataLayout.h>
#include <Dune\Common\CommandLineParser.h>
#include <Dune\Bake\Configuration.h>
#include <Dune\Bake\RequestDispatcher.h>
#include <Dune\Bake\Context\header.h>

#include <QtCore\QString>
#include <QtCore\QDir>
#include <QtCore\QFileInfo>
#include <QtCore\QCoreApplication>

using namespace Dune;

Bool PrepareOutputPath( const QString&  path )
{
    // Grab output path
    QDir outputPathInfo( path );

	if( outputPathInfo.exists() )
    {
        return TRUE;
    }
    else
    {
        return outputPathInfo.mkpath( path );
    }

    return FALSE;
}

Int32 Process( const QStringList& cmdLine )
{
    CCommandLineParser cmdLineArgs;
    cmdLineArgs.Parse( cmdLine );

	Bool provideOutput = FALSE;
	// Check if we provide an output path
	Sb::CFilePath outputPath;

	if( cmdLineArgs.HasOption( "o" ) )
	{
        outputPath = cmdLineArgs.GetOptionValue( "o" ).toLocal8Bit().constData();
		provideOutput = TRUE;
	}

	Char dir[256];
	Char name[256];
	Sb::CFilePath filePath = cmdLineArgs.GetOptionValue( "i" ).toLocal8Bit().constData();
	CProject				project( filePath );

	if( project.GetProjectFilePath().Exist() == FALSE )
	{
		DebugLogWarning( "Project: %s does not exist", project.GetProjectFilePath().GetBuffer() );
	}

	// Try to bake project first
	{
		CXMLDataLayout			dataLayout;
		Sb::CPersistentArchive		archive(dataLayout, project.GetProjectFilePath(), nAccesMode_Read );
		CBakeRequestDispatcher	bakeDispatcher;
		SBakeConfiguration		bakeConfig;

		if( !archive.Serialize(project, WT("Project") ) )
        {
            DebugLogWarning( "Failed to load project %s ", project.GetProjectFilePath().GetBuffer() );
            return 0;
        }

		archive.FlushData();

		if( !provideOutput )
			outputPath = project.GetSettings().GetBakeOutputPath();

		bakeConfig.OutputPath = outputPath;

		// If no errors occurs during project loading
		if( archive.GetError() )
		{
			DebugLogWarning( "Failed to load project %s ", project.GetProjectFilePath().GetBuffer() );
			return 0;
		}

		// Create the full directory hierarchy for the output path
		if( PrepareOutputPath( outputPath.GetBuffer() ) == FALSE )
		{
			DebugLogWarning( "Failed to create output path: %s ",  outputPath.GetBuffer() );
			return 0;
		}

		// Bake !
		Sb::DebugLogInfo( "\nBaking project: %s\nProject file: %s\nOutput path: %s", 
			project.GetProjectFilePath().GetName( name, 256 ),
			project.GetProjectFilePath().GetBuffer(),
			project.GetSettings().GetBakeOutputPath().GetBuffer() );

		CBakeContextExport bakeContext(  project.GetRepo(), bakeDispatcher, bakeConfig );
		bakeContext.Start( );

		while( bakeContext.Update() )
		{
			//Sleep( 100 );
		}
	}
	// Save project if needed
	{
		if( cmdLineArgs.HasOption( "s" ) )
		{
			CXMLDataLayout			dataLayout;
			Sb::CPersistentArchive		archive(dataLayout, project.GetProjectFilePath(), nAccesMode_Write );

			archive.Serialize(project, WT("Project") );
			archive.FlushData();
		}
	}

    return 0;
}

Int32 main( Int32 argc, Char** argv )
{
    QCoreApplication qtApp( argc, argv );
	CCoreManager coreManager;
	CDuneLogHandler	logHandler;
	CoreManager.GetLog().AddHandler( logHandler );

	return Process( qtApp.arguments() );
}

