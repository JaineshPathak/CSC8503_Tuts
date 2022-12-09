#include "CWGoatPlayer.h"
#include "CWGoatGame.h"
#include "CollisionDetection.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"
#include "Maths.h"

NCL::CSC8503::CWGoatPlayer::CWGoatPlayer(CWGoatGame& gGame, GameWorld& gWorld, GameTechRenderer& gRenderer) : game(gGame), world(gWorld), renderer(gRenderer)
{
	goatMesh = renderer.LoadMesh("Goat.msh");
	whiteTex = renderer.LoadTexture("WhiteTex.png");
	basicShader = renderer.LoadShader("scene.vert", "scene.frag");

	SphereVolume* volume = new SphereVolume(5.0f);
	SetBoundingVolume((CollisionVolume*)volume);

	transform
		.SetPosition(startPos)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(startRot.x, startRot.y, startRot.z))
		.SetScale(startScale * 2);

	renderObject = new RenderObject(&transform, goatMesh, whiteTex, basicShader);
	renderObject->SetColour(Debug::WHITE);

	physicsObject = new PhysicsObject(&transform, boundingVolume);
	physicsObject->SetInverseMass(mass);
	physicsObject->InitCubeInertia();
	physicsObject->SetLinearDamping(0.3f);
	physicsObject->SetGravityMultiplier(5.0f);
	physicsObject->SetRestitution(0);

	world.AddGameObject(this);
}

NCL::CSC8503::CWGoatPlayer::~CWGoatPlayer()
{
	delete goatMesh;
	delete basicShader;
}

void NCL::CSC8503::CWGoatPlayer::Update(float dt)
{
	Ray groundRay = Ray(transform.GetPosition(), Vector3(0, -1.0f, 0));
	RayCollision groundCollisionData;
	if (world.Raycast(groundRay, groundCollisionData, true, this))
	{
		float distance = (groundCollisionData.collidedAt - transform.GetPosition()).Length();
		isOnGround = (distance <= 6.f);
	}

	renderObject->SetColour(isOnGround ? Debug::BLUE : Debug::RED);
	//physicsObject->SetLinearDamping(isOnGround ? 3.0f : 0.1f);

	if (game.GetCursorStatus())
		return;

	Vector3 linearMovement;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W)) linearMovement.z = -1.0f;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S)) linearMovement.z = 1.0f;

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A)) linearMovement.x = -1.0f;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D)) linearMovement.x = 1.0f;

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && isOnGround) physicsObject->AddForce(Vector3(0, 1, 0) * jumpForce * 50.0f);

	//airControl = isOnGround ? 1.0f : 0.1f;

	linearMovement.y = 0.0f;
	if (linearMovement.Length() >= 0.1f)
	{
		float targetAngle = atan2(-linearMovement.x, -linearMovement.z);
		targetAngle = RadiansToDegrees(targetAngle) + world.GetMainCamera()->GetYaw();

		Quaternion newRot = Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0);
		float finalRotSpeed = isOnGround ? rotationSpeed : rotationSpeed * (airControl * 0.5);
		transform.SetOrientation(Quaternion::Slerp(transform.GetOrientation(), newRot, finalRotSpeed * dt));

		Vector3 moveDir = newRot * Vector3(0, 0, -1.0f);
		physicsObject->AddForce(moveDir.Normalised() * (isOnGround ? moveSpeed : moveSpeed * airControl));
	}
	else if(physicsObject->GetLinearVelocity().Length() > 0.1f && isOnGround)
	{
		Vector3 dampedVel = Vector3::SmoothDamp(physicsObject->GetLinearVelocity(), Vector3(0, 0, 0), velocityRef, velocityDampSmoothness, FLT_MAX, dt);
		physicsObject->SetLinearVelocity(dampedVel);
	}
}