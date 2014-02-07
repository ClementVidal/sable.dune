#ifndef _DUNE_BAKE_REQUESTDISPATCHER_
#define _DUNE_BAKE_REQUESTDISPATCHER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>



namespace Dune
{

class CBank;
class CBankObject;
class CBakeContext;
class CHandle;
class CProject;

/**
\ingroup Bake
Request dispatcher take care to analyse object dependency and to submit bake request in the appropriate order 
into the bake context.
*/
class CBakeRequestDispatcher : public Sb::CManagedObject
{

    DEFINE_MANAGED_CLASS( CBakeRequestDispatcher )

public:

    /** @name DataTypes */
    //@{
	typedef Sb::CList<SBakeRequest > RequestList;
	typedef RequestList::Iterator Iterator;
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CBakeRequestDispatcher( CProject& project);
    virtual ~CBakeRequestDispatcher();
    //@}

    /** @name Accessors*/
    //@{
	CProject&	GetProject() const;
    //@}

    /** @name Manipulator*/
    //@{
	Void			StartIteration( Iterator& iterator ) const ;
    Void            Dispatch( );
    //@}

protected:

    // Methods
    CBakeContext&	GetContext();
	Bool            IsRequestAlreadyAdded( const CHandle& r ) const;
	Void            AddRequest( const SBakeRequest& r );

private:

    // Friendship
    friend class CBakeContextExport;

    // Types

    // Methods 
    Void DispatchObjects( String objectType );
    Void SetContext( CBakeContext& context );

    // Attributes
	CProject*		m_Project;
    CBakeContext*   m_Context;
    RequestList		m_RequestList;
	
};


};

#endif