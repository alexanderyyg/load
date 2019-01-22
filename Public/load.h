// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "ContentBrowserModule.h"
#include "SlateBasics.h"
#include "SlotBase.h"
#include "SPanel.h"
#include "SlateApplication.h"
#include "AssetToolsModule.h"
#include "AssetEditorManager.h"
#include "ClassViewerModule.h"
#include "SClassPickerDialog.h"
#include "AssertionMacros.h"
#include "AssetRegistryModule.h"
#include "SComponentClassCombo.h"
#include "IContentBrowserSingleton.h"
#include "Editor/UnrealEd/Public/Kismet2/DebuggerCommands.h"
#include "KismetEditorUtilities.h"
#include "Runtime/Slate/Public/Framework/Commands/UIAction.h"
#include "Developer/AssetTools/Public/AssetTypeActions_Base.h"

#include "SSCSEditor.h"
#include "ToolkitManager.h"
#include "MultiBoxExtender.h"
#include "MultiBoxBuilder.h"
#include "LevelEditorActions.h"
#include "LevelEditor.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Editor/UnrealEd/Classes/Factories/MaterialFactoryNew.h"

#include "Runtime/Engine/Public/ComponentReregisterContext.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpression.h"
#include "Editor/PackagesDialog/Public/PackagesDialog.h"

#include "Runtime/Engine/Classes/Materials/MaterialExpressionMultiply.h"
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
#include "SWindow.h"
//#include "FbxImportTools.h"
#include "AlexStorage.h"
#include "AlexAssetActions.h"
#include "AlexMaterial.h"
//#include "SViewport.h"
//#include "Internationalization.h"

DECLARE_LOG_CATEGORY_EXTERN(ModuleLog, Log, All);

class FloadModule : public IModuleInterface
{
public:


	virtual TSharedPtr<FExtender> GetMenuExtender();// override;
	void MyButton_Clicked();
	void MyButton_Clicked2();
	void MyButton_Clicked3();

	void MyButton_Clicked5();

	UMaterial* MakeParentMaterial();
	void MakeMaterialInstance(UMaterial* UnrealMaterial);

	TSharedPtr<FUICommandList> MyPluginCommands;
	TSharedPtr<FExtensibilityManager> MyExtensionManager;
	TSharedPtr< const FExtensionBase > MenuExtension;
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
//	virtual IAssetTools& Get() const;

	static inline FloadModule& GetModule()
	{
		static const FName ModuleName = "load";
		return FModuleManager::LoadModuleChecked< FloadModule >(ModuleName);
	}
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialInstanceDynamic *DynamicMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialInstance * Material;
	TArray<UAlexMaterial> UMs;
	TArray<FString> FbxNames;
	UAlexStorage * AlexStorageTool;

protected:
	TSharedPtr<FExtender> MenuExtender;
	TSharedPtr<FExtender> MenuExtender2;
	void AddFbxExtension(FMenuBuilder& MenuBuilder);
	void RegisterMenuExtensions();
	void GenerateToolsMenu(FMenuBarBuilder& MenuBarBuilder);
	void FillAlexMenu(FMenuBuilder& MenuBuilder);
	void CombineMeshWithoutActor();
//	bool RefreshFilteredState(FSCSEditorTreeNodePtrType TreeNode, bool bRecursive);

	void GenerateActorToolsMenu(FMenuBarBuilder& MenuBarBuilder);
	void CombineTool(FMenuBuilder& MenuBuilder);
//	FFbxImportTools* FbxBuilder;
	UTexture2D* tmpTexture;


};



