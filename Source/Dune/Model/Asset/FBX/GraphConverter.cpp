#include <Dune\Model\Asset\FBX\GraphConverter.h>

using namespace Dune;

CFBXGraphConverter::CFBXGraphConverter( /*domNode& daeNode, Bool fixYUp*/ )
{
}

CFBXGraphConverter::~CFBXGraphConverter()
{
}

Sb::CVector3f CFBXGraphConverter::GetWorldPosition() const
{
/*	const domTranslate_Array& trsArray = m_DomNode->getTranslate_array(); 

	if( trsArray.getCount() )
	{
		domFloat3 trs = trsArray[0]->getValue();

		if( m_FixYUp )
			return Sb::CVector3f( (Float32)trs[0], (Float32)trs[2], (Float32)-trs[1] );

		return Sb::CVector3f( (Float32)trs[0], (Float32)trs[1], (Float32)trs[2] );
	}
*/
	return Sb::CVector3f( 0.0f, 0.0f, 0.0f );
}
