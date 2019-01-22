//#include "UE4CookTestEditor.h"
#include "AlexAssetActions.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
#include "SSCSEditor.h"
#include "ContentBrowserModule.h"
#include "ObjectTools.h"
#include "Editor/UnrealEd/Classes/ThumbnailRendering/SceneThumbnailInfo.h"
#include "FbxMeshUtils.h"
#include "Developer/SourceControl/Public/SourceControlHelpers.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "FbxBuilderImporter.h"
#include "FbxBuilderFactory.h"
#include "IContentBrowserSingleton.h"

#define LOCTEXT_NAMESPACE "MeshAssetTypeActions"

static TAutoConsoleVariable<int32> CVarEnableSaveGeneratedLODsInPackage(
	TEXT("r.StaticMesh.EnableSaveGeneratedLODsInPackage"),
	0,
	TEXT("Enables saving generated LODs in the Package.\n") \
	TEXT("0 - Do not save (and hide this menu option) [default].\n") \
	TEXT("1 - Enable this option and save the LODs in the Package.\n"),
	ECVF_Default);

bool FAlexAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FAlexAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& menuBuilder)
{
	auto Meshes = GetTypedWeakObjectPtrs<UStaticMesh>(InObjects);

	if (CVarEnableSaveGeneratedLODsInPackage.GetValueOnGameThread() != 0)
	{
		menuBuilder.AddMenuEntry(
			NSLOCTEXT("AssetTypeActions_StaticMesh", "ObjectContext_SaveGeneratedLODsInPackage", "Save Generated LODs"),
			NSLOCTEXT("AssetTypeActions_StaticMesh", "ObjectContext_SaveGeneratedLODsInPackageTooltip", "Run the mesh reduce and save the generated LODs as part of the package."),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FAlexAssetActions::ExecuteSaveGeneratedLODsInPackage, Meshes),
				FCanExecuteAction()
			)
		);
	}
	menuBuilder.AddMenuEntry(FText::FromString("Combine Meshs"),
		FText::FromString("Make sure you select meshs you want to combine"),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.ViewOptions"),
		FUIAction(FExecuteAction::CreateRaw(this, &FAlexAssetActions::CombineClicked),
			FCanExecuteAction())
	);
	/*
	menuBuilder.AddMenuEntry(FText::FromString("Reimport Combined Mesh"),
		FText::FromString("Make sure you select one of the combined meshs "),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.ViewOptions"),
		FUIAction(FExecuteAction::CreateRaw(this, &FAlexAssetActions::ReimportClicked),
			FCanExecuteAction())

	);
*/
	menuBuilder.AddSubMenu(
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_LODMenu", "Level Of Detail"),
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_LODTooltip", "LOD Options and Tools"),
		FNewMenuDelegate::CreateSP(this, &FAlexAssetActions::GetLODMenu, Meshes),
		false,
		FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.AssetActions")
	);
	menuBuilder.AddMenuEntry(
		NSLOCTEXT("AssetTypeActions_StaticMesh", "ObjectContext_ClearVertexColors", "Remove Vertex Colors"),
		NSLOCTEXT("AssetTypeActions_StaticMesh", "ObjectContext_ClearVertexColors", "Removes vertex colors from all LODS in all selected meshes."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateSP(this, &FAlexAssetActions::ExecuteRemoveVertexColors, Meshes)
		)
	);
}

UThumbnailInfo* FAlexAssetActions::GetThumbnailInfo(UObject* Asset) const
{
	UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(Asset);
	UThumbnailInfo* ThumbnailInfo = StaticMesh->ThumbnailInfo;
	if (ThumbnailInfo == NULL)
	{
		ThumbnailInfo = NewObject<USceneThumbnailInfo>(StaticMesh, NAME_None, RF_Transactional);
		StaticMesh->ThumbnailInfo = ThumbnailInfo;
	}

	return ThumbnailInfo;
}


uint32 FAlexAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

FText FAlexAssetActions::GetName() const
{
	return FText::FromString(TEXT("Alex StaticMesh"));
}

UClass* FAlexAssetActions::GetSupportedClass() const
{
	return UStaticMesh::StaticClass();
}

FColor FAlexAssetActions::GetTypeColor() const
{
	return FColor::Emerald;
}

void FAlexAssetActions::ReimportClicked()//会出现问题，先不用
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
//	UStaticMesh* myObj = NewObject<UStaticMesh>();
	TArray<UObject*> Objs;
	FText title_text1;
	FText Message1;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		if (SelectedAssets[i].GetClass() == UStaticMesh::StaticClass())
		{//Cast<UStaticMesh>
		//	myObj = (SelectedAssets[i].GetAsset());
			Objs.Add(SelectedAssets[i].GetAsset());
		}
	}


	if (Objs.Num() <= 0)// || (LoadedObjects.Num() <= 0))
	{
		title_text1 = FText::FromString("NO Found mesh");
		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);

		return;
	}

//	for (UObject *OBj : Objs)
//	{	
	UFbxBuilderFactory * FbxFac= NewObject<UFbxBuilderFactory>();
//	FbxFac->AddToRoot();



		UObject * OBj = Objs[0];
		UStaticMesh * Mesh = Cast<UStaticMesh>(OBj);
		UFbxBuilderStaticMeshImportData *FbxAssetImportData = Cast<UFbxBuilderStaticMeshImportData>(Mesh->AssetImportData);
	//	FbxAssetImportData = UFbxBuilderStaticMeshImportData::GetImportDataForStaticMesh(Mesh, FbxFac->ImportUI->StaticMeshImportData);
		if (!FbxAssetImportData)
		{

			FbxAssetImportData = NewObject<UFbxBuilderStaticMeshImportData>(Mesh, NAME_None, RF_NoFlags, Mesh->AssetImportData);
			if (Mesh->AssetImportData != NULL)
			{
				FbxAssetImportData->SourceData = Mesh->AssetImportData->SourceData;
			}
			Mesh->AssetImportData = FbxAssetImportData;
		}
//		Message1 = FText::FromString(FString::Printf(*FString("FileName=%s\n"), *Mesh->GetName()));
//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		if (FbxAssetImportData != nullptr && FbxAssetImportData->bImportAsScene)
		{
			//This mesh was import with a scene import, we cannot reimport it
			return ;
		}
		const FString OutFilename = FbxAssetImportData->GetFirstFilename();

		Message1 = FText::FromString(FString::Printf(*FString("FileName=%s\n"), *OutFilename));
		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
	//	UFbxBuilderStaticMeshImportData* ImportData = UFbxBuilderStaticMeshImportData::GetImportDataForStaticMesh(Mesh, ImportUI->StaticMeshImportData);

		
	//	UFbxBuilderStaticMeshImportData* ImportData = Cast<UFbxBuilderStaticMeshImportData>(Mesh->AssetImportData);





		UnFbxB::FFbxBuilderImporter* FFbxImporter = UnFbxB::FFbxBuilderImporter::GetInstance();
		UnFbxB::FBXImportOptions* ImportOptions = FFbxImporter->GetImportOptions();
	//			Message1 = FText::FromString("1");
	//			FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);

		//Pop the message log in case of error
//		UnFbxB::FFbxLoggerSetter Logger(FFbxImporter, true);

		//Clean up the options
		UnFbxB::FBXImportOptions::ResetOptions(ImportOptions);


		UFbxBuilderImportUI* ReimportUI = NewObject<UFbxBuilderImportUI>();
	//	ReimportUI->MeshTypeToImport = FBXIT_StaticMesh;
		ReimportUI->bOverrideFullName = false;
	//	ReimportUI->StaticMeshImportData->bCombineMeshes = false ;//false;
//		Message1 = FText::FromString("2");
//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);


//UFbxBuilderImportUI *


		if (!FbxFac->ImportUI)
		{
				FbxFac->ImportUI = NewObject<UFbxBuilderImportUI>(FbxFac, NAME_None, RF_Public);
				Message1 = FText::FromString("3");
				FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		}

		UFbxBuilderStaticMeshImportData* ImportData = UFbxBuilderStaticMeshImportData::GetImportDataForStaticMesh(Mesh, FbxFac->ImportUI->StaticMeshImportData);
	//	UFbxBuilderStaticMeshImportData* ImportData = Cast<UFbxBuilderStaticMeshImportData>(Mesh->AssetImportData);
	//	Message1 = FText::FromString("4");
	//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);

		ImportData->UpdateFilenameOnly(OutFilename);

	//	Message1 = FText::FromString("5");
	//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);


		const bool ShowImportDialogAtReimport = GetDefault<UEditorPerProjectUserSettings>()->bShowImportDialogAtReimport && !GIsAutomationTesting;

	//	Message1 = FText::FromString("6");
	//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);


		bool bOperationCanceled = FbxFac->bOperationCanceled;// true;
		if (ImportData && !ShowImportDialogAtReimport)
		{
			// Import data already exists, apply it to the fbx import options
			ReimportUI->StaticMeshImportData = ImportData;
	//		ReimportUI->bResetMaterialSlots = false;
			
			ApplyImportUIToImportOptions(ReimportUI, *ImportOptions);


	//		Message1 = FText::FromString("7");
	//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);

		}
		else
		{
			if (ImportData == nullptr)
			{
				// An existing import data object was not found, make one here and show the options dialog
				ImportData = UFbxBuilderStaticMeshImportData::GetImportDataForStaticMesh(Mesh, FbxFac->ImportUI->StaticMeshImportData);
				Mesh->AssetImportData = ImportData;
				return;
			}
			ReimportUI->bIsReimport = true;
			ReimportUI->StaticMeshImportData = ImportData;

			bool bImportOperationCanceled = false;
			bool bForceImportType = true;
			bool bShowOptionDialog = true;
			bool bOutImportAll = false;
			bool bIsObjFormat = false;
			bool bIsAutomated = false;

			GetImportOptions(FFbxImporter, ReimportUI, bShowOptionDialog, bIsAutomated, Mesh->GetPathName(), bOperationCanceled, bOutImportAll, bIsObjFormat, bForceImportType, FBXBuilderIT_StaticMesh, Mesh);
	
			Message1 = FText::FromString("8");
			FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);

		}

		//We do not touch bAutoComputeLodDistances when we re-import, setting it to true will make sure we do not change anything.
		//We set the LODDistance only when the value is false.
		ImportOptions->bAutoComputeLodDistances = true;
		ImportOptions->LodNumber = 0;
		ImportOptions->MinimumLodNumber = 0;

		if (!bOperationCanceled && ensure(ImportData))
		{
			const FString Filename = ImportData->GetFirstFilename();
			const FString FileExtension = FPaths::GetExtension(Filename);
			const bool bIsValidFile = FileExtension.Equals(TEXT("fbx"), ESearchCase::IgnoreCase) || FileExtension.Equals("obj", ESearchCase::IgnoreCase);

			if (!bIsValidFile)
			{
				return;
			}

			if (!(Filename.Len()))
			{
				// Since this is a new system most static meshes don't have paths, so logging has been commented out
				//UE_LOG(LogEditorFactories, Warning, TEXT("-- cannot reimport: static mesh resource does not have path stored."));
				return;
			}

		//	UE_LOG(LogEditorFactories, Log, TEXT("Performing atomic reimport of [%s]"), *Filename);

			// Ensure that the file provided by the path exists
			if (IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
			{
		//		UE_LOG(LogEditorFactories, Warning, TEXT("-- cannot reimport: source file cannot be found."));
				return;
			}

			FString CurrentFilename = Filename;
			bool bImportSucceed = true;
			if (FFbxImporter->ImportFromFile(*Filename, FPaths::GetExtension(Filename), true))
			{
				FFbxImporter->ApplyTransformSettingsToFbxNode(FFbxImporter->Scene->GetRootNode(), ImportData);

	//			Message1 = FText::FromString("9");
	//			FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);

				const TArray<UAssetUserData*>* UserData = Mesh->GetAssetUserDataArray();
				TArray<UAssetUserData*> UserDataCopy;
				if (UserData)
				{
					for (int32 Idx = 0; Idx < UserData->Num(); Idx++)
					{
						if ((*UserData)[Idx] != nullptr)
						{
							Message1 = FText::FromString("10");
							FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);	

							UserDataCopy.Add((UAssetUserData*)StaticDuplicateObject((*UserData)[Idx], GetTransientPackage()));


						}
					}
				}

				// preserve settings in navcollision subobject
				UNavCollision* NavCollision = Mesh->NavCollision ?
					(UNavCollision*)StaticDuplicateObject(OBj, GetTransientPackage()) :
					nullptr;
		//		Message1 = FText::FromString("10.1");
		//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
				// preserve extended bound settings
				const FVector PositiveBoundsExtension = Mesh->PositiveBoundsExtension;
				const FVector NegativeBoundsExtension = Mesh->NegativeBoundsExtension;
		//		Message1 = FText::FromString("10.2");
		//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
				if (FFbxImporter->ReimportStaticMesh(Mesh, ImportData))//崩溃跳出
				{
			//		UE_LOG(LogEditorFactories, Log, TEXT("-- imported successfully"));

					Message1 = FText::FromString("11");
					FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
					// Copy user data to newly created mesh
					for (int32 Idx = 0; Idx < UserDataCopy.Num(); Idx++)
					{
						UserDataCopy[Idx]->Rename(nullptr, Mesh, REN_DontCreateRedirectors | REN_DoNotDirty);
						Mesh->AddAssetUserData(UserDataCopy[Idx]);
					}

					if (NavCollision)
					{
						Mesh->NavCollision = NavCollision;
				//		NavCollision->Rename(NULL, Mesh, REN_DontCreateRedirectors | REN_DoNotDirty);
					}

					// Restore bounds extension settings
					Mesh->PositiveBoundsExtension = PositiveBoundsExtension;
					Mesh->NegativeBoundsExtension = NegativeBoundsExtension;

					Mesh->AssetImportData->Update(Filename);

					// Try to find the outer package so we can dirty it up
					if (Mesh->GetOuter())
					{
						Mesh->GetOuter()->MarkPackageDirty();

						Message1 = FText::FromString("12");
						FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
					}
					else
					{
						Mesh->MarkPackageDirty();
						Message1 = FText::FromString("13");
						FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
					}

					FFbxImporter->ImportStaticMeshGlobalSockets(Mesh);
				}
				else
				{
			//		UE_LOG(LogEditorFactories, Warning, TEXT("-- import failed"));
					bImportSucceed = false;
				}
			}
			else
			{
			//	UE_LOG(LogEditorFactories, Warning, TEXT("-- import failed"));
				bImportSucceed = false;
			}

			FFbxImporter->ReleaseScene();

			return;
		}
		else
		{
			FFbxImporter->ReleaseScene();
			return;
		}

//		FbxFac->RemoveFromRoot();




//	}

}



void FAlexAssetActions::CombineClicked()
{

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	UBlueprint* Loaded = NewObject<UBlueprint>();
	UBlueprint* LoadedObject;
	UStaticMesh* myObj = NewObject<UStaticMesh>();
	TArray<UStaticMesh*> Objs;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	FText title_text1;
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		if (SelectedAssets[i].GetClass() == UStaticMesh::StaticClass())
		{
			myObj = Cast<UStaticMesh>(SelectedAssets[i].GetAsset());
			Objs.Add(myObj);
		}
	}


	if (Objs.Num() <= 0)// || (LoadedObjects.Num() <= 0))
	{
		title_text1 = FText::FromString("NO Found mesh");
		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);

		return;
	}
	int32 position = -1;
	
	FString BodyName = Objs[0]->GetOutermost()->GetName();
	position = BodyName.Find("/", ESearchCase::CaseSensitive, ESearchDir::FromEnd);


	FString PackagPath;
	PackagPath = BodyName.Left(position);
//	title_text1 = FText::FromString(PackagPath);
//	FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);

	TArray<UObject*> NewObjects;
	TArray<UObject*> AssetsIn;

	UObject *Asset = StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/load/ActorBase.ActorBase"));

	//复制第一步原地复制
	AssetsIn.Add(Asset);
	ObjectTools::DuplicateObjects(AssetsIn, TEXT(""), "/load", /*bOpenDialog=*/false, &NewObjects);

	// If any objects were duplicated, report the success
	if (NewObjects.Num())
	{
		for (int32 ObjectIndex = 0; ObjectIndex < AssetsIn.Num(); ObjectIndex++)
		{
			UObject* SourceAsset = AssetsIn[ObjectIndex];
			UObject* DestAsset = NewObjects[ObjectIndex];
			SourceControlHelpers::BranchPackage(DestAsset->GetOutermost(), SourceAsset->GetOutermost());
		}
	}
	else
	{
		return;
	}

	//改名第二步

	position = -1;
	UStaticMesh * Mesh1 = Objs[0];
	BodyName = Mesh1->GetName();
	position=BodyName.Find("_", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	FString BasePackageName1;
	if ((position >= 0) && (position <= BodyName.Len()))
	{

		BasePackageName1 = BodyName.Left(position);
	}
	else
	{
		BasePackageName1 = BodyName;

	}




	UObject *Result = NewObjects[0];
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	TArray<FAssetRenameData> AssetsAndNames;
	const FString PackagePath = FPackageName::GetLongPackagePath(Result->GetOutermost()->GetName());
	new(AssetsAndNames) FAssetRenameData(Result, PackagePath, BasePackageName1);
	AssetToolsModule.Get().RenameAssets(AssetsAndNames);

//	title_text1 = FText::FromString(BasePackageName1);
//	FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);






	//复制第三步转移
	FString BasePackageName2 = "/load/" + BasePackageName1 + "." + BasePackageName1;
	UObject *Asset2 = StaticLoadObject(UObject::StaticClass(), NULL, *BasePackageName2);

	TArray<UObject*> AssetsIn2;
	TArray<UObject*> NewObjects2;

	AssetsIn2.Add(Asset2);

	ObjectTools::DuplicateObjects(AssetsIn2, TEXT(""), PackagPath, /*bOpenDialog=*/false, &NewObjects2);

	if (NewObjects2.Num())
	{
		// Now branch the files in source control if possible
		for (int32 ObjectIndex = 0; ObjectIndex < AssetsIn2.Num(); ObjectIndex++)
		{
			UObject* SourceAsset = AssetsIn2[ObjectIndex];
			UObject* DestAsset = NewObjects2[ObjectIndex];
			SourceControlHelpers::BranchPackage(DestAsset->GetOutermost(), SourceAsset->GetOutermost());
		}
	}
	else
	{
		return;
	}

	//第四部删除


	ObjectTools::DeleteObjects(AssetsIn2,false);




	UObject *Result2 = NewObjects2[0];


	LoadedObject = Cast<UBlueprint>(Result2);
	if (LoadedObject == nullptr)
	{
		title_text1 = FText::FromString("NO Found blueprint");
		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);
		return;

	}


	for (int i = 0; i < Objs.Num(); i++)
	{
		UStaticMesh * ExistingMat = Objs[i];
		FString x = ExistingMat->GetName();
		const FName NewStaticMeshName = *x;
		USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), NewStaticMeshName);
		
		LoadedObject->SimpleConstructionScript->GetDefaultSceneRootNode()->AddChildNode(NewNode, true);
		if (NewNode->GetVariableName() != NAME_None)
		{
			FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->GetVariableName());
		}
		UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
		myStaticK->SetStaticMesh(ExistingMat);
	}

}

void FAlexAssetActions::GetLODMenu(class FMenuBuilder& MenuBuilder, TArray<TWeakObjectPtr<UStaticMesh>> Meshes)
{

	MenuBuilder.AddSubMenu(
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_ImportLOD", "Import LOD"),
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_ImportLODtooltip", "Imports meshes into the LODs"),
		FNewMenuDelegate::CreateSP(this, &FAlexAssetActions::GetImportLODMenu, Meshes)
	);

	MenuBuilder.AddMenuSeparator();

	MenuBuilder.AddMenuEntry(
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_CopyLOD", "Copy LOD"),
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_CopyLODTooltip", "Copies the LOD settings from the selected mesh."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateSP(this, &FAlexAssetActions::ExecuteCopyLODSettings, Meshes),
			FCanExecuteAction::CreateSP(this, &FAlexAssetActions::CanCopyLODSettings, Meshes)
		)
	);

	FText PasteLabel = FText(LOCTEXT("StaticMesh_PasteLOD", "Paste LOD"));
	if (LODCopyMesh.IsValid())
	{
		PasteLabel = FText::Format(LOCTEXT("StaticMesh_PasteLODWithName", "Paste LOD from {0}"), FText::FromString(LODCopyMesh->GetName()));
	}

	MenuBuilder.AddMenuEntry(
		PasteLabel,
		NSLOCTEXT("AssetTypeActions_StaticMesh", "StaticMesh_PasteLODToltip", "Pastes LOD settings to the selected mesh(es)."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateSP(this, &FAlexAssetActions::ExecutePasteLODSettings, Meshes),
			FCanExecuteAction::CreateSP(this, &FAlexAssetActions::CanPasteLODSettings, Meshes)
		)
	);


}



void FAlexAssetActions::GetImportLODMenu(class FMenuBuilder& MenuBuilder, TArray<TWeakObjectPtr<UStaticMesh>> Objects)
{
	check(Objects.Num() > 0);
	auto First = Objects[0];
	UStaticMesh* StaticMesh = First.Get();

	for (int32 LOD = 1;LOD <= First->GetNumLODs();++LOD)
	{
		FText LODText = FText::AsNumber(LOD);
		FText Description = FText::Format(NSLOCTEXT("AssetTypeActions_StaticMesh", "Reimport LOD (number)", "Reimport LOD {0}"), LODText);
		FText ToolTip = NSLOCTEXT("AssetTypeActions_StaticMesh", "ReimportTip", "Reimport over existing LOD");
		if (LOD == First->GetNumLODs())
		{
			Description = FText::Format(NSLOCTEXT("AssetTypeActions_StaticMesh", "LOD (number)", "LOD {0}"), LODText);
			ToolTip = NSLOCTEXT("AssetTypeActions_StaticMesh", "NewImportTip", "Import new LOD");
		}

		MenuBuilder.AddMenuEntry(Description, ToolTip, FSlateIcon(),
			FUIAction(FExecuteAction::CreateStatic(&FAlexAssetActions::ExecuteImportMeshLOD, static_cast<UObject*>(StaticMesh), LOD)));
	}
}

void FAlexAssetActions::ExecuteImportMeshLOD(UObject* Mesh, int32 LOD)
{
	FbxMeshUtils::ImportMeshLODDialog(Mesh, LOD);
}

void FAlexAssetActions::ExecuteCopyLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects)
{
	LODCopyMesh = Objects[0];
}

bool FAlexAssetActions::CanCopyLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects) const
{
	return Objects.Num() == 1;
}

void FAlexAssetActions::ExecutePasteLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects)
{
	if (!LODCopyMesh.IsValid())
	{
		return;
	}

	// Retrieve LOD settings from source mesh
	struct FLODSettings
	{
		FMeshReductionSettings		ReductionSettings;
		float						ScreenSize;
	};

	TArray<FLODSettings> LODSettings;
	LODSettings.AddZeroed(LODCopyMesh->SourceModels.Num());
	for (int32 i = 0; i < LODCopyMesh->SourceModels.Num(); i++)
	{
		LODSettings[i].ReductionSettings = LODCopyMesh->SourceModels[i].ReductionSettings;
		LODSettings[i].ScreenSize = LODCopyMesh->SourceModels[i].ScreenSize;
	}

	const bool bAutoComputeLODScreenSize = LODCopyMesh->bAutoComputeLODScreenSize;

	// Copy LOD settings over to selected objects in content browser (meshes)
	for (TWeakObjectPtr<UStaticMesh> MeshPtr : Objects)
	{
		if (MeshPtr.IsValid())
		{
			UStaticMesh* Mesh = MeshPtr.Get();

			const int32 LODCount = LODSettings.Num();
			if (Mesh->SourceModels.Num() > LODCount)
			{
				const int32 NumToRemove = Mesh->SourceModels.Num() - LODCount;
				Mesh->SourceModels.RemoveAt(LODCount, NumToRemove);
			}

			while (Mesh->SourceModels.Num() < LODCount)
			{
				new(Mesh->SourceModels) FStaticMeshSourceModel();
			}

			for (int32 i = 0; i < LODCount; i++)
			{
				Mesh->SourceModels[i].ReductionSettings = LODSettings[i].ReductionSettings;
				Mesh->SourceModels[i].ScreenSize = LODSettings[i].ScreenSize;
			}

			for (int32 i = LODCount; i < LODSettings.Num(); ++i)
			{
				FStaticMeshSourceModel& SrcModel = Mesh->SourceModels[i];

				SrcModel.ReductionSettings = LODSettings[i].ReductionSettings;
				SrcModel.ScreenSize = LODSettings[i].ScreenSize;
			}

			Mesh->bAutoComputeLODScreenSize = bAutoComputeLODScreenSize;

			Mesh->PostEditChange();
			Mesh->MarkPackageDirty();
		}
	}

}

bool FAlexAssetActions::CanPasteLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects) const
{
	return LODCopyMesh.IsValid();
}



void FAlexAssetActions::ExecuteSaveGeneratedLODsInPackage(TArray<TWeakObjectPtr<UStaticMesh>> Objects)
{
	for (auto StaticMeshIt = Objects.CreateConstIterator(); StaticMeshIt; ++StaticMeshIt)
	{
		auto StaticMesh = (*StaticMeshIt).Get();
		if (StaticMesh)
		{
			StaticMesh->GenerateLodsInPackage();
		}
	}
}

void FAlexAssetActions::ExecuteRemoveVertexColors(TArray<TWeakObjectPtr<UStaticMesh>> Objects)
{
	FText WarningMessage = LOCTEXT("Warning_RemoveVertexColors", "Are you sure you want to remove vertex colors from all selected meshes?  There is no undo available.");
	if (FMessageDialog::Open(EAppMsgType::YesNo, WarningMessage) == EAppReturnType::Yes)
	{
		FScopedSlowTask SlowTask(1.0f, LOCTEXT("RemovingVertexColors", "Removing Vertex Colors"));
		for (auto StaticMeshPtr : Objects)
		{
			bool bRemovedVertexColors = false;
			UStaticMesh* Mesh = StaticMeshPtr.Get();
			if (Mesh)
			{
				Mesh->RemoveVertexColors();
			}
		}
	}
}

void FAlexAssetActions::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
	for (auto& Asset : TypeAssets)
	{
		const auto StaticMesh = CastChecked<UStaticMesh>(Asset);
		StaticMesh->AssetImportData->ExtractFilenames(OutSourceFilePaths);
	}
}

#undef LOCTEXT_NAMESPACE