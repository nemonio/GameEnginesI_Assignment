#pragma once
#include "Game.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <iostream>
#include "FountainEffect.h"
#include "PhysicsCamera.h"


using namespace std;

namespace BGE
{
	class TomasRuizPeninAssignment :
		public Game
	{
	private:		
		shared_ptr<PhysicsController> Wheel1;
		shared_ptr<PhysicsController> Wheel2;
		shared_ptr<PhysicsController> Center;
	public:
		TomasRuizPeninAssignment(void);
		~TomasRuizPeninAssignment(void);
		bool Initialise();
		void Update();
		void CreateSceneryObjects();

		shared_ptr<PhysicsController> CreateLivingMolecule(glm::vec3 position, float scale = 5);



	};
}

