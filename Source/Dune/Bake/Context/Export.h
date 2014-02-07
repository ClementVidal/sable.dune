#ifndef _DUNE_BAKE_CONTEXT_EXPORT_
#define _DUNE_BAKE_CONTEXT_EXPORT_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Thread/Header.h>
#include <Sable/Core/Bank/Header.h>
#include <Dune/Bake/Context/Context.h>

#include <QtCore/QString>

namespace Dune
{

class CBakeRequestDispatcher;

/**
\ingroup Bake
Global supervisor for baking process
*/
class CBakeContextExport : public CBakeContext
{

    DEFINE_MANAGED_CLASS( CBakeContextExport )

public:

    /** @name DataTypes */
    //@{
    enum EState
    {
        nState_ProcessingRequest = 0,
        nState_AskCancel,
        nState_Finished
    };
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CBakeContextExport( Dune::CProject& project, CBakeRequestDispatcher& dispatcher, const SBakeConfiguration& config );
    virtual ~CBakeContextExport();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    Void    Start( );
    Void    Cancel();
    Bool    Update();
    //@}

private:

    // Friendship
    friend class CBakeRequestDispatcher;

    // Types
	class CProcessThread : public Sb::CThread
	{
	public:
		CProcessThread( CBakeContextExport& bakeContext );
		Void Entry();

		CBakeContextExport* BakeContext;
	};

	typedef Sb::CMap< Sb::CString, Sb::CRef<Sb::CBank> > BankMap;

	// Methods 
    CBakeRequestDispatcher&     GetDispatcher();
    Bool                        IsProcessingFinished() const;
    Bool                        IsCancelAsked() const;
    Void                        ThreadEntry( Void** args, UInt32 argsCount );
    Void                        ProcessRequestQueue();
    Bool                        PrepareOutputDirectory();
    Void                        DeleteWholeFolder( const QString& str );

    // Attributes
	CBakeRequestDispatcher&         m_Dispatcher;
    Sb::CBitArray32					m_State;
    CProcessThread					m_Thread;
	Sb::CBankLibrary				m_BankLibrary;

};

};

#endif