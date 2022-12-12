#pragma once
#include <iostream>

//Including these below gives loads of errors
#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

#include "CWLevelManager.h"
#include "CWGoatPlayer.h"
#include "CWFollowCamera.h"

#include "NavigationGrid.h"
#include "NavigationPath.h"

namespace NCL 
{
	namespace CSC8503
	{
		class CWGoatGame
		{
		public:
			CWGoatGame();
			~CWGoatGame();

			virtual void UpdateGame(float dt);

			bool GetCursorStatus() const { return toggleCamera; }
			CWGoatPlayer* GetPlayer() { return player; }

			void OnPropSpawn() { totalPropsToDestroy++; }
			void OnPropDestroy(const int& propScore) { totalPropsToDestroy--; }

			MeshGeometry* GetGoatMesh() { return levelManager->GetGoatMesh(); }
			MeshGeometry* GetDudeMesh() { return levelManager->GetDudeMesh(); }
			MeshGeometry* GetEnemyMesh() { return levelManager->GetEnemyMesh(); }

			ShaderBase* GetBasicShader() { return levelManager->GetBasicShader(); }
			TextureBase* GetWhiteTex() { return levelManager->GetWhiteTex(); }

			NavigationGrid* GetNavGrid() const { return navGrid; }
			void AddPawnToList(CWPawn* pawn) { pawnsList.emplace_back(pawn); }

		protected:
			void InitCamera();

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;
			CWGoatPlayer* player;

			CWLevelManager* levelManager;

			Camera* mainCamera;
			Vector3 startCameraPos = Vector3(512.0f, 40.0f, 512.0f);

			CWFollowCamera* cameraFollow;

			std::vector<Vector3> testNodes;
			Vector3 startPathPos;
			Vector3 endPathPos;
			NavigationGrid* navGrid;
			NavigationPath navPath;
			void TestFindPath();
			void DisplayTestFindPath();
			float timer = 0.0f;

			bool useGravity = true;
			bool toggleCamera = false;
			bool toggleCursor = false;

			//Debug Mode
			GameObject* selectedObject;

			int totalPropsToDestroy;

			std::vector<CWPawn*> pawnsList;
		};
	}
}