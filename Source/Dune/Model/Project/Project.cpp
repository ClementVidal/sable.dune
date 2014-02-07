#include <Dune/Model/Project/Project.h>


using namespace Dune;

CProject::CProject( const Sb::CFilePath& path ) :
    m_SystemBank( *this, "System" )
{
	Char tmp[128];

	m_Name = path.GetName( tmp, 128 );
	m_Directory = path.GetDirectory( tmp, 128 );

	m_FilePath = path;

	AddDefaultAssetPath();
    CreateRootObjects();
}

CProject::~CProject()
{
    while( GetBankCount() )
    {
        RemoveBank( GetBank(0)->GetUuid() );
    }
}

Void CProject::AddDefaultAssetPath()
{
	Sb::CFilePath libPath( GetProjectDirectory() );
	libPath.AppendDir( "Library" );

	Sb::CFilePath rootPath( GetProjectDirectory() );
	libPath.MakeRelativeTo( rootPath );

	m_Settings.AddAssetPath( libPath );
}

Void CProject::CreateRootObjects()
{
	CBankObject* object = NULL;
    object = m_SystemBank.AddObject( CBankObjectDirectory::GetStaticTypeInfo(), CHandle() );
	m_RootUI = object->GetHandle();

    object = m_SystemBank.AddObject( CBankObjectDirectory::GetStaticTypeInfo(), CHandle() );
	m_AssetRoot = object->GetHandle();

    object = m_SystemBank.AddObject( CBankObjectDirectory::GetStaticTypeInfo(), CHandle() );
	m_SceneRoot = object->GetHandle();

	object = m_SystemBank.AddObject( CBankObjectDirectory::GetStaticTypeInfo(), CHandle() );
	m_GameRoot = object->GetHandle();
	
}

Bool CProject::GetIsSystemBank( const CUuid& b ) const
{
    return m_SystemBank.GetUuid() == b;
}

String CProject::GetProjectName() const
{
    return m_Name;
}

Bool CProject::Serialize( Sb::CPersistentArchive& ar )
{
    Int16 version = 0;
    CUuid systemBankUuid = m_SystemBank.GetUuid();

	if( ! ar.Serialize( m_Name, WT("Name") ) )
		return FALSE;

    if( !ar.Serialize( m_Settings, WT("Settings") ) )
		return FALSE;

    if( !ar.Serialize( systemBankUuid, WT("Uuid") ) )
        return FALSE;

    if( ar.IsLoading() )
    {
        m_SystemBank.SetUuid( systemBankUuid );
        m_SystemBank.Clear();
        CreateRootObjects();
    }

    UInt32 bankCount = m_BankMap.GetItemCount();

    if( ar.BeginScope( WT("BankTable"), version ) == version )
    {
        ar.Serialize( bankCount, WT( "BankCount" ) );
        if( ar.IsStoring() )
        {
            BankMap::Iterator it;
            ForEachItem( it, m_BankMap )
            {
                if( ar.BeginScope( WT("Bank"), version ) == version )
                {
					// Create a relocated path from the project dir and the bank relative path
					Char rootDir[256];
					GetProjectFilePath().GetDirectory( rootDir, 256 );

					Sable::CFilePath rootPath( rootDir );
					Sable::CFilePath bankPath = (*it).FilePath;
					bankPath.MakeRelativeTo( rootPath );

                    ar.Serialize( it.GetKey(), WT("Uuid") );
                    ar.Serialize( bankPath, WT("FilePath") );

                    ar.EndScope();
                }
            }

            if( SerializeExternalBank( ar.IsStoring() ) == FALSE )
                return FALSE;
        }
        else
        {
            UInt32 i=0;
            for(i=0; i<bankCount; i++ )
            {
                CUuid uuid;
                Sb::CFilePath filePath;

                if( ar.BeginScope( WT("Bank"), version ) == version )
                {
                    ar.Serialize( uuid, WT("Uuid") );
                    ar.Serialize( filePath, WT("FilePath") );

					// Create a relocated path from the project dir and the bank relative path
					Char rootDir[256];
					GetProjectFilePath().GetDirectory( rootDir, 256 );

					Sable::CFilePath relocatedPath;
					if( filePath.IsRelative() )
					{
						relocatedPath =  Sable::CFilePath( rootDir );
						relocatedPath.AppendRelativePath( filePath );
					}
					else
					{
						relocatedPath = filePath;
					}

                    CBank* bank = AddExistingBank( relocatedPath );
					if( !bank )
					{
						ar.EndScope();
						return FALSE;
					}

                    if( bank->GetUuid() != uuid )
                    {
                        CoreManager.GetLog().LogWarning( "A loaded bank does not match the uuid listed in the project" );
						ar.EndScope();
						return FALSE;
                    }

                    ar.EndScope();
                }
            }

            for(i=0; i<GetBankCount(); i++ )
            {
                RebuildParentship( *GetBank( i ) );
            }
        }
        ar.EndScope();

    }

    return TRUE;
}

/**
Use this method when some object of a bank A are child of objects within bank B.
This will validate/consolidate/correct parentship of objects across banks
*/
Void CProject::RebuildParentship( CBank& bank )
{
    CBank::Iterator it;
    ForEachItem( it, bank )
    {
        CBankObject* obj = &(*it);
        CBankObject* parentObj = GetObj( obj->GetParent() );
        
        parentObj->AddChild( obj->GetHandle() );
    }
}

Bool CProject::SerializeExternalBank( Bool isStoring )
{
    BankMap::Iterator it;
    ForEachItem( it, m_BankMap )
    {
        const SBankEntry& entry = *it;

        Sb::CXMLDataLayout      dataLayout( entry.FilePath );
        Sb::CPersistentArchive ar(dataLayout, isStoring?nAccesMode_Write:nAccesMode_Read   );
        
        if( isStoring )
            CoreManager.GetLog().LogInfo( "Saving external bank: %s", entry.FilePath.GetPath() );
        else
            CoreManager.GetLog().LogInfo( "Loading external bank: %s", entry.FilePath.GetPath() );

        ar.Serialize( *entry.Bank, WT("Bank" ) );

        ar.FlushData();
    
    }

    return TRUE;
}

String CProject::GetProjectDirectory() const
{
    return m_Directory;
}

CProjectSettings& CProject::GetSettings()
{
    return m_Settings;
}

const Sb::CFilePath& CProject::GetProjectFilePath() const
{
    return m_FilePath;
}

Int32 CProject::GetBankIndex( const CUuid& b ) const
{
	Int32 i = 0;
	BankMap::Iterator it;
	ForEachItem( it, m_BankMap )
	{
		if( it->Bank->GetUuid() == b )
			return i;
		i++;
	}

	if( m_SystemBank.GetUuid() == b )
		return Project_SystemBankIndex;

	return -1;
}

UInt32 CProject::GetBankCount() const
{
    return m_BankMap.GetItemCount();
}

const CBank* CProject::GetBank( Int32 i ) const
{
	UInt32 j = 0;
	BankMap::Iterator it;
	ForEachItem( it, m_BankMap )
	{
		if( i == j )
			return it->Bank;
		j++;
	}

	if( i == Project_SystemBankIndex )
	{
		return &m_SystemBank;
	}

	return NULL;
}

CBank* CProject::GetBank( Int32 i )
{
	UInt32 j = 0;
	BankMap::Iterator it;
	ForEachItem( it, m_BankMap )
	{
		if( i == j )
			return it->Bank;
		j++;
	}

	if( i == Project_SystemBankIndex )
	{
		return &m_SystemBank;
	}

	return NULL;
}

const Sb::CFilePath CProject::GetBankFilePath( const CUuid& b ) const
{
    if( m_BankMap.HasItem( b ) )
    {
        return m_BankMap.GetItem( b ).FilePath; 
    }

    return Sb::CFilePath();
}

CBankObject* CProject::GetObj( const CHandle& h ) const
{
	CBank* bank = GetBank( h.GetBankUuid() );
	if( !bank )
		return NULL;

	return bank->GetObj( h );
}

CBank* CProject::GetBank( const CUuid& b ) const
{
    if( m_BankMap.HasItem( b ) )
        return m_BankMap.GetItem( b ).Bank;
    else  if( m_SystemBank.GetUuid()  == b )
        return &m_SystemBank;

    return NULL;
}

CBank* CProject::GetBank( String name )
{
    return NULL;
}

CBank* CProject::AddExistingBank( const Sb::CFilePath& path )
{
	if( path.Exist() == FALSE )
	{
		CoreManager.GetLog().LogError( "A required bank cannot be found: %s", path.GetBuffer() );
		return NULL;
	}

    Sb::CRef<CBank> bank = NEWOBJ( CBank, ( *this ) );

    Sb::CXMLDataLayout      dataLayout( path );
    Sb::CPersistentArchive ar(dataLayout, nAccesMode_Read );

    ar.Serialize( *bank, WT("Bank" ) );

    if( GetBank( bank->GetUuid() ) )
    {
        CoreManager.GetLog().LogWarning( "Bank with a same Uuid already loaded: %s", path.GetPath() );
        return NULL;
    }
    else        
    {
        UInt32 i;
        for(i=0;i<GetBankCount();i++)
        {
            if( !Sb::StringCompare( GetBank( i )->GetName(), bank->GetName() ) )
            {
                CoreManager.GetLog().LogWarning( "A bank with this name already exist: %s", bank->GetName() );
                return NULL;
            }
        }
    }

    SBankEntry entry;
    entry.Bank = bank;
    entry.FilePath = path;
    m_BankMap.AddItem( bank->GetUuid(), entry );

    ar.FlushData();

	// Notify bank added
	SignalBankAdded( *bank );

    return bank;
}

/**
Execute a query over the repo based on a string expression.

Supported expression can be as follow:

- AttribName = Value :
    - AttribName: the name of a specific property in the object
    - Value: a string value used to compare.

- IsKindOf( TypeName ) :
    - TypeName: The name of a C++ class type ( like CWidgetFrame ... )

- * :
    - Grab every object of the database
*/
Void CProject::IssueQuery( String text, Sb::CList<CHandle>& result ) const
{
	for( UInt32 j=0;j<GetBankCount();j++ )
	{
		GetBank( j )->IssueQuery( text, result );
	}
}

CFBXManager& CProject::GetFBXManager()
{
    return m_FBXManager;
}

CBank* CProject::AddNewBank( const Sb::CFilePath& path, String name )
{
    // test de validation sur le nom de la banque
    UInt32 i=0;
    if( Sb::StringGetLength( name ) == 0 )
    {
        CoreManager.GetLog().LogWarning( "Invalid empty bank name" );
        return NULL;
    }
    else
    {
        for(i=0;i<GetBankCount();i++)
        {
            if( !Sb::StringCompare( GetBank( i )->GetName(), name ) )
            {
                CoreManager.GetLog().LogWarning( "A bank with this name already exist: %s", name );
                return NULL;
            }
        }
    }
 

    CBank* bank = NEWOBJ( CBank, ( *this ) );

    Sb::CXMLDataLayout      dataLayout( path );
    Sb::CPersistentArchive ar(dataLayout, nAccesMode_Write );

    CoreManager.GetLog().LogInfo( "Saving Bank: %s", path.GetPath() );
    ar.Serialize( *bank, WT("Bank" ) );

    SBankEntry entry;
    entry.Bank = bank;
    entry.FilePath = path;
	bank->SetName( name );
    m_BankMap.AddItem( bank->GetUuid(), entry );

    ar.FlushData();

    SignalBankAdded( *bank );

    return bank;
}

Bool CProject::RemoveBank( const CUuid& b )
{
    CBank* bank = GetBank( b );
    if( bank )
    {
        SignalBankRemoved( *bank );
        m_BankMap.RemoveItemAtKey( b ); 
    }

    return FALSE;
}

const CHandle& CProject::GetRootUI() const
{
    return m_RootUI;
}

const CHandle& CProject::GetRootAsset() const
{
    return m_AssetRoot;
}

const CHandle& CProject::GetRootGame() const
{
	return m_GameRoot;
}

const CHandle& CProject::GetRootScene() const
{
    return m_SceneRoot;
}