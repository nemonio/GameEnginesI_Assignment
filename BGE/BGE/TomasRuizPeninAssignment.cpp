#include "TomasRuizPeninAssignment.h"

using namespace BGE;

TomasRuizPeninAssignment::TomasRuizPeninAssignment(void) :Game()
{
}

TomasRuizPeninAssignment::~TomasRuizPeninAssignment(void)
{
}

bool TomasRuizPeninAssignment::Initialise()
{	
	//Number of living molecules to instantiate. No more than 25.
	const int numberOfLivingMolecules = 10;

	//set a harder gravity to -200 in the y axis
	dynamicsWorld->setGravity(btVector3(0,-200,0));
	//create a Camera with capsule rigidbody and the floor with rigid body
	physicsFactory->CreateCameraPhysics();
	physicsFactory->CreateGroundPhysics();

	shared_ptr<PhysicsController> MySetOfLivingMolecules[numberOfLivingMolecules];
	shared_ptr<GameComponent> LivingMolecules[numberOfLivingMolecules];

	//Create a number of animats
	for (int i = 0; i<numberOfLivingMolecules; i++)
	{
		//Different behaviours as they have slightly different position, mass, and spin velocity.
		
		MySetOfLivingMolecules[i] = CreateLivingMolecule(glm::vec3(10 * i, 30, 0), 1 + 0.1*i);;
		//LivingMolecules[i] = CreateLivingMolecule(glm::vec3(10 * i, 30, 0), 1 + 0.1*i);;
	}


	
	CreateSceneryObjects();


	
	return Game::Initialise();
}

void TomasRuizPeninAssignment::Update()
{
	Game::Update();
}

//	
void TomasRuizPeninAssignment::CreateSceneryObjects()
{
	const int XSizeRigidBodiesMatrix = 10;
	const int YSizeRigidBodiesMatrix = 10;
	const float distanceBetweenElements = 30;
	const float cubeSize = 9;

	//the bigger the more random
	const float randomnessFactor = 8;

	//Create a matrix of kinematic random size and orientation cubes so the animats interact with some scenery
	for (int x = 0; x<XSizeRigidBodiesMatrix; x++)
	{
		//Different behaviours as they have slightly different position, mass, and spin velocity.
		for (int y = 0; y < XSizeRigidBodiesMatrix; y++)
		{
			physicsFactory->CreateBox(RandomClamped(cubeSize / randomnessFactor, cubeSize), RandomClamped(cubeSize / randomnessFactor, cubeSize), RandomClamped(cubeSize / randomnessFactor, cubeSize), glm::vec3(distanceBetweenElements * x - 150, cubeSize / 2, distanceBetweenElements * y - 150), glm::angleAxis(RandomClamped(0, 90), glm::vec3(0, 0, 1)), true);

		}
	}


}

//The smallest ball is always kept protected in the inside of the molecule
shared_ptr<PhysicsController> TomasRuizPeninAssignment::CreateLivingMolecule(glm::vec3 position, float UNITS)
{
	float RADIUS = UNITS / 2;
	float DISTANCE = glm::abs((glm::sqrt(2 * (UNITS * UNITS)) - UNITS) / 2);


	//Fixed joint
	shared_ptr<PhysicsController> fixedToMainBall = physicsFactory->CreateSphere(RADIUS, position + glm::vec3(0, UNITS + RADIUS, UNITS), glm::quat());
	shared_ptr<PhysicsController> mainBall = physicsFactory->CreateSphere(UNITS*2, position, glm::quat());

	btTransform t1, t2;
	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(0, -(RADIUS), -(RADIUS)));
	t2.setOrigin(btVector3(0, UNITS - 2, UNITS - 2));
	btFixedConstraint * main_fixed = new btFixedConstraint(*mainBall->rigidBody, *fixedToMainBall->rigidBody, t1, t2);


	//Side hinge Joints
	glm::vec3 fixed_left_hinge_pos = position + glm::vec3(((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), ((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), 0);
	glm::vec3 fixed_right_hinge_pos = position + glm::vec3(-((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), ((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), 0);
	shared_ptr<PhysicsController> left_wing = physicsFactory->CreateSphere(UNITS * 2, fixed_left_hinge_pos + glm::vec3((UNITS + DISTANCE), 0, 0), glm::quat());
	shared_ptr<PhysicsController> right_wing = physicsFactory->CreateSphere(UNITS * 2, fixed_right_hinge_pos + glm::vec3(-(UNITS + DISTANCE), 0, 0), glm::quat());

	btHingeConstraint * left_hinge = new btHingeConstraint(*fixedToMainBall->rigidBody, *left_wing->rigidBody, GLToBtVector(glm::vec3(((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), ((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), 0)), GLToBtVector(glm::vec3(-(UNITS + DISTANCE), 0, 0)), btVector3(0, 0, 1), btVector3(0, 0, 1));
	btHingeConstraint * right_hinge = new btHingeConstraint(*fixedToMainBall->rigidBody, *right_wing->rigidBody, GLToBtVector(glm::vec3(-((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), ((UNITS + DISTANCE) * (1 / glm::sqrt(2.0))), 0)), GLToBtVector(glm::vec3((UNITS + DISTANCE), 0, 0)), btVector3(0, 0, 1), btVector3(0, 0, 1));


	//set limits to 90 degrees in every axis
	left_hinge->setLimit(M_PI, M_PI, M_PI);
	right_hinge->setLimit(M_PI, M_PI, M_PI);

	//2 Motors to spin the hinge joints. Depending on size.
	left_hinge->enableAngularMotor(true, -UNITS+5, UNITS+5);
	right_hinge->enableAngularMotor(true, UNITS+5, UNITS+5);

	//Add the joints to the physics world
	dynamicsWorld->addConstraint(main_fixed);

	dynamicsWorld->addConstraint(left_hinge);
	dynamicsWorld->addConstraint(right_hinge);

	//return the biggest rigid body
	return fixedToMainBall;


}
