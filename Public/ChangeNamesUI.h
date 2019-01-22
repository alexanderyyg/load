// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

/**
 * Fbx Importer UI options.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "SlateBasics.h"
#include "UnrealEd.h"
#include "Internationalization.h"
#include "CoreUObject.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
//#include "Factories/ImportSettings.h"


#include "ChangeNamesUI.generated.h"
//此文件虽然可以传递的变量几乎都被注释掉了，但可以作为框架为未来可能需要扩充的变量做准备
/** Import mesh type */

//#define DECLARE_DELEGATE( DelegateName ) FUNC_DECLARE_DELEGATE( DelegateName, void )//in 
//DECLARE_DELEGATE(FOnPreviewFbxImport);

UCLASS(config=EditorPerProjectUserSettings, AutoExpandCategories=(FTransform), HideCategories=Object, MinimalAPI)
class UChangeNamesUI : public UObject//, public IImportSettingsParser
{
public:	
	//GENERATED_UCLASS_BODY()
	GENERATED_BODY()



	UChangeNamesUI(const FObjectInitializer& ObjectInitializer)//在load.cpp中定义
	{}
//	UChangeNamesUI(const FObjectInitializer& ObjectInitializer);
//	UChangeNamesUI(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	bool bIsObjImport;
	/*
	UPROPERTY()
	TEnumAsByte<enum EFBXBuilderImportTypeB> OriginalImportTypeB;

	UPROPERTY()
	TEnumAsByte<enum EFBXBuilderImportTypeB> MeshTypeToImportB;
*/
	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=Miscellaneous, meta=(OBJRestrict="true"))
	uint32 bOverrideFullName:1;

//	UPROPERTY(EditAnywhere, Category = Mesh, meta = (ImportType = "StaticMesh|SkeletalMesh", DisplayName="Skeletal Mesh"))
//	bool bImportAsSkeletal;
	

	UPROPERTY(EditAnywhere, Category=Mesh, meta=(ImportType="SkeletalMesh"))
	bool bImportMesh;


	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=Mesh, meta=(ImportType="SkeletalMesh"))
	uint32 bCreatePhysicsAsset:1;


	UPROPERTY(EditAnywhere, config, Category = LodSettings, meta = (ImportType = "StaticMesh"))
	uint32 bAutoComputeLodDistances : 1;

	UPROPERTY(EditAnywhere, config, Category = LodSettings, meta = (ImportType = "StaticMesh", UIMin = "0.0"))
	float LodDistance0;
	/** Specify the LOD distance for LOD 1*/

	/** Override for the name of the animation to import. By default, it will be the name of FBX **/
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Animation, meta=(editcondition="bImportAnimations", ImportType="SkeletalMesh")) 
	FString OverrideAnimationName;


	/** Import data used when importing static meshes */
//	UPROPERTY(EditAnywhere, Transient, Instanced, Category = Mesh, meta=(ImportType = "StaticMesh"))
//	class UFbxBuilderStaticMeshImportData* StaticMeshImportData;

	/** Import data used when importing skeletal meshes */
//	UPROPERTY(EditAnywhere, Transient, Instanced, Category=Mesh, meta=(ImportType = "SkeletalMesh"))
//	class UFbxBuilderSkeletalMeshImportData* SkeletalMeshImportData;

	/** Import data used when importing animations */
//	UPROPERTY(EditAnywhere, Transient, Instanced, Category=Animation, meta=(editcondition="bImportAnimations", ImportType = "Animation"))
//	class UFbxBuilderAnimSequenceImportData* AnimSequenceImportData;

	/** Import data used when importing textures */
//	UPROPERTY(EditAnywhere, Transient, Instanced, Category=Material)
//	class UFbxBuilderTextureImportData* TextureImportData;

	/** If true the automated import path should detect the import type.  If false the import type was specified by the user */

	/** If true the existing material array will be reset by the incoming fbx file. The matching "material import name" will be restore properly but, the entries that has no match will use the material instance of the existing data at the same index. (Never enable this option if you have gameplay code that use a material slot)*/
//	UPROPERTY(EditAnywhere, config, AdvancedDisplay, Category = Material, meta = (OBJRestrict = "true", ImportType = "Mesh"))
//	uint32 bResetMaterialSlots : 1;


	/** UObject Interface */
	/*
	virtual bool CanEditChange(const UProperty* InProperty) const override
	{
		bool bIsMutable = Super::CanEditChange(InProperty);
		if (bIsMutable && InProperty != NULL)
		{
			FName PropName = InProperty->GetFName();

			if (PropName == TEXT("StartFrame") || PropName == TEXT("EndFrame"))
			{
				bIsMutable = AnimSequenceImportData->AnimationLength == FBXBuilderALIT_SetRange && bImportAnimations;
			}
			else if (PropName == TEXT("bImportCustomAttribute") || PropName == TEXT("AnimationLength"))
			{
				bIsMutable = bImportAnimations;
			}

			if (bIsObjImport && InProperty->GetBoolMetaData(TEXT("OBJRestrict")))
			{
				bIsMutable = false;
			}
		}

		return bIsMutable;
	}
	*/

	/* Whether this UI is construct for a reimport */
	bool bIsReimport;
};


