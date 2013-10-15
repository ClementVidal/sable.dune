#ifndef _DUNE_MODEL_PROJECT_PROJECT_
#define _DUNE_MODEL_PROJECT_PROJECT_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Collection\Map.h>
#include <Sable\Core\ManagedObject\Ref.h>

#include <Dune\Model\Bank\Bank.h>
#include <Dune\Model\Project\Settings.h>
#include <Dune\Model\Bank\Object.h>
#include <Dune\Common\Uuid.h>
#include <Dune\Model\Asset\FBX\Manager.h>
#include <Dune\Model\Bank\ObjectAccessor.h>

#define Project_SystemBankIndex INT32_MAX_VAL

namespace Sable
{
    class CPersistentArchive;
};

namespace Dune
{

class CProjectElement;
class CAssetDirectoryRoot;
class CApplication;
class CBank;

/** 
\ingroup ModelProject
Main project class 
*/
class CProject : public CBankObjectAccessor
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CProject( const Sb::CFilePath& path );
    ~CProject();
    //@}

    /** @name Manipulator*/
    //@{
    //@}

    /** @name Accessor*/
    //@{
    String					    GetProjectDirectory() const;
    String                      GetProjectName() const;
    const Sb::CFilePath&		GetProjectFilePath() const;

    Bool                        GetIsSystemBank( const CUuid& b ) const;
    UInt32                      GetBankCount() const;
    CBank*                      GetBank( Int32 i );
	const CBank*                GetBank( Int32 i ) const;
    CBank*                      GetBank( String name );
	CBank*                      GetBank( const CUuid& b ) const;
    Int32                       GetBankIndex( const CUuid& b ) const;
    const Sb::CFilePath         GetBankFilePath( const CUuid& b ) const;
	CBankObject*				GetObj( const CHandle& h ) const;

	template< class DataType >
	DataType*					GetObj( const CHandle& h ) const;

    CBank*                      AddExistingBank( const Sb::CFilePath& path );
    CBank*                      AddNewBank( const Sb::CFilePath& path, String name );
    Bool                        RemoveBank( const CUuid& b );

    CProjectSettings&		    GetSettings() ;
	CFBXManager&				GetFBXManager();

    const CHandle&              GetRootUI() const;
	const CHandle&              GetRootAsset() const;
	const CHandle&              GetRootScene() const;
	const CHandle&              GetRootGame() const;
	Void						IssueQuery( String, Sb::CList<CHandle>& result ) const;
    //@}

    /** @name Manipulator*/
    //@{
    Bool Serialize( Sb::CPersistentArchive& ar );
    //@}

    /** @name Signal*/
    //@{
    Sb::CEventSignal< CBank& > SignalBankAdded;
    Sb::CEventSignal< CBank& > SignalBankRemoved;
    //@}

private:

    // DataTypes
    struct SBankEntry
    {
        Sb::CRef<CBank>  Bank;
        Sb::CFilePath    FilePath;
    };
    typedef Sb::CMap<CUuid, SBankEntry> BankMap;

    // Methods
    Void    CreateRootObjects();
    Bool    SerializeExternalBank( Bool isStoring );
	Void    RebuildParentship( Dune::CBank& bank );
	Void	AddDefaultAssetPath();

    // Attributes
	Sb::CStaticString<64>		m_Name;
	Sb::CStaticString<128>		m_Directory;
    Sb::CFilePath               m_FilePath;

    BankMap                     m_BankMap;
    CProjectSettings            m_Settings;
    CFBXManager                 m_FBXManager;

    CHandle                         m_RootUI;
    CHandle                         m_AssetRoot;
    CHandle                         m_SceneRoot;
	CHandle							m_GameRoot;

    mutable CBank                   m_SystemBank;

};


template< class DataType >
DataType* CProject::GetObj( const CHandle& h ) const
{
	CBankObject* obj = GetObj( h );
	if( obj && obj->GetTypeInfo().IsKindOf( DataType::GetStaticTypeInfo() ) )
		return (DataType*) obj;

	return NULL;
}

}

#endif
