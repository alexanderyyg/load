#pragma once
#include "DesktopPlatformModule.h"
#include "ContentBrowserModule.h"
#include "Factories/TextureFactory.h"
#include "MainFrame.h"
#include "SNotificationList.h"
//#include "NotificationManager.h"
#include "AssetData.h"
#include "ObjectTools.h"
#include "Engine/Texture2D.h"
#include "IClassTypeActions.h"
#include "FileManagerGeneric.h"
#include "AssetToolsModule.h"
#include "PackageTools.h"
#include "AssetRegistryModule.h"
#include "UnrealEd.h"

#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"

#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Editor/UnrealEd/Classes/Factories/MaterialFactoryNew.h"

#include "Runtime/Engine/Public/ComponentReregisterContext.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpression.h"


#include "Runtime/Engine/Classes/Materials/MaterialExpressionMultiply.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionPower.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionTextureSample.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionAppendVector.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionTextureCoordinate.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionScalarParameter.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionConstant.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionConstant3Vector.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpressionVectorParameter.h"
#include "Editor/UnrealEd/Classes/Factories/MaterialInstanceConstantFactoryNew.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "AlexMaterial.h"



#include "Kismet/BlueprintFunctionLibrary.h"







class UAlexStorage
{
public:
	UAlexStorage()
	{
		MeshAddPath = "/";
		//	MeshAddPath = "/Mesh/";

		PicAddPath = "/";
		//	PicAddPath = "/Texture/";

		MatAddPath = "/";
		//	MatAddPath = "/Material/";
	};
	~UAlexStorage()
	{
	};
	UAlexMaterial Mat;
	FString FillStorage(const FString& StartPath);
	FString OpenFolder;
//	bool bOpened ;
	void AddPicToUAlexMaterial(FString Filename, const FString ExportPath, TArray<UAlexMaterial> *Mats);
	void LoadPic(const FString InputPath,const FString ExportPath, TArray<UAlexMaterial> *Mats, TArray<FString> *Import);
//	LoadPic(const FString InputPath, TArray<UAlexMaterial> *Mats, TArray<FString> *Import)
//	TArray<FString> ImportedFileNames;
//	TArray<FString> *ImportedNames;
	UMaterialInstanceConstant* CreateMaterialInstance(FString MaterialName,FString ExportPath);
	UMaterial* MakeBaseMaterial();

	FString MeshAddPath;
	FString PicAddPath;
	FString MatAddPath;


private:










};