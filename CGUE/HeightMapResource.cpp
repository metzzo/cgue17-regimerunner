#include "HeightMapResource.h"
#include "glm/glm.hpp"
#include "ModelResource.h"


namespace Engine {
	HeightMapResource::HeightMapResource(string filename, vec3 size, int texWidth, int texHeight) : RenderableResource(filename)
	{
		this->heightMap = new TextureResource(filename);
		this->mesh = nullptr;
		this->size = size;
		this->pixels = nullptr;
		this->texWidth = texWidth;
		this->texHeight = texHeight;
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

		for (auto y = 0; y < heightMap->GetHeight(); y++)
		{
			for (auto x = 0; x < heightMap->GetWidth(); x++)
			{
				auto scaleC = float(x) / float(heightMap->GetWidth() - 1);
				auto scaleR = float(y) / float(heightMap->GetHeight() - 1);


				auto pixel = pixels[heightMap->GetBytesPerPixel()*(x*heightMap->GetHeight() + y)];
				auto height = float(pixel) / 255.0f;

				vertexData[y][x] = vec3(scaleC, height, scaleR);
				coordsData[y][x] = vec2(scaleC, scaleR);
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


		// generate texture
		for (auto& tex : this->textures)
		{
			tex.texture->Init();
		}
		auto texturePixels = new GLubyte[3 * texWidth * texHeight];
		for (auto x = 0; x < texWidth; x++)
		{
			for (auto y = 0;  y < texHeight; y++)
			{
				auto hX = int(float(x) / texWidth*heightMap->GetWidth());
				auto hY = int(float(y) / texHeight*heightMap->GetHeight());

				auto pixel = float(pixels[heightMap->GetBytesPerPixel()*(hX*heightMap->GetHeight() + hY)]) / 255.0f;

				for (auto& tex : this->textures)
				{
					if (tex.min <= pixel && tex.max >= pixel)
					{
						auto origTexX = x % tex.texture->GetWidth();
						auto origTexY = y % tex.texture->GetHeight();
						for (auto i = 0; i < 3; i++)
						{
							texturePixels[3*(x*texHeight + y) + i] = static_cast<GLubyte*>(tex.texture->GetPixels())[3*(origTexX*tex.texture->GetHeight() + origTexY) + i];
						}
					}
				}
			}
		}

		/*for (auto y = 0; y < heightMap->GetHeight(); y++)
		{
			for (auto x = 0; x < heightMap->GetWidth(); x++)
			{
				auto pixel = float(pixels[heightMap->GetBytesPerPixel()*(x*heightMap->GetHeight() + y)])/255.0f;
				for (auto texX = int(float(x) / heightMap->GetWidth() * texWidth); texX < int(float(x + 1) / heightMap->GetWidth() * texWidth); texX++)
				{
					for (auto texY = int(float(y) / heightMap->GetHeight() * texHeight); texY < int(float(y + 1) / heightMap->GetHeight() * texHeight); texY++)
					{
						for (auto& tex : this->textures)
						{
							if (tex.min <= pixel && tex.max >= pixel)
							{
								auto origTexX = texX % tex.texture->GetWidth();
								auto origTexY = texY % tex.texture->GetHeight();
								for (auto i = 0; i < 3; i++)
								{
									texturePixels[texX*heightMap->GetHeight() + texY + i] = static_cast<GLubyte*>(tex.texture->GetPixels())[origTexX*tex.texture->GetWidth() + origTexY + i];
								}
							}
						}
					}
				}
			}
		}*/

		auto terrainTex = new TextureResource(texturePixels, texWidth, texHeight);
		terrainTex->Init();
		mesh->diffuseTexture.push_back(terrainTex);
		mesh->Init();
		this->meshes.push_back(mesh);

		// now build texture
	}

	void HeightMapResource::AddTexture(TextureResource* tex, float min, float max)
	{
		HeightMapTexture hTex;
		hTex.texture = tex;
		hTex.min = min;
		hTex.max = max;
		this->textures.push_back(hTex);
	}

	vec3 HeightMapResource::GetSize() const
	{
		return size;
	}

	float HeightMapResource::GetHeightAt(int x, int z) const
	{
		auto xPos = int(float(x) / size.x * heightMap->GetWidth());
		auto zPos = int(float(z) / size.z * heightMap->GetHeight());
		return float(pixels[heightMap->GetBytesPerPixel()*(xPos*heightMap->GetHeight() + zPos)])/255.0f * size.y;
	}

	TextureResource* HeightMapResource::GetHeightMap() const
	{
		return heightMap;
	}
}
