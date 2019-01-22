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

	//模型名字
	FString MeshName;

	//文件名
	FString MeshFileName;

	//保存后新模型名
	FString SaveMeshName;

	//子模型相对位置
	FVector rotPivot;

	//模型的Rotator
	FVector Rota;

	//模型的Location
	FVector Locat;

	//模型的Scale
	FVector Scal;

	//父节点名字
	FString ParentName;

	//存放的模型
	UStaticMesh * OwnerMesh;

	//父节点模型
//	UStaticMesh * ParentMesh;

	//存放的节点
//	TSharedPtr<class USCS_Node> OwnNode;
	USCS_Node* OwnNode;
	//存放的Component
//	UStaticMeshComponent* OwnComponent;
	
	class UAlexOBJ * ParentNode;

	fbxsdk::FbxNode * FBXNode;
};