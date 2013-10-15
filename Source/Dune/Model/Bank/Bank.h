#ifndef _DUNE_MODEL_BANK_REPO_
#define _DUNE_MODEL_BANK_REPO_

#include <Sable\Core\Collection\Array.h>
#include <Sable\Core\Collection\Map.h>
#include <Sable\Core\ManagedObject\Ref.h>
#include <Sable\Core\Event\Signal.h>

#include <Dune\Model\Bank\ObjectAccessor.h>
#include <Dune\Common\Uuid.h>

#define CastRepoObject( obj, type ) (*(type*)&(obj))

namespace Sable
{
    class CPersistentArchive;
}

namespace Dune
{

class CHandle;
class CBankObject;
class CPropertyInterface;
class CProject;
class CPropertyHandle;

/***
\ingroup ModelRepository
Object repository
*/
class CBank : public CBankObjectAccessor
{

    DEFINE_MANAGED_CLASS( CBank );

public:

    /** @name DataTypes*/
    //@{
    class CIterator 
    {

    public:

        /** @name Constructor/Destructor*/
        //@{
        CIterator( );
        CIterator( const CBank* bank  );
        //@}

        /** @name Operator*/
        //@{
		CBankObject& operator * () const;
		CBankObject* operator -> () const;
        //@}

        /** @name Accessor*/
        //@{
        Bool    GetIsValid() const;
        CBankObject& GetData() const;
        //@}

        /** @name Manipulator*/
        //@{
        Void    Iterate();
        //@}

    private:

        UInt32 m_Index;
        const CBank* m_Bank;
    };

    typedef CIterator Iterator;
    //@}

    /** @name Constructor/Destructor*/
    //@{
	CBank();
    CBank( CProject& prj, String name = "Default" );
    ~CBank();
    //@}

    /** @name Accessors*/
    //@{
	CBankObject*		GetObj( const CHandle& handle ) const;
	Void				SetName( String name ) ;
    String              GetName() const;
    CProject*           GetProject() const;
    Bool				IsBinded( const CHandle& handle ) const;
	Bool				IsReserved( const CHandle& handle ) const;
    const   CUuid&      GetUuid() const;
    //@}

    /** @name Manipulator*/
    //@{
	Bool				Merge( const CBank& mergedBank, const CHandle& newRoot, Sb::CMap<CHandle,CHandle>& mappingTable );
	Bool				Merge( const CBankObject& mergedObject, const CHandle& newRoot, Sb::CMap<CHandle,CHandle>& mappingTable );
    Bool                Serialize( Sb::CPersistentArchive& ar );

    Void                Clear();

    Bool                RemoveObject( CHandle h, Bool freeHandle = TRUE );
    CBankObject*        AddObject( const Sb::CTypeInfo& type, const CHandle& parent );

	Void				BindObject( const CHandle& h , CBankObject& obj );
	CHandle				ReserveHandle();
	Bool				FreeHandle( const CHandle& h );

	Bool				IssueQuery( String text, Sb::CList<CHandle>& result ) const;
	//@}

	/** @name System*/
	//@{
	Void                StartIteration( Iterator& iterator ) const ;
	//@}

    /** @name Signal*/
    //@{
    Sb::CEventSignal< CBankObject& > SignalObjectAdded;
    Sb::CEventSignal< CBankObject& > SignalObjectRemoved;
    //@}

private:

    // Frienship
    friend class CProject;

	// DataType
    struct SEntry
    {
        SEntry&    operator=( const SEntry& other );
        Sb::CRef<CBankObject>		Object;
        Int32                       Spin;
        Int32                       NextFree;
    };

    // Methods
    void	SetUuid( const CUuid& uuid );
	Bool	Merge( const Sb::CList<CHandle>& objList, const CHandle& newRoot, const CBankObjectAccessor& srcObjAccessor,  Sb::CMap<CHandle,CHandle>& mappingTable );
	Void	RemapHandleProperty( const CPropertyHandle& srcProp, CPropertyHandle& dstProp, const CHandle& newRoot, Sb::CMap<CHandle,CHandle>& mappingTable ) const;

    // Attributes
    Sb::CStaticString<128>      m_Name;
    Sb::CArray<SEntry>          m_Table;
    Int32                       m_FirstFree;
    UInt32                      m_BindedObjectCount;
    UInt32                      m_ReservedObjectCount;
    CUuid                       m_Uuid;

    CProject*                   m_Project;

	// Methods

};

};

#endif