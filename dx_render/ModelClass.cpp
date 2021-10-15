////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

// The class constructor initializes the vertex and index buffer pointers to null.
ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

// The Initialize function will call the initialization functions for the vertex and index buffers.
bool ModelClass::Initialize(ID3D11Device * device, const char* modelFileName, const wchar_t* textureFilename)
{
	bool result;
	std::vector<VertexType> obj_verts;
	std::vector<unsigned long> obj_indices;
	result = LoadOBJ(modelFileName, obj_verts, obj_indices);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	// result = InitializeBuffers(device);
	result = InitializeOBJBuffers(device,obj_verts,obj_indices);
	if (!result)
	{
		return false;
	}
	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}


	return true;
}

// The Shutdown function will call the shutdown functions for the vertex and index buffers.
void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();
	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

// Render is called from the GraphicsClass::Render function.
// This function calls RenderBuffers to put the vertex and index buffers on the graphics pipeline so the color shader will be able to render them.
void ModelClass::Render(ID3D11DeviceContext * deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

// GetIndexCount returns the number of indexes in the model.
// The color shader will need this information to draw this model.
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

// The InitializeBuffers function is where we handle creating the vertexand index buffers.
// Usually you would read in a model and create the buffers from that data file.
// For this tutorial we will just set the points in the vertex and index buffer manually since it is only a single triangle.
bool ModelClass::InitializeBuffers(ID3D11Device * device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	// First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.

	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	
	// Now fill both the vertex and index array with the three points of the triangle as well as the index to each of the points.
	// Please note that I create the points in the clockwise order of drawing them.
	// If you do this counter clockwise it will think the triangle is facing the opposite direction and not draw it due to backface culling.
	// Always remember that the order in which you send your vertices to the GPU is very important.
	// The color is set here as well since it is part of the vertex description. I set the color to green.

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertices[0].texture = { 0.0f, 1.0f };

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	//vertices[1].color = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertices[1].texture = { 0.0f, 0.0f };

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertices[2].texture = { 1.0f, 1.0f };

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
	//vertices[3].color = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertices[3].texture = { 1.0f, 0.0f };

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // Bottom right.
	indices[3] = 1;  // Bottom left.
	indices[4] = 3;  // Bottom left.
	indices[5] = 2;  // Bottom left.
	
	// With the vertex array and index array filled out we can now use those to create the vertex buffer and index buffer.
	// Creating both buffers is done in the same fashion.
	// First, fill out a description of the buffer.
	// In the description the ByteWidth(size of the buffer) and the BindFlags(type of buffer) are what you need to ensure are filled out correctly.
	// After the description is filled out you need to also fill out a subresource pointer which will point to either your vertex or index array you previously created.
	// With the description and subresource pointer you can call CreateBuffer using the D3D device and it will return a pointer to your new buffer.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// After the vertex and index buffers have been created, you can delete the vertex and index arrays as they are no longer needed, since the data was copied into the buffers.

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool ModelClass::InitializeOBJBuffers(ID3D11Device* device, std::vector<VertexType>& obj_verts, std::vector<unsigned long>& obj_indices)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.

	// Set the number of vertices in the vertex array.
	m_vertexCount = obj_verts.size();

	// Set the number of indices in the index array.
	m_indexCount = obj_indices.size();


	// With the vertex array and index array filled out we can now use those to create the vertex buffer and index buffer.
	// Creating both buffers is done in the same fashion.
	// First, fill out a description of the buffer.
	// In the description the ByteWidth(size of the buffer) and the BindFlags(type of buffer) are what you need to ensure are filled out correctly.
	// After the description is filled out you need to also fill out a subresource pointer which will point to either your vertex or index array you previously created.
	// With the description and subresource pointer you can call CreateBuffer using the D3D device and it will return a pointer to your new buffer.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = obj_verts.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = obj_indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// After the vertex and index buffers have been created, you can delete the vertex and index arrays as they are no longer needed, since the data was copied into the buffers.

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	// Not sure if this is needed

	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, const wchar_t* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadOBJ(const char* filename, OUT std::vector<VertexType>& out_verts, OUT std::vector<unsigned long>& obj_indices)
{
	FILE* f = nullptr;
	bool res = fopen_s(&f, filename, "r");
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<XMFLOAT3> verts;
	std::vector<XMFLOAT2> uvs;
	std::vector<XMFLOAT3> norms;
	if (res == 0) {
		printf("The file %s was opened\n",filename);
	}
	else {
		printf("File opening failed for %s\n", filename);
		return false;
	}
	fseek(f, 0L, SEEK_SET);
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(f, "%s", lineHeader, 128);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			XMFLOAT3 vertex;
			fscanf_s(f, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			verts.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			XMFLOAT2 uv;
			fscanf_s(f, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			XMFLOAT3 normal;
			fscanf_s(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			norms.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		VertexType v;
		v.position = verts[vertexIndices[i]-1];
		v.texture = uvs[uvIndices[i]-1];
		v.normal = norms[normalIndices[i]-1];
		out_verts.push_back(v);
	}
	// [NOTE] This could be unnecessary/redundant, we could probably grab them from above - need to see how DX11 handles this sort of thing
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned long vInd;
		//vInd = vertexIndices[i];
		vInd = unsigned long(i);
		obj_indices.push_back(vInd);
	}

	return true;
}

// The ShutdownBuffers function just releases the vertex and index buffers that were created in the InitializeBuffers function.
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// RenderBuffers is called from the Render function.
// The purpose of this function is to set the vertex and index buffers as active on the input assembler in the GPU.
// Once the GPU has an active vertex buffer it can then use the shader to render that buffer.
// This function also defines how those buffers should be drawn such as triangles, lines, fans, and so forth.
// In this tutorial we set the vertex and index buffers as active on the input assembler,
// and tell the GPU that the buffers should be drawn as triangles using the IASetPrimitiveTopology DirectX function.
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}