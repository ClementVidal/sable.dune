#ifndef _DUNE_BAKE_CONTEXT_CONTEXT_
#define _DUNE_BAKE_CONTEXT_CONTEXT_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\ManagedObject\ManagedObject.h>
#include <Sable\Core\ManagedObject\Ref.h>
#include <Sable\Core\Thread\CriticalSection.h>

#include <Dune\Bake\Configuration.h>
#include <Dune\Bake\Context\Context.h>
#include <Dune\Bake\ObjectSet.h>

namespace Sable
{
    class CBank;
};

namespace Dune
{

class SBakeConfiguration;
class CBank;
class CBankObject;
class CHandle;
class CBank;

/**
\ingroup Bake
Base class for Global supervisor of the baking process
*/
class CBakeContext : public Sb::CManagedObject
{

    DEFINE_MANAGED_CLASS( CBakeContext )

public:

    /** @name DataTypes */
    //@{
	enum EType
	{
		nType_Error,
		nType_Info,
		nType_Warning
	};
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CBakeContext( CProject& project, const SBakeConfiguration& config );
    virtual ~CBakeContext();
    //@}

    /** @name Accessors*/
    //@{
    Sb::CFilePath				GetBankOutputFilePath( const CBank& b );
    Sb::CFilePath				GetBankOutputDir( const CBank& b );
	Bool						HasObjectSet( const CHandle& h ) const;
    CBakeObjectSet*				GetObjectSet( const CHandle& h );
    const SBakeConfiguration&   GetConfig() const;
    CProject&                   GetProject() ;
    //@}

    /** @name Manipulator*/
    //@{
	Void LogError( const Char* format, ... );
	Void LogWarning( const Char* format, ... );
	Void LogInfo( const Char* format, ... );
	//@}

	/** @name Signal*/
	//@{
	Sb::CEventSignal< EType, String >       SignalLog;
	//@}

protected:

    // Friendship

    // Types
	typedef Sb::CMap< CHandle, CBakeObjectSet > ObjectSetTable;

	// Methods 	

    // Attributes
    CProject&							m_Project;
    const SBakeConfiguration&			m_Config;
    ObjectSetTable						m_ObjectSetTable;

	Sb::CCriticalSection				m_CriticalSection;
};

};

#endif