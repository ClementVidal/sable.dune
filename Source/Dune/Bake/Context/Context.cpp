#include <Dune/Bake/Context/Context.h>

#include <Dune/Bake/ObjectSet.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CBakeContext, Sb::CManagedObject )

CBakeContext::CBakeContext( CProject& project, const SBakeConfiguration& config ) :
    m_Project( project ),
    m_Config( config )
{
 
}

CBakeContext::~CBakeContext()
{

}

CBakeObjectSet* CBakeContext::GetObjectSet( const CHandle& h ) 
{
    if( h.IsValid() == FALSE )
        return NULL;

    if( m_ObjectSetTable.HasItem( h ) )
        return &m_ObjectSetTable.GetItem( h );
    
    return NULL;
}

Bool CBakeContext::HasObjectSet( const CHandle& h ) const
{
	return m_ObjectSetTable.HasItem( h );
}

const SBakeConfiguration& CBakeContext::GetConfig() const
{
    return m_Config;
}

CProject& CBakeContext::GetProject()
{
    return m_Project;
}

Sb::CFilePath CBakeContext::GetBankOutputFilePath( const CBank& b )
{
    Char tmp[256];
    Sb::StringSetFormattedText( tmp, 256, "%s/%s.dbnk", GetProject().GetSettings().GetBakeOutputPath().GetBuffer(), b.GetName());
    return Sb::CFilePath( tmp );
}

Sb::CFilePath CBakeContext::GetBankOutputDir( const CBank& b )
{
    Char tmp[256];
    Sb::StringSetFormattedText( tmp, 256, "%s/%s/", GetProject().GetSettings().GetBakeOutputPath().GetBuffer(), b.GetName() );
	
    return Sb::CFilePath( tmp );
}

Void CBakeContext::LogError( const Char* format, ... )
{
	m_CriticalSection.Enter();

	va_list args;
	va_start( args, format );

	Char buffer[1024];
	Sb::StringSetFormattedTextVaList( buffer, 1024, format, args );
	SignalLog( nType_Error, buffer );

	va_end( args ); 

	m_CriticalSection.Leave();
}

Void CBakeContext::LogWarning( const Char* format, ... )
{
	m_CriticalSection.Enter();

	va_list args;
	va_start( args, format );

	Char buffer[1024];
	Sb::StringSetFormattedTextVaList( buffer, 1024, format, args );
	SignalLog( nType_Warning, buffer );

	va_end( args );

	m_CriticalSection.Leave();
}

Void CBakeContext::LogInfo( const Char* format, ... )
{
	m_CriticalSection.Enter();

	va_list args;
	va_start( args, format );

	Char buffer[1024];
	Sb::StringSetFormattedTextVaList( buffer, 1024, format, args );
	SignalLog( nType_Info, buffer );
	va_end( args );

	m_CriticalSection.Leave();
}
