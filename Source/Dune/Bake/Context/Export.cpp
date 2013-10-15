#include <Dune\Bake\Context\Export.h>

#include <Dune\Bake\RequestDispatcher.h>
#include <Dune\Model\Bank\Header.h>

#include <Sable\Core\Bank\Bank.h>

#include <QtCore\QFileInfo>
#include <QtCore\QDir>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CBakeContextExport, Sb::CManagedObject )

CBakeContextExport::CProcessThread::CProcessThread( CBakeContextExport& bakeContext )
{
	BakeContext = &bakeContext;
}	

Void CBakeContextExport::CProcessThread::Entry()
{
	CBakeRequestDispatcher::Iterator it;

    // Create one Sable bank for each Dune Bank and add it the the Bank library
    for( UInt32 i=0; i<BakeContext->GetProject().GetBankCount(); i++ )
    {
        Dune::CBank* duneBank = BakeContext->GetProject().GetBank( i );
        Sb::CFilePath tmp = BakeContext->GetBankOutputFilePath( *duneBank );

        BakeContext->m_BankLibrary.AddBank( Sb::CFilePath( tmp.GetBuffer() ) );
    }

    // For each dispatched object, create a corresponding CBakeObjectSet
    // and add it to the bake context object set table
	ForEachItem( it, BakeContext->GetDispatcher() )
	{
		const SBakeRequest& bakeRequest = (*it);

		CBakeObjectSet bakeObjSet( bakeRequest.Obj->GetName() );
		// Build and prepare the bake group
		bakeRequest.Obj->PrepareBakeObjectSet( *BakeContext, bakeObjSet );

		BakeContext->m_ObjectSetTable.AddItem( bakeRequest.Obj->GetHandle(), bakeObjSet );

	}

	ForEachItem( it, BakeContext->GetDispatcher() )
	{
		const SBakeRequest& bakeRequest = (*it);

		CBakeObjectSet*	bakeObjSet = BakeContext->GetObjectSet( bakeRequest.Obj->GetHandle() );

		// If object derive fro m CBankObject, 
		// add the corresponding Sable objet(s) to the sable bank
		CBankObject* bankObj = SafeCastRef( bakeRequest.Obj, CBankObject );
		if( bankObj )
		{
			UInt32 i;
			// Add each sable object in set to the corresponding sable bank
			// in the bank archive
			for( i=0;i<bakeObjSet->GetObjCount();i++ )
			{
				Sb::CBank* sableBank = BakeContext->m_BankLibrary.GetBank( bankObj->GetBank()->GetName() );
				if( sableBank->AddObject( *bakeObjSet->GetObj<Sb::CManagedObject>( i ), bakeObjSet->GetName() ) == FALSE )
				{
					BakeContext->LogWarning("Failed to add object: %s.", bakeObjSet->GetName() );
				}
			}
		}

		// Bake each sable object
		bakeRequest.Obj->BakeObjectSet( *BakeContext, *bakeObjSet );
	}
}

CBakeContextExport::CBakeContextExport( Dune::CProject& proj, CBakeRequestDispatcher& dispatcher, const SBakeConfiguration& config ) :
	CBakeContext( proj, config ),
    m_Dispatcher( dispatcher ),
	m_Thread( *this )
{
    m_State.Clear();
    DebugAssert( config.ThreadCount > 0 );

    m_Dispatcher.SetContext( *this );

}

CBakeContextExport::~CBakeContextExport()
{

}

/**
Start each working thread, and pass them their attached bakeRequest table.
*/
Void CBakeContextExport::ProcessRequestQueue()
{
    m_State.SetBit( nState_ProcessingRequest );
	m_Thread.Start( );
}

Bool CBakeContextExport::IsProcessingFinished() const
{
    return m_Thread.IsFinished();
}

CBakeRequestDispatcher& CBakeContextExport::GetDispatcher()
{
    return m_Dispatcher;
}

Bool CBakeContextExport::PrepareOutputDirectory()
{
    QFileInfo outputPath( GetConfig().OutputPath.GetBuffer() );
    if( outputPath.exists()  )
    {
        DeleteWholeFolder( outputPath.absoluteFilePath() );
    }

    QDir root;
    root.mkpath( outputPath.absoluteFilePath() );

    outputPath.refresh();

    if( outputPath.exists() == FALSE || outputPath.isDir() == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

Void CBakeContextExport::DeleteWholeFolder( const QString& dirString )
{
    QDir dir(dirString);

    //First delete any files in the current directory
    QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    for( Int32 file = 0; file < files.count(); file++)
    {
        dir.remove(files.at(file).fileName());
    }

    //Now recursively delete any child directories
    QFileInfoList dirs = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    for(int dir = 0; dir < dirs.count(); dir++)
    {
        DeleteWholeFolder(dirs.at(dir).absoluteFilePath ());
    }

    //Finally, remove empty parent directory
    dir.rmdir( dir.path() );
}

Void CBakeContextExport::Start( )
{
    if( !PrepareOutputDirectory() )
        return;

    // Dispatche Processing bakeRequest
    m_Dispatcher.Dispatch();

    // Process bakeRequest queue
    ProcessRequestQueue();

}

/**
This function should be called as soon as possible by the main thread in order to keep 
an eyes on the update process.

\return TRUE when the baking is finished, FALSE otherwise
*/
Bool CBakeContextExport::Update()
{
    if( IsProcessingFinished() && ! m_State.GetBit( nState_Finished ) )
    {
        // Save bank
		m_BankLibrary.Save();

        m_State.SetBit( nState_Finished );
        return FALSE;
    }

    return TRUE;
}

Void CBakeContextExport::Cancel()
{
    m_State.SetBit( nState_AskCancel );
}

Bool CBakeContextExport::IsCancelAsked() const
{
    return m_State.GetBit( nState_AskCancel );
}
