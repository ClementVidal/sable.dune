#ifndef _DUNE_MODEL_BANK_HANDLE_
#define _DUNE_MODEL_BANK_HANDLE_

#include <Dune/Common/Uuid.h>

namespace Sable
{
    class CPersistentArchive;
}

namespace Dune
{

class CBankObject;
class CProject;

/**
Handle used to reference Asset within the Repository
*/ 
class CHandle
{

public:

    friend class CBank;

    /** @name Constructor/Destructor*/
    //@{
    CHandle();
    CHandle( const CUuid& r );
	CHandle( const CUuid& r, Int32 index, Int32 spin );
    //@}

	Bool			IsValid() const;

    Int32			GetIndex() const;
    Int32			GetSpin() const;
	const CUuid&	GetBankUuid() const;

    Void			SetNull();

    Bool operator== ( const CHandle& h ) const ;
    Bool operator!= ( const CHandle& h ) const ;
    Bool operator< ( const CHandle& h ) const ;

    Bool Serialize( Sb::CPersistentArchive& ar );

private:

	// Methods
	Void SetIndex( Int32 i );
	Void SetSpin( Int32 i );

	// Attributes
    Int32		m_Index;
    Int32		m_Spin;

    CUuid       m_Bank;
};



};

#endif
