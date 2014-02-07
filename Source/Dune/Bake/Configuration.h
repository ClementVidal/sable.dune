#ifndef _DUNE_BAKE_CONFIGURATION_
#define _DUNE_BAKE_CONFIGURATION_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/File/FilePath.h>
#include <Sable/Core/Collection/BitArray.h>
#include <Sable/Core/ManagedObject/Ref.h>

namespace Dune
{

class CBankObject;
class CBankObject;

/**
\ingroup Bake
Bake request
*/
class SBakeRequest
{

public:

    /** @name DataTypes */
    //@{
    enum EType
    {
        nType_Process = 0,
        nType_Synch
    };
    //@}

    /** @name Constructor/Destructor*/
    //@{
    SBakeRequest( );
    //@}

    /** @name Operators*/
    //@{
    SBakeRequest& operator=( const SBakeRequest& other );
    //@}

    /** @name Attributes*/
    //@{
    Sb::CRef<CBankObject>    Obj;
    Sb::CBitArray32                   Type;
    //@}
};

/**
Bake configuration
*/
class SBakeConfiguration
{

public:

    /** @name Constructor/Destructor*/
    //@{
    SBakeConfiguration( );
    ~SBakeConfiguration();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Attributes*/
    //@{
    Sb::CFilePath	OutputPath;
    UInt32			    ThreadCount;
	Bool			    OptimizeModels;
	//@}

private:

    // Types

    // Methods 

    // Attributes

};


};

#endif