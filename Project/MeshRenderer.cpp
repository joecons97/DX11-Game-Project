#include "MeshRenderer.h"
#include "Direct3D.h"
#include "GameObject.h"
#include "Camera.h"
#include "DirectionLight.h"
#include "ConstantBuffers.h"
#include "GameObject.h"

MeshRenderer::~MeshRenderer()
{
	delete m_pMesh;
}

void MeshRenderer::UpdateGfx()
{
	TransformationBuffer m_transBuffer;

	XMVECTOR pos = { 
		Camera::GetMain()->GetPosition().x + DirectionLight::GetMainLight()->GetOwner()->GetForward().x * -50,
		Camera::GetMain()->GetPosition().y + DirectionLight::GetMainLight()->GetOwner()->GetForward().y * -50,
		Camera::GetMain()->GetPosition().z + DirectionLight::GetMainLight()->GetOwner()->GetForward().z * -50
	};

	XMVECTOR forward = { DirectionLight::GetMainLight()->GetOwner()->GetForward().x,
		DirectionLight::GetMainLight()->GetOwner()->GetForward().y,
		DirectionLight::GetMainLight()->GetOwner()->GetForward().z };

	XMVECTOR up = { DirectionLight::GetMainLight()->GetOwner()->GetUp().x,
		DirectionLight::GetMainLight()->GetOwner()->GetUp().y,
		DirectionLight::GetMainLight()->GetOwner()->GetUp().z };

	XMMATRIX lightViewMat = XMMatrixLookToLH(pos, forward, up);
	XMMATRIX lightProjMat = XMMatrixOrthographicLH(150, 150, .1, 100);

	m_transBuffer.DirLightView = lightViewMat;
	m_transBuffer.DirLightProjection = lightProjMat;
	m_transBuffer.World = m_owner->GetWorldMatrix();
	m_transBuffer.WorldViewProjection = m_owner->GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix();

	ConstantBuffers::GetInstance()->Bind(BUFFER_TRANSFORMATIONS, &m_transBuffer);
	m_pMesh->Draw();
}

void MeshRenderer::UpdateShadowMap()
{
	TransformationBuffer m_transBuffer;
	m_transBuffer.World = m_owner->GetWorldMatrix();

	XMVECTOR pos = {
		Camera::GetMain()->GetPosition().x + DirectionLight::GetMainLight()->GetOwner()->GetForward().x * -50,
		Camera::GetMain()->GetPosition().y + DirectionLight::GetMainLight()->GetOwner()->GetForward().y * -50,
		Camera::GetMain()->GetPosition().z + DirectionLight::GetMainLight()->GetOwner()->GetForward().z * -50
	};

	XMVECTOR forward = { 
		DirectionLight::GetMainLight()->GetOwner()->GetForward().x,
		DirectionLight::GetMainLight()->GetOwner()->GetForward().y,
		DirectionLight::GetMainLight()->GetOwner()->GetForward().z };

	XMVECTOR up = { 
		DirectionLight::GetMainLight()->GetOwner()->GetUp().x,
		DirectionLight::GetMainLight()->GetOwner()->GetUp().y,
		DirectionLight::GetMainLight()->GetOwner()->GetUp().z };

	XMMATRIX lightViewMat = XMMatrixLookToLH(pos, forward, up);
	XMMATRIX lightProjMat = XMMatrixOrthographicLH(150, 150, .1, 100);
	m_transBuffer.WorldViewProjection = m_owner->GetWorldMatrix() * lightViewMat * lightProjMat;

	m_transBuffer.DirLightView = lightViewMat;
	m_transBuffer.DirLightProjection = lightProjMat;

	ConstantBuffers::GetInstance()->Bind(BUFFER_TRANSFORMATIONS, &m_transBuffer);
	Direct3D::GetInstance()->DrawBasic(m_pMesh);
}
