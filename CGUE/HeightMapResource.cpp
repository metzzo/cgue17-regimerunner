#include "HeightMapResource.h"
#include "glm/glm.hpp"
#include "ModelResource.h"


namespace Engine {
	HeightMapResource::HeightMapResource(string filename, vec3 size) : RenderableResource(filename)
	{
		this->heightMap = new TextureResource(filename);
		this->mesh = nullptr;
		this->size = size;
	}

	HeightMapResource::~HeightMapResource()
	{
	}

	void HeightMapResource::Load()
	{
		this->heightMap->Init();
		this->pixels = static_cast<GLubyte*>(heightMap->GetPixels());

		vector< vector< glm::vec3> > vertexData(heightMap->GetHeight(), vector<glm::vec3>(heightMap->GetWidth()));
		vector< vector< glm::vec2> > coordsData(heightMap->GetHeight(), vector<glm::vec2>(heightMap->GetWidth()));

		auto texU = float(heightMap->GetWidth()) * 0.1f;
		auto texV = float(heightMap->GetHeight()) * 0.1f;
		for (auto y = 0; y < heightMap->GetHeight(); y++)
		{
			for (auto x = 0; x < heightMap->GetWidth(); x++)
			{
				auto scaleC = float(x) / float(heightMap->GetWidth() - 1);
				auto scaleR = float(y) / float(heightMap->GetHeight() - 1);


				auto pixel = pixels[heightMap->GetBytesPerPixel()*(y*heightMap->GetWidth() + x)];
				auto height = float(pixel) / 255.0f;

				vertexData[y][x] = vec3(scaleC, height, scaleR);
				coordsData[y][x] = vec2(texU*scaleC, texV*scaleR);
			}
		}

		// gen normals
		vector< vector<glm::vec3> > normals[2];
		for (auto i = 0; i < 2; i++)
		{
			normals[i] = vector< vector<glm::vec3> >(heightMap->GetHeight() - 1, vector<glm::vec3>(heightMap->GetWidth() - 1));
		}

		for (auto y = 0;  y < heightMap->GetHeight() - 1; y++)
		{
			for (auto x = 0;  x < heightMap->GetWidth() - 1; x++)
			{
				glm::vec3 tri0[] =
				{
					vertexData[y][x],
					vertexData[y + 1][x],
					vertexData[y + 1][x + 1]
				};
				glm::vec3 tri1[] =
				{
					vertexData[y + 1][x + 1],
					vertexData[y][x + 1],
					vertexData[y][x]
				};

				auto triangleNorm0 = glm::cross(tri0[0] - tri0[1], tri0[1] - tri0[2]);
				auto triangleNorm1 = glm::cross(tri1[0] - tri1[1], tri1[1] - tri1[2]);

				normals[0][y][x] = glm::normalize(triangleNorm0);
				normals[1][y][x] = glm::normalize(triangleNorm1);
			}
		}

		auto finalNormals = vector< vector<glm::vec3> >(heightMap->GetHeight(), vector<glm::vec3>(heightMap->GetWidth()));

		for (auto y = 0; y < heightMap->GetHeight() - 1; y++)
		{
			for (auto x = 0; x < heightMap->GetWidth() - 1; x++)
			{
				auto finalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

				// Look for upper-left triangles
				for (auto k = 0; k < 2 && x != 0 && y != 0; k++) {
					finalNormal += normals[k][y - 1][x - 1];
				}

				// Look for upper-right triangles
				if (y != 0 && x != heightMap->GetWidth() - 1) {
					finalNormal += normals[0][y - 1][x];
				}
				// Look for bottom-right triangles
				for (auto k = 0; k < 2 && y != heightMap->GetHeight() - 1 && x != heightMap->GetWidth() - 1; k++)
				{
					finalNormal += normals[k][y][x];
				}
				// Look for bottom-left triangles
				if (y != heightMap->GetWidth() - 1 && x != 0)
				{
					finalNormal += normals[1][y][x - 1];
				}

				finalNormals[y][x] = glm::normalize(finalNormal);
			}
		}

		this->mesh = new Mesh();
		this->mesh->mode = GL_TRIANGLE_STRIP;

		for (auto y = 0; y < heightMap->GetHeight(); y++)
		{
			for (auto x = 0; x < heightMap->GetWidth(); x++)
			{
				auto pos = vertexData[y][x];
				auto normal = finalNormals[y][x];
				auto coord = coordsData[y][x];

				auto vertex = Vertex();
				vertex.Position = vec3(pos.x * size.x, pos.y*size.y, pos.z*size.z);
				vertex.Normal = normal;
				vertex.TexCoords = coord;
				mesh->vertices.push_back(vertex);

			}
		}

		mesh->restartIndex = heightMap->GetWidth()*heightMap->GetHeight();

		for (auto y = 0; y < heightMap->GetHeight() - 1; y++)
		{
			for (auto x = 0; x < heightMap->GetWidth(); x++)
			{
				for (auto k = 0; k < 2; k++) {
					auto row = y + (1 - k);
					auto index = row*heightMap->GetWidth() + x;
					mesh->indices.push_back(index);
				}
			}
			mesh->indices.push_back(mesh->restartIndex);
		}
		for (auto& tex : this->textures)
		{
			tex->Init();
			mesh->diffuseTexture.push_back(tex);
		}
		mesh->Init();
		this->meshes.push_back(mesh);
	}

	void HeightMapResource::AddTexture(TextureResource* tex)
	{
		this->textures.push_back(tex);
	}

	vec3 HeightMapResource::GetSize() const
	{
		return size;
	}

	float HeightMapResource::GetHeightAt(int x, int z) const
	{
		auto xPos = int(float(x) / size.x * heightMap->GetWidth());
		auto zPos = int(float(z) / size.z * heightMap->GetHeight());
		return float(pixels[heightMap->GetBytesPerPixel()*(zPos*heightMap->GetWidth() + xPos)])/255.0f * size.y;
	}

	TextureResource* HeightMapResource::GetHeightMap() const
	{
		return heightMap;
	}
}