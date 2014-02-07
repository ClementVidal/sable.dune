#ifndef _DUNE_APPLICATION_SETTINGS_
#define _DUNE_APPLICATION_SETTINGS_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/StaticString.h>
#include <Sable/Core/Collection/List.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>



namespace Dune
{

class CProject;

/** 
Application settings internal storage
*/
class CApplicationSettings : public Sb::CManagedObject
{

public:

    /** @name DataTypes*/
    //@{
    typedef Sb::CList< Sb::CStaticString<128> > RecentProjectList;
    //@}
    
    /** @name Constructor/Destructor*/
    //@{
    CApplicationSettings( );
    virtual ~CApplicationSettings();
    //@}

    /** @name Manipulator*/
    //@{
	String					GetInternalBakePath() const;
    const RecentProjectList& GetRecentProjectList() const;
    //@}

    /** @name Manipulator*/
    //@{
    Void AddRecentProject( CProject& p );
    Bool Save( );
    Bool Load( );
    //@}

private:

    // Attributes
    RecentProjectList  m_RecentProject;
	Sb::CStaticString<128>	m_InternalBakePath;


};

}

#endif
