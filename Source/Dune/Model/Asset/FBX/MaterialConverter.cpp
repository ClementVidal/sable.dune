#include <Dune/Model/Asset/FBX/MaterialConverter.h>


using namespace Dune;

CFBXMaterialConverter::CFBXMaterialConverter( /*domMaterial& daeMaterial */ )
{
	
	Parse();
}

CFBXMaterialConverter::~CFBXMaterialConverter()
{
}

const CFBXMaterialConverter::STextureInfo& CFBXMaterialConverter::GetDiffuseTexture() const
{
	return m_DiffuseTexture;
}

String CFBXMaterialConverter::GetName() const
{
	return NULL;
}

Void CFBXMaterialConverter::Parse()
{
	/*
	domInstance_effect* inst_effect = m_DomMaterial->getInstance_effect();

	domEffect* effect = (domEffect*)inst_effect->getUrl().getElement().cast();
	domFx_profile_abstract_Array& profiles = effect->getFx_profile_abstract_array();

	if( profiles.getCount() == 1 )
	{
		domFx_profile_abstract* profile = profiles[0];
		if( profile->getElementType() == COLLADA_TYPE::PROFILE_COMMON )
		{
			domProfile_COMMON* profileCommon = (domProfile_COMMON*) profile;
			domCommon_newparam_type_Array& paramArray = profileCommon->getNewparam_array();

			// Process Blinn technique
			domProfile_COMMON::domTechnique::domBlinn* blinnTechnique = profileCommon->getTechnique()->getBlinn();

			if( blinnTechnique )
			{
				domCommon_color_or_texture_type::domTexture* texture;
				
				texture = blinnTechnique->getDiffuse()->getTexture();
				if( texture )
					ExtractTextureInfo( m_DiffuseTexture, *texture, paramArray );

				texture = blinnTechnique->getSpecular()->getTexture();
				if( texture )
					ExtractTextureInfo( m_SpecularTexture, *texture, paramArray );
			}

			// Process phong technique
			domProfile_COMMON::domTechnique::domPhong* phongTechnique = profileCommon->getTechnique()->getPhong();

			if( phongTechnique )
			{
				domCommon_color_or_texture_type::domTexture* texture;

				texture = phongTechnique->getDiffuse()->getTexture();
				if( texture )
					ExtractTextureInfo( m_DiffuseTexture, *texture, paramArray );

				texture = phongTechnique->getSpecular()->getTexture();
				if( texture )
					ExtractTextureInfo( m_SpecularTexture, *texture, paramArray );
			}
		}
	}
	*/
}