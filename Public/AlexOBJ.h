#pragma once
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include <fbxsdk.h>

class UAlexOBJ
{
public:
	UAlexOBJ()
	{
		MeshName="";
		SaveMeshName = "";
		MeshFileName = "";
		rotPivot = FVector(0.0f, 0.0f, 0.0f);
		Rota = FVector(0.0f, 0.0f, 0.0f);
		ParentName="";
		OwnNode = nullptr;
	//	OwnComponent = nullptr;
		ParentNode = nullptr;
		OwnerMesh = nullptr;
		FBXNode = nullptr;
	};
	~UAlexOBJ()
	{
	};

	//ģ������
	FString MeshName;

	//�ļ���
	FString MeshFileName;

	//�������ģ����
	FString SaveMeshName;

	//��ģ�����λ��
	FVector rotPivot;

	//ģ�͵�Rotator
	FVector Rota;

	//ģ�͵�Location
	FVector Locat;

	//ģ�͵�Scale
	FVector Scal;

	//���ڵ�����
	FString ParentName;

	//��ŵ�ģ��
	UStaticMesh * OwnerMesh;

	//���ڵ�ģ��
//	UStaticMesh * ParentMesh;

	//��ŵĽڵ�
//	TSharedPtr<class USCS_Node> OwnNode;
	USCS_Node* OwnNode;
	//��ŵ�Component
//	UStaticMeshComponent* OwnComponent;
	
	class UAlexOBJ * ParentNode;

	fbxsdk::FbxNode * FBXNode;
};