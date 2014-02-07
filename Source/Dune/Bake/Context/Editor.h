#ifndef _DUNE_BAKE_CONTEXT_EDITOR_
#define _DUNE_BAKE_CONTEXT_EDITOR_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Bake/Context/Context.h>

namespace Dune
{

/**
\ingroup Bake
Global supervisor for baking process
*/
class CBakeContextEditor : public CBakeContext
{

    DEFINE_MANAGED_CLASS( CBakeContextEditor )

public:

    /** @name DataTypes */
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CBakeContextEditor( CProject& proj, const SBakeConfiguration& config );
    virtual ~CBakeContextEditor();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
	CBakeObjectSet* BakeObject( const CHandle& h, Bool bakeDependency, Bool forcePrepare = FALSE );
	Void			Clear();
    //@}

private:

    // Friendship

    // Types

    // Methods 

    // Attributes	

};

};

#endif