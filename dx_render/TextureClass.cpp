////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"
#include <DDSTextureLoader.h>

using namespace DirectX;

// The class constructor will initialize the texture shader resource pointer to null.
TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

// Initialize takes in the Direct3D deviceand file name of the texture and then loads the texture file into the shader resource variable called m_texture.
// The texture can now be used to render with.
bool TextureClass::Initialize(ID3D11Device* device, const wchar_t* filename)
{
	HRESULT result;
	// ID3D11Resource* texture;

	// Load the texture in.
	result = CreateDDSTextureFromFile(device, filename,nullptr, &m_texture);
	//result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

// The Shutdown function releases the texture resource if it has been loaded and then sets the pointer to null.
void TextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

// GetTexture is the function that is called by other objects that need access to the texture shader resource so that they can use the texture for rendering.
ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}