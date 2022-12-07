#pragma once
#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

namespace NCL
{
	namespace CSC8503
	{
		class CWLevelManager
		{
		public:
			CWLevelManager(GameWorld& world, GameTechRenderer& renderer);
			~CWLevelManager();

			//void Update(float dt);

		protected:

			void InitAssets();
			void InitGoatWorld();

			void InitBaseObjects();
			void InitSideWalks();
			void InitBuildings();

			void AddCube(const Vector3& cubePos, const Vector3& cubeSize, const Vector3& cubeRot, const float& cubeMass, const Vector4& cubeColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const std::string& cubeName = "DefaultCube", TextureBase* cubeTex = nullptr);
			void AddBuilding(const Vector3& buildingPos, const Vector3& buildingSize, const Vector3& buildingRot, const float& buildingMass, MeshGeometry* buildingMesh, TextureBase* buildingTex = nullptr);

			MeshGeometry* cubeMesh = nullptr;

			//Buildings
			MeshGeometry* highRise3 = nullptr;
			MeshGeometry* highRise9 = nullptr;
			MeshGeometry* highRise12 = nullptr;
			MeshGeometry* highRise13 = nullptr;
			MeshGeometry* highRise18 = nullptr;

			TextureBase* basicTex = nullptr;
			TextureBase* whiteTex = nullptr;
			TextureBase* conceptTex = nullptr;
			ShaderBase* basicShader = nullptr;

			GameWorld& world;
			GameTechRenderer& renderer;
		};
	}
}