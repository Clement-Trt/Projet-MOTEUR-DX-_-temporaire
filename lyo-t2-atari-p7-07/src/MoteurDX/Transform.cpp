#include "pch.h"

#include "Transform.h"

Transform::Transform()
{
	Identity();
}

void Transform::UpdateMatrix()
{
	DirectX::XMMATRIX matriceScale = DirectX::XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	DirectX::XMMATRIX matriceRotation = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&qRotation));
	DirectX::XMMATRIX matricePosition = DirectX::XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);

	XMStoreFloat4x4(&mRotation, matriceRotation);

	// ordre important ! (Scale * Rotation * Position)
	DirectX::XMMATRIX newMatrice;

	newMatrice = matriceScale; // ne pas *= directement il faut initialisï¿½Eune valeur de base !
	newMatrice *= matriceRotation;
	newMatrice *= matricePosition;

	DirectX::XMStoreFloat4x4(&matrix, newMatrice);
}

void Transform::Identity()
{
	vPosition = { 0,0,0 };
	vScale = { 1,1,1 };

	qRotation = { 0,0,0,1 };
	mRotation =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	vFront = { 0,0,1 };
	vRight = { 1,0,0 };
	vUp = { 0,1,0 };

	matrix =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}
void Transform::ResetRotation()
{
	DirectX::XMVECTOR defaultForward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	DirectX::XMVECTOR currentForward = DirectX::XMVector3Rotate(defaultForward, XMLoadFloat4(&qRotation));

	// 2. Projetter cette direction sur le plan horizontal (en annulant Y)
	DirectX::XMVECTOR forwardProj = DirectX::XMVectorSet(DirectX::XMVectorGetX(currentForward), DirectX::XMVectorGetY(currentForward), DirectX::XMVectorGetZ(currentForward), 0.f);
	forwardProj = DirectX::XMVector3Normalize(forwardProj);

	// 3. Calculer le yaw ï¿½ partir de forwardProj
	// Note : atan2 prend (y, x) mais ici on utilise (x, z) pour obtenir l'angle par rapport ï¿½ l'axe Z.
	float yaw = atan2f(DirectX::XMVectorGetX(forwardProj), DirectX::XMVectorGetZ(forwardProj));

	// 4. Recrï¿½er un quaternion avec yaw, et avec pitch = roll = 0
	DirectX::XMVECTOR newQuat = DirectX::XMQuaternionRotationRollPitchYaw(0.f, yaw, 0.f);

	float deltaTime = 1; // Utiliser DeltaTime du jeu ?
	const float rotationSpeed = 0.04; 

	// Calcul d'un facteur d'interpolation (entre 0 et 1)
	float t = rotationSpeed * deltaTime;
	if (t > 1.0f) t = 1.0f;

	// Interpolation entre la rotation actuelle et la rotation cible
	//qRotation = DirectX::XMQuaternionSlerp(XMLoadFloat4(&qRotation), newQuat, t);
	DirectX::XMStoreFloat4(&qRotation, DirectX::XMQuaternionSlerp(XMLoadFloat4(&qRotation), newQuat, t));




	// Pour mettre ï¿½ jour la matrice de rotation si nï¿½cessaire:
	DirectX::XMMATRIX mRotation = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&qRotation));

	UpdateMatrix();
}
void Transform::ResetRoll()
{
	// 1. Calculer le vecteur forward actuel normalisé
	DirectX::XMVECTOR forward = DirectX::XMVector3Normalize(
		DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), XMLoadFloat4(&qRotation))
	);

	// 2. Définir le vecteur up du monde (celui qu'on souhaite obtenir, par exemple (0,1,0))
	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	// 3. Calculer le vecteur right (perpendiculaire à worldUp et forward)
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, forward));

	// 4. Recalculer le vecteur up à partir de forward et right
	DirectX::XMVECTOR up = DirectX::XMVector3Cross(forward, right);

	// 5. Construire une matrice de rotation à partir de ces axes
	DirectX::XMMATRIX rotationMat = DirectX::XMMATRIX(right, up, forward, DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f));

	// 6. Convertir cette matrice en quaternion : ce quaternion a le même forward (donc le même yaw et pitch)
	//    mais son roll est forcé à 0 car l'axe up est aligné sur worldUp.
	DirectX::XMVECTOR newQuat = DirectX::XMQuaternionRotationMatrix(rotationMat);

	// Optionnel : si tu souhaites une transition en douceur, interpole avec le quaternion actuel
	float deltaTime = 1.0f; // Remplacer par le delta time réel
	const float rotationSpeed = 0.04f;
	float t = rotationSpeed * deltaTime;
	if (t > 1.0f) t = 1.0f;
	newQuat = DirectX::XMQuaternionSlerp(XMLoadFloat4(&qRotation), newQuat, t);

	DirectX::XMStoreFloat4(&qRotation, newQuat);

	// Mettre à jour la matrice de transformation
	UpdateMatrix();
}


void Transform::Rotation(float roll, float pitch, float yaw)
{
	// quaternion contenant les 3 rotations
	DirectX::XMVECTOR qRotYPR = DirectX::XMQuaternionIdentity();

	// quaternion contenant une seule rotation
	DirectX::XMVECTOR qRotTmp;

	// Creer un quaternion pour chaque rotation (delta)
	qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vFront), roll);
	qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

	qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vRight), pitch);
	qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

	qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vUp), yaw);
	qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

	// Ajouter la rotation delta a la rotation actuelle de lobjet
	DirectX::XMVECTOR newQRotation = DirectX::XMQuaternionMultiply(XMLoadFloat4(&qRotation), qRotYPR);
	newQRotation = DirectX::XMQuaternionNormalize(newQRotation);

	DirectX::XMStoreFloat4(&qRotation, newQRotation);

	// Convertir le quaternion en une matrice de rotation (magique)
	DirectX::XMMATRIX mMatriceRot = DirectX::XMMatrixRotationQuaternion(newQRotation);
	DirectX::XMStoreFloat4x4(&mRotation, mMatriceRot);

	// Mettre a jour les axes de notre objet (3 vecteurs) _11 _23 -> case de la matrice et table float4x4
	vRight = { mRotation._11, mRotation._12, mRotation._13 };
	vUp = { mRotation._21, mRotation._22, mRotation._23 };
	vFront = { mRotation._31, mRotation._32, mRotation._33 };

	UpdateMatrix();
}

void Transform::Move(float dirFront, float dirRight, float dirUp)
{
    // sauvegarde notre ancienne position
    m_oldPosition = vPosition;

    vPosition.z += dirFront * vFront.z;
    vPosition.x += dirFront * vFront.x;
    vPosition.y += dirFront * vFront.y;

	// Deplacement selon le vecteur vRight (droite/gauche)
	vPosition.z += dirRight * vRight.z;
	vPosition.x += dirRight * vRight.x;
	vPosition.y += dirRight * vRight.y;

	// Deplacement selon le vecteur vUp (haut/bas)
	vPosition.z += dirUp * vUp.z;
	vPosition.x += dirUp * vUp.x;
	vPosition.y += dirUp * vUp.y;

	UpdateMatrix();
}

void Transform::Scale(float scaleX, float scaleY, float scaleZ)
{
	vScale = DirectX::XMFLOAT3(scaleX, scaleY, scaleZ);

	UpdateMatrix();
}

void Transform::AddToGlobalPosX(float deltaX)
{
	vPosition.x += deltaX; 
	UpdateMatrix();
}
void Transform::AddToGlobalPosY(float deltaY)
{
	vPosition.y += deltaY; 
	UpdateMatrix();
}
void Transform::AddToGlobalPosZ(float deltaZ)
{
	vPosition.z += deltaZ; 
	UpdateMatrix();
}

//DirectX::XMMATRIX& Transform::GetXMMatrix()
//{
//    DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4(&matrix);
//    return m;
//    // TODO: insï¿½rer une instruction return ici
//}

