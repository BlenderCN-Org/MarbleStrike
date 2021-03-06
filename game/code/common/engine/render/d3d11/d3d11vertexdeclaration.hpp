#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_VERTEXDECLARATION_HPP
#define D3D11_VERTEXDECLARATION_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexdeclaration.hpp"

#include <d3d11.h>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11VertexDeclaration : public VertexDeclaration
{
public:

	D3D11VertexDeclaration();
	~D3D11VertexDeclaration();

	void CreateVertexDeclaration( EffectPass* effectPass );
	void SetVertexElement( 
		D3D11_INPUT_ELEMENT_DESC &vertexElement, 
		LPCSTR semanticName,
		UINT semanticIndex,
		DXGI_FORMAT format,
		UINT inputSlot,
		UINT alignedByteOffset,
		D3D11_INPUT_CLASSIFICATION inputSlotClass,
		UINT instanceDataStepRate
		);
	void Dispatch( void* context );

private:

	ID3D11InputLayout* mVertexLayout;	

};

#endif

#endif