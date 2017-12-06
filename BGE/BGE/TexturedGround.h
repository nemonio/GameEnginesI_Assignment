#pragma once
#include "GameComponent.h"
#include <vector>

using namespace std;

namespace BGE
{
	class TexturedGround :
		public GameComponent
	{
	public:
		TexturedGround(void);
		~TexturedGround(void);
		
		bool Initialise();
		void Draw();
		void Cleanup();

		std::vector<glm::vec3> vertices; 	
		std::vector<glm::vec2> texels;
		std::vector<glm::vec3> normals;

		bool TexturedGround::rayIntersectsWorldPlane(glm::vec3 origin, glm::vec3 look, glm::vec3 & point);
		float width, height;

	private:
		GLuint vertexbuffer;
		GLuint texelbuffer;
		GLuint programID;
		
		GLuint normalbuffer;
		GLuint colourbuffer;
		GLuint ambientID;
		GLuint specularID;
		GLuint diffusePerVertexID;
		GLuint mID, vID, pID, nID;

		GLuint textureID;
		GLuint textureSampler;
		GLuint m_3x3_inv_transp;
	};
}
