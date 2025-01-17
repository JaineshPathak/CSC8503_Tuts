#include "PositionConstraint.h"
//#include "../../Common/Vector3.h"
#include "GameObject.h"
#include "PhysicsObject.h"
//#include "Debug.h"



using namespace NCL;
using namespace Maths;
using namespace CSC8503;

PositionConstraint::PositionConstraint(GameObject* a, GameObject* b, float d)
{
	objectA		= a;
	objectB		= b;
	distance	= d;
}

PositionConstraint::~PositionConstraint()
{

}

//a simple constraint that stops objects from being more than <distance> away
//from each other...this would be all we need to simulate a rope, or a ragdoll
void PositionConstraint::UpdateConstraint(float dt)	
{
	Vector3 relativePos = objectA->GetTransform().GetPosition() - objectB->GetTransform().GetPosition();
	float currentDistance = relativePos.Length();
	float offset = distance - currentDistance;

	if (abs(offset) > 0.0f)
	{
		Vector3 offsetDir = relativePos.Normalised();		//v

		PhysicsObject* physA = objectA->GetPhysicsObject();
		PhysicsObject* physB = objectB->GetPhysicsObject();

		Vector3 relativeVelocity = physA->GetLinearVelocity() - physB->GetLinearVelocity();		//j

		float constraintMass = physA->GetInverseMass() + physB->GetInverseMass();

		if (constraintMass > 0.0f)
		{
			float velocityDot = Vector3::Dot(relativeVelocity, offsetDir);		//j . v
			float biasFactor = 0.02f;
			float bias = -(biasFactor / dt) * offset;

			float lambda = -(velocityDot + bias) / constraintMass;

			Vector3 aImpulse = offsetDir * lambda;
			Vector3 bImpulse = -offsetDir * lambda;

			physA->ApplyLinearImpulse(aImpulse);
			physB->ApplyLinearImpulse(bImpulse);
		}

		//Rotation
		Matrix4 temp = Matrix4::BuildViewMatrix(objectA->GetTransform().GetPosition(), objectB->GetTransform().GetPosition(), Vector3(0, 1, 0));
		Matrix4 modelMat = temp.Inverse();
		objectA->GetTransform().SetOrientation(Quaternion(modelMat));
		objectB->GetTransform().SetOrientation(Quaternion(modelMat));
	}
}
