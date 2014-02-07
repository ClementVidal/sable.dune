#include <Dune/Model/Asset/AudioBuffer.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Bake/ObjectSet.h>
#include <Sable/Audio/Buffer/Header.h>

using namespace Dune;
 
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetAudioBuffer, CAsset  )

String CAssetAudioBuffer::PropFilePath = "FilePath";

CAssetAudioBuffer::CAssetAudioBuffer()
{
	m_FilePath = new CPropertyFilePath( PropFilePath );
	m_FilePath->SetWildcard( "Wave file (*.wav)" );

	AddProperty( *m_FilePath );

	SetName("SoundBuffer");
}

CAssetAudioBuffer::~CAssetAudioBuffer()
{
}

CPropertyFilePath& CAssetAudioBuffer::GetFilePath() const
{
	return (CPropertyFilePath&) *GetProperty(PropFilePath);
}

Void CAssetAudioBuffer::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CAudioBufferWave* buffer = NEWOBJ( Sb::CAudioBufferWave, ( ) );
	objSet.AddObject( *buffer );
}

Void CAssetAudioBuffer::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CAudioBufferWave* buffer = objSet.GetObj<Sb::CAudioBufferWave>( );

	if( buffer )
	{
		buffer->SetFilePath( m_FilePath->GetValue() );
		CAsset::BakeObjectSet( context, objSet );
	}
}
