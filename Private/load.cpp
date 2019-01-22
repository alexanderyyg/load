// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "load.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "ObjectTools.h"
#include "Framework/Application/SlateApplication.h"
#include "MainFrame.h"
#include "ImportAndSaveUI.h"
#include "ChangeNamesWindow.h"
#include "Developer/SourceControl/Public/SourceControlHelpers.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "ImportAndSaveWindow.h"
#include "BlueprintEditorModule.h"


//#include "FbxImportTools.h"

DEFINE_LOG_CATEGORY(ModuleLog)

#define LOCTEXT_NAMESPACE "MyModule"


IMPLEMENT_MODULE(FloadModule, load)

void FloadModule::StartupModule()
{



	RegisterMenuExtensions();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//	MenuExtender = MakeShareable(new FExtender);
//	MenuExtension = MenuExtender->AddMenuExtension(
//		"EditMain",
//		EExtensionHook::After,
//		NULL,
//		FMenuExtensionDelegate::CreateRaw(this, &FloadModule::AddFbxExtension));
//	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
//	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
//	MyExtensionManager = LevelEditorModule.GetMenuExtensibilityManager();

}

TSharedPtr<FExtender> FloadModule::GetMenuExtender()
{
	return MenuExtender;
}

void FloadModule::RegisterMenuExtensions()
{


/*//简单追加在Edit后面

	MenuExtender = MakeShareable(new FExtender());
	//After,
	MenuExtension = MenuExtender->AddMenuExtension(
		"EditMain",
		EExtensionHook::After,
		NULL,
		FMenuExtensionDelegate::CreateRaw(this, &FloadModule::AddFbxExtension));
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

*/
/*
	追加在Help菜单前
*/

	MenuExtender = MakeShareable(new FExtender());

	MenuExtension = MenuExtender->AddMenuBarExtension(
		"Help",
		EExtensionHook::Before,
		NULL,
		FMenuBarExtensionDelegate::CreateRaw(this, &FloadModule::GenerateToolsMenu));
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	MyExtensionManager = LevelEditorModule.GetMenuExtensibilityManager();

	/*
	MenuExtension = MenuExtender->AddMenuExtension(
		"Help",
		EExtensionHook::After,
		NULL,
		FMenuExtensionDelegate::CreateRaw(this, &FloadModule::AddFbxExtension));
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	MyExtensionManager = LevelEditorModule.GetMenuExtensibilityManager();
*/

	MenuExtender2 = MakeShareable(new FExtender());

	MenuExtension = MenuExtender2->AddMenuBarExtension(
		"Help",
		EExtensionHook::Before,
		NULL,
		FMenuBarExtensionDelegate::CreateRaw(this, &FloadModule::GenerateActorToolsMenu));
	
	//必须设置load.uplugin中的 "LoadingPhase": "PostEngineInit"
	FBlueprintEditorModule& BPEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule&>("Kismet");
	BPEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender2);
	MyExtensionManager = BPEditorModule.GetMenuExtensibilityManager();


	
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	auto Actions = MakeShareable(new FAlexAssetActions);
	AssetTools.RegisterAssetTypeActions(Actions);







/*
	TSharedPtr< FUICommandInfo > command;


	TSharedPtr<FBindingContext> context = FInputBindingManager::Get().GetContextByName(TEXT("PlayWorld"));
	FText::FromName(context->GetContextName());
	context->GetContextName().ToString();



	UI_COMMAND_Function(
		context.Get(),
		command,
		TEXT("combine Mesh"), // Replace strings by your command name
		TEXT("CommandName"),
		TEXT("CommandName"),
		".CommandName",
		TEXT("CommandName"),
		TEXT("CommandName"),
		EUserInterfaceActionType::None,
		FInputChord(EKeys::H) // Replace by your input key     Gamepad_FaceButton_Bottom
	);
	LevelEditorModule.GetGlobalLevelEditorActions()->MapAction(command,
				FExecuteAction::CreateUObject(this, &FloadModule::MyButton_Clicked2) // Replace by your method binding
			);
			*/
//	FPlayWorldCommands::GlobalPlayWorldActions->MapAction(
//		command,
//		FExecuteAction::CreateUObject(this, &FloadModule::MyButton_Clicked2) // Replace by your method binding
//	);

	
}
void FloadModule::GenerateToolsMenu(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		LOCTEXT("AlexMenu", "AlexTools"),
		LOCTEXT("AlexMenu_ToolTip", "Import the Alex Tools menu"),
		FNewMenuDelegate::CreateRaw(this, &FloadModule::FillAlexMenu),
		"AlexTools");
}

void FloadModule::FillAlexMenu(FMenuBuilder& MenuBuilder)
{


	MenuBuilder.BeginSection("Model Import");
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("ImportMyFbx", "FBX Import"),
			LOCTEXT("ImportMyFbxToolTip", "Imports Fbx"),
			FSlateIcon::FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FloadModule::MyButton_Clicked))
		);
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CombineMesh", "Mesh Combine"),
			LOCTEXT("CombineMeshToolTip", "Combine Mesh"),
			FSlateIcon::FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FloadModule::MyButton_Clicked2))
		);
		MenuBuilder.AddMenuEntry(
			LOCTEXT("ChangeName", "Change Assert Name"),
			LOCTEXT("ChangeNameToolTip", "Change Assert Name"),
			FSlateIcon::FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FloadModule::MyButton_Clicked5))
		);
	}
	MenuBuilder.EndSection();
}



void FloadModule::GenerateActorToolsMenu(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		LOCTEXT("AlexMenu_CombineBox", "AlexTools1"),
		LOCTEXT("AlexMenu_ToolTip1", "Combin Alex Tools menu"),
		FNewMenuDelegate::CreateRaw(this, &FloadModule::CombineTool),
		"AlexTools1");
}

void FloadModule::CombineTool(FMenuBuilder& MenuBuilder)
{


	MenuBuilder.BeginSection("Model Combine");
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CombinMyStaticMesh", "Combine StaticMesh"),
			LOCTEXT("CombinAllStaticMesh", "Combine Mesh"),
			FSlateIcon::FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FloadModule::MyButton_Clicked3))
		);
	}
	MenuBuilder.EndSection();
}






void FloadModule::ShutdownModule()
{
	if (MyExtensionManager.IsValid())
	{
		//FFbxBuilderCommands::Unregister();
		MenuExtender->RemoveExtension(MenuExtension.ToSharedRef());
	//	MenuExtender2->RemoveExtension(MenuExtension.ToSharedRef());
	}
	else
	{
		MyExtensionManager.Reset();
	}
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
void FloadModule::AddFbxExtension(FMenuBuilder& MenuBuilder)
{

	//UE_LOG(LogTemp, Warning, TEXT("FbxBuilder11 IT WORKS!!!!"));


	MenuBuilder.AddMenuEntry(
		LOCTEXT("ImportMyFbx", "FBX Import"),
		LOCTEXT("ImportMyFbxToolTip", "Imports Fbx"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FloadModule::MyButton_Clicked))
	);

}

void FloadModule::MyButton_Clicked()
{
	TSharedPtr<SWindow> ParentWindow;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}
	const float FbxImportWindowWidth = 410.0f;
	const float FbxImportWindowHeight = 550.0f;
	FVector2D FbxImportWindowSize = FVector2D(FbxImportWindowWidth, FbxImportWindowHeight); // Max window size it can get based on current slate

	FSlateRect WorkAreaRect = FSlateApplicationBase::Get().GetPreferredWorkArea();
	FVector2D DisplayTopLeft(WorkAreaRect.Left, WorkAreaRect.Top);
	FVector2D DisplaySize(WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top);

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(DisplayTopLeft.X, DisplayTopLeft.Y);
	FbxImportWindowSize *= ScaleFactor;

	FVector2D WindowPosition = (DisplayTopLeft + (DisplaySize - FbxImportWindowSize) / 2.0f) / ScaleFactor;
//	UImportAndSaveUI* ImportUI = NewObject<UImportAndSaveUI>();
//	ImportUI->MeshTypeToImportB = FBXBuilderIT_AnimationB;
//	ImportUI->OriginalImportTypeB= FBXBuilderIT_SkeletalMeshB;

//	ImportUI->bOverrideFullName = false;
//	ReimportUI->bImportAnimations = true;
//	FBXImportOptions* ImportOptions = FbxImporter->GetImportOptions();

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(NSLOCTEXT("UnrealEd", "FBXImportOpionsTitle", "FBX Import Options"))
		.SizingRule(ESizingRule::Autosized)
		.AutoCenter(EAutoCenter::None)
		.ClientSize(FbxImportWindowSize)
		.ScreenPosition(WindowPosition);
	TSharedPtr<SImportAndSaveWindow> FbxOptionWindow;
	// FBXImportOptions* ImportOptions bForceImportType= new FBXImportOptions();
	 bool bForceImportType = true;
	 EFBXBuilderImportTypeB ImportType = FBXBuilderIT_StaticMeshB;
	 bool bIsObjFormat = false;

//	 TArray<UAlexMaterial> UMs;

//	 AlexStorageTool = new UAlexStorage();
//	 AlexStorageTool->FillStorage("D:\\", &UMs);

	 Window->SetContent
	(
		SAssignNew(FbxOptionWindow, SImportAndSaveWindow)
		.WidgetWindow(Window)
		.ForcedImportType(bForceImportType ? TOptional<EFBXBuilderImportTypeB>(ImportType) : TOptional<EFBXBuilderImportTypeB>())
		.IsObjFormat(bIsObjFormat)
		.MaxWindowHeight(FbxImportWindowHeight)
		.MaxWindowWidth(FbxImportWindowWidth)
	);
//	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
//	FSlateApplication::Get().AddWindow(Window);
//	TSharedPtr<SWindow> TopWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
	FSlateApplication::Get().AddWindowAsNativeChild(Window, ParentWindow.ToSharedRef(), true);
//		.ImportMaterials(&UMs)
//		.ImportUI(ImportUI)




/*
	FbxBuilder = new FFbxImportTools();
	//UE_LOG(LogTemp, Warning, TEXT("FbxBuilder11 BUTTON CLICKED!!!!"));
//	TArray<UObject*> ImportedObjects = FbxBuilder->ImportAssets(TEXT("/Game/Assets/Mesh"));
	FloadModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FloadModule>("load");
	TArray<UObject*> ImportedObjects = AssetToolsModule.Get().ImportAssets(TEXT("/Game/Assets/Mesh"));
*/
}

void FloadModule::MyButton_Clicked2()
{
//	TArray<FString> InDependencies;


	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	UBlueprint* Loaded = NewObject<UBlueprint>();
	UStaticMesh* myObj = NewObject<UStaticMesh>();
	TArray<UStaticMesh*> Objs;
	TArray<UBlueprint*> LoadedObjects;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		if (SelectedAssets[i].GetClass() == UBlueprint::StaticClass())
		{
			Loaded = Cast<UBlueprint>(SelectedAssets[i].GetAsset());
			LoadedObjects.Add(Loaded);
		}

		if (SelectedAssets[i].GetClass() == UStaticMesh::StaticClass())
		{
			myObj = Cast<UStaticMesh>(SelectedAssets[i].GetAsset());
			Objs.Add(myObj);
		}


	}
	if (LoadedObjects.Num() <= 0)
	{
		CombineMeshWithoutActor();
	}
	if (Objs.Num() <= 0)// || (LoadedObjects.Num() <= 0))
		return;

	for (UBlueprint* LoadedObject : LoadedObjects)
	{
		for (int i = 0; i < Objs.Num(); i++)
		{
			//		obj = LoadedObjects[i];
			//		BasePackage = obj->GetOutermost()->GetName();



			//		UStaticMesh * ExistingMat = LoadObject<UStaticMesh>(NULL, *BasePackage, nullptr, LOAD_Quiet | LOAD_NoWarn);
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

	//建立一个材质贴图结构表
//	TArray<UAlexMaterial> UMs;

//	AlexStorageTool = new UAlexStorage();
//	AlexStorageTool->FillStorage("D:\\");

//	FString ObjectPath = "/Game/";

//	FString BasePackageName = "/Game/a.a";

//	UPackage* OldPackage = LoadPackage(nullptr, *BasePackageName, LOAD_None);
//	UStaticMesh * ExistingMat = LoadObject<UStaticMesh>(NULL, *BasePackageName, nullptr, LOAD_Quiet | LOAD_NoWarn);


//	FString BasePackageName1 = "/Game/b.b_C";

//	UPackage* OldPackage1 = LoadPackage(nullptr, *BasePackageName1, LOAD_None);
//	使用蓝图类型方法

//	static ConstructorHelpers::FClassFinder<AActor> UnitSelector(*BasePackageName1);


//	UBlueprintGeneratedClass* BotPawnClass = Cast<UBlueprintGeneratedClass>(UnitSelector.Class);
//	UBlueprint* BotPawnBlueprint = Cast<UBlueprint>(BotPawnClass->ClassGeneratedBy);

//	UBlueprint* BotPawnClass = Cast<UBlueprint>(UnitSelector.Class);
//	UClass* BotPawnBlueprint = Cast<UClass>(BotPawnClass->GeneratedClass);


//	AActor* BotPawnBlueprint = CastChecked<AActor>(BotPawnBlueprint->GetDefaultObject());






	/*
	TSubclassOf<AActor> UnitSelectorClass = UnitSelector.Class;

	if (!UnitSelectorClass)
	{
		FText title_text1 = FText::FromString("Wrong");
		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);
		return;
	}*/
//	AActor* newActor = NewObject<AActor>(UnitSelectorClass);
//	UnitSelectorClass.GetDefaultObject()
/*
	static ConstructorHelpers::FClassFinder<AActor> UnitSelector(*BasePackageName1);
	AActor * newActor = Cast<AActor *>(UnitSelector);
*/
//	USceneComponent* root=(Cast<AActor>(UnitSelectorClass.Get()))->GetRootComponent();
//	USceneComponent* root = (Cast<AActor>(newActor))->GetRootComponent();
//	AActor *acheck = UnitSelectorClass.GetDefaultObject();
/*	USceneComponent* root = acheck->GetRootComponent();
	if (root)
	{

		FText title_text1 = FText::FromString(root->GetName());
		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);
	}
	*/
//	TArray<UActorComponent*> childs=acheck->GetComponentsByClass(UStaticMeshComponent::StaticClass());


//	for (int i = 0; i < childs.Num(); i++)
//	{
//		UStaticMeshComponent * X = Cast<UStaticMeshComponent>(childs[i]);
//		X->SetStaticMesh(ExistingMat);
//	}

//	UMaterial* BaseMaterial= MakeParentMaterial();
//	if (BaseMaterial != nullptr)
//	{
//		MakeMaterialInstance(BaseMaterial);
//	}
	
}

void FloadModule::MyButton_Clicked5()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	TArray<UStaticMesh*> Objs;
	UStaticMesh* myObj = NewObject<UStaticMesh>();
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	/*
	UWorld* world = GEditor->GetEditorWorldContext().World();
	int worldnum = 0;
	for (TActorIterator<AStaticMeshActor> ActorItr(world); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsSelected())
		{
			worldnum++;
			break;
		}
	}
*/
	if ((SelectedAssets.Num() <= 0))// && (worldnum <= 0))
		return;

	TSharedPtr<SWindow> ParentWindow;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}
	const float ChangeWindowWidth = 410.0f;
	const float ChangeWindowHeight = 600.0f;
	FVector2D FbxImportWindowSize = FVector2D(ChangeWindowWidth, ChangeWindowHeight); // Max window size it can get based on current slate

	FSlateRect WorkAreaRect = FSlateApplicationBase::Get().GetPreferredWorkArea();
	FVector2D DisplayTopLeft(WorkAreaRect.Left, WorkAreaRect.Top);
	FVector2D DisplaySize(WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top);

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(DisplayTopLeft.X, DisplayTopLeft.Y);
	FbxImportWindowSize *= ScaleFactor;

	FVector2D WindowPosition = (DisplayTopLeft + (DisplaySize - FbxImportWindowSize) / 2.0f) / ScaleFactor;
	//	UImportAndSaveUI* ImportUI = NewObject<UImportAndSaveUI>();
	//	ImportUI->MeshTypeToImportB = FBXBuilderIT_AnimationB;
	//	ImportUI->OriginalImportTypeB= FBXBuilderIT_SkeletalMeshB;
	EFBXBuilderImportTypeB ImportType = FBXBuilderIT_StaticMeshB;
	//	ImportUI->bOverrideFullName = false;
	//	ReimportUI->bImportAnimations = true;
	//	FBXImportOptions* ImportOptions = FbxImporter->GetImportOptions();

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(NSLOCTEXT("UnrealEd", "GiveMatOpionsTitle", "Change Assets Name Options"))
		.SizingRule(ESizingRule::Autosized)
		.AutoCenter(EAutoCenter::None)
		.ClientSize(FbxImportWindowSize)
		.ScreenPosition(WindowPosition);
	TSharedPtr<ChangeNamesWindow> ChangeOptionWindow;
	// FBXImportOptions* ImportOptions bForceImportType= new FBXImportOptions();
	bool bForceImportType = true;

	bool bIsObjFormat = false;

	//	 TArray<UAlexMaterial> UMs;

	//	 AlexStorageTool = new UAlexStorage();
	//	 AlexStorageTool->FillStorage("D:\\", &UMs);

	Window->SetContent
	(
		SAssignNew(ChangeOptionWindow, ChangeNamesWindow)
		.WidgetWindow(Window)
		.IsObjFormat(bIsObjFormat)
		.MaxWindowHeight(ChangeWindowHeight)
		.MaxWindowWidth(ChangeWindowWidth)
	);
	//	.ForcedImportType(bForceImportType ? TOptional<EFBXBuilderImportTypeB>(ImportType) : TOptional<EFBXBuilderImportTypeB>())
	//	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
	FSlateApplication::Get().AddWindowAsNativeChild(Window, ParentWindow.ToSharedRef(), true);

}


void FloadModule::CombineMeshWithoutActor()
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
	position = BodyName.Find("_", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
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

	//	UObject *Asset3 = StaticLoadObject(UObject::StaticClass(), NULL, *BasePackageName2);
	//	TArray<UObject*> AssetsIn3;
	//	AssetsIn3.Add(Asset3);
	ObjectTools::DeleteObjects(AssetsIn2, false);




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

void FloadModule::MyButton_Clicked3()
{
	//建立一个材质贴图结构表
	//	TArray<UAlexMaterial> UMs;

	//	AlexStorageTool = new UAlexStorage();
	//	AlexStorageTool->FillStorage("D:\\");

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	UBlueprint* LoadedObject = NewObject<UBlueprint>();
	TArray<UStaticMesh*> Objs;
	UStaticMesh* myObj;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		if (SelectedAssets[i].GetClass() == UBlueprint::StaticClass())
		{
			LoadedObject = Cast<UBlueprint>(SelectedAssets[i].GetAsset());
		}
		if (SelectedAssets[i].GetClass() == UStaticMesh::StaticClass())
		{
			myObj = Cast<UStaticMesh>(SelectedAssets[i].GetAsset());
			Objs.Add(myObj);
		}

	}





//	FString ObjectPath="/Game/";

//	FString BasePackageName="/Game/a.a";

//	UPackage* OldPackage = LoadPackage(nullptr, *BasePackageName, LOAD_None);



	LoadedObject->Modify();


	TSharedPtr< IToolkit > FoundAssetEditor = FToolkitManager::Get().FindEditorForAsset(LoadedObject);
	if (FoundAssetEditor.IsValid() && FoundAssetEditor->IsBlueprintEditor())
	{
		TSharedPtr<IBlueprintEditor> BlueprintEditor = StaticCastSharedPtr<IBlueprintEditor>(FoundAssetEditor);


		TArray<TSharedPtr<class FSCSEditorTreeNode> > testTree;
		testTree = BlueprintEditor->GetSelectedSCSEditorTreeNodes();
		int num = -1;
		UActorComponent* myCompK;
		for (int32 k = 0; k < testTree.Num(); k++)
		{

			num = k;

			myCompK = testTree[k]->GetComponentTemplate();

			//		UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(myCompK);
			//myCompK->PostDuplicate(true);

			//	myStaticK->DestroyComponent(true);
			//		myStaticK->SetStaticMesh(ExistingMat);
			//	myStaticK->PostDuplicate(true);
			//			USceneComponent* root = myStaticK->GetAttachmentRoot();


			//myStaticK->DetachFromComponent(root);


			for (UStaticMesh * ExistingMat : Objs)
			{

				FString x = ExistingMat->GetName();
				const FName NewStaticMeshName = *x;
				USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), NewStaticMeshName);

				FSCSEditorTreeNodePtrType NewNodePtr;

				//	USimpleConstructionScript* NodeSCS = NewNode->GetSCS();

				NewNodePtr = testTree[k]->AddChild(NewNode, false);


				if (NewNode->GetVariableName() != NAME_None)
				{
					FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->GetVariableName());
				}

				UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
				myStaticK->SetStaticMesh(ExistingMat);

				//删除节点方法	
				//	USceneComponent* ChildInstance = Cast<USceneComponent>(testTree[k]->GetComponentTemplate());
				//	TSharedPtr<class FSCSEditorTreeNode > nod= testTree[k];
				//	nod->GetParent()->RemoveChild(nod);

			}
		}

	}




	/*

	for (UStaticMesh * ExistingMat : Objs)
	{
		//	UStaticMesh * ExistingMat = LoadObject<UStaticMesh>(NULL, *BasePackageName, nullptr, LOAD_Quiet | LOAD_NoWarn);
		//	UStaticMesh * ExistingMat = LoadObject<UStaticMesh>(NULL, *BasePackageName, nullptr, LOAD_Quiet | LOAD_NoWarn);



		LoadedObject->Modify();
		FString x = FString("ddd");
		const FName NewStaticMeshName = *x;
		USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), NewStaticMeshName);

		LoadedObject->SimpleConstructionScript->GetDefaultSceneRootNode()->AddChildNode(NewNode, true);
		if (NewNode->VariableName != NAME_None)
		{
			FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->VariableName);
		}
		UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
		myStaticK->SetStaticMesh(ExistingMat);



		FString x1 = FString("fff");
		const FName NewStaticMeshName1 = *x1;
		USCS_Node* NewNode1 = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), NewStaticMeshName1);

		LoadedObject->SimpleConstructionScript->GetDefaultSceneRootNode()->AddChildNode(NewNode1, true);
		if (NewNode1->VariableName != NAME_None)
		{
			FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode1->VariableName);
		}
		myStaticK = Cast<UStaticMeshComponent>(NewNode1->ComponentTemplate);
		myStaticK->SetStaticMesh(ExistingMat);

	}
	*/



//观察Engine\Source\Editor\ContentBrowser\Private\ContentBrowserUtils.cpp其中有rename 和 move delete
// 参考https://answers.unrealengine.com/questions/782588/view.html
//	FStringAssetReference testAssetRef = "/Game/b.b_C";
//	UObject* OldObject = testAssetRef.TryLoad();
/*
	FString BasePackageName1=FString("Blueprint'")+ FString("/Game/b.b_C")+ FString("'");

	//	UPackage* OldPackage1 = LoadPackage(nullptr, *BasePackageName1, LOAD_None);
	//	使用蓝图类型方法
	UObject *OldObject= LoadObject<UObject>(NULL, *BasePackageName1);
//	UClass* OldObject = LoadClass<AActor>(NULL, *BasePackageName1);
//	UClass* OldObject = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/Blueprints/MapPathBrush_BP.MapPathBrush_BP_C'"));

//	static ConstructorHelpers::FClassFinder<UObject> UnitSelector(*BasePackageName1);


//	UBlueprintGeneratedClass* BotPawnClass = Cast<UBlueprintGeneratedClass>(UnitSelector.Class);
//	UBlueprint* OldObject = Cast<UBlueprint>(BotPawnClass->ClassGeneratedBy);
//	UBlueprint* OldObject = Cast<UBlueprint>(UnitSelector.Class);


	FString BasePackageName2 = "bb";

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	TArray<FAssetRenameData> AssetsAndNames;
	const FString PackagePath = FPackageName::GetLongPackagePath(OldObject->GetOutermost()->GetName());
	new(AssetsAndNames) FAssetRenameData(OldObject,PackagePath, BasePackageName2);

	AssetToolsModule.Get().RenameAssets(AssetsAndNames);
//	testAssetRef =FString("/Game/bb");
//	OldObject = testAssetRef.TryLoad();

	FString BasePackageName3 = FString("Blueprint'") + FString("/Game/bb.bb_C") + FString("'");
	UClass* NewBlueprint = LoadClass<AActor>(NULL, *BasePackageName3);

//	static ConstructorHelpers::FClassFinder<AActor> UnitSelector1(*BasePackageName2);

*/
//	UBlueprintGeneratedClass* BotPawnClass1 = Cast<UBlueprintGeneratedClass>(UnitSelector1.Class);
//	UBlueprint* NewBlueprint = Cast<UBlueprint>(BotPawnClass1->ClassGeneratedBy);
//	UBlueprint* NewBlueprint = Cast<UBlueprint>(UnitSelector1.Class);

//	LoadedObject = Cast<UBlueprint>(NewBlueprint);

//以上是rename load参考https://blog.csdn.net/u013131744/article/details/68928021










/*

	TSharedPtr< IToolkit > FoundAssetEditor = FToolkitManager::Get().FindEditorForAsset(LoadedObject);
	if (FoundAssetEditor.IsValid() && FoundAssetEditor->IsBlueprintEditor())
	{
		TSharedPtr<IBlueprintEditor> BlueprintEditor = StaticCastSharedPtr<IBlueprintEditor>(FoundAssetEditor);

	
		TArray<TSharedPtr<class FSCSEditorTreeNode> > testTree;
		testTree = BlueprintEditor->GetSelectedSCSEditorTreeNodes();
		int num = -1;
		UActorComponent* myCompK;
		for (int32 k = 0; k < testTree.Num(); k++)
		{

			num = k;

			myCompK = testTree[k]->GetComponentTemplate();
	//		UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(myCompK);
			//myCompK->PostDuplicate(true);

			//	myStaticK->DestroyComponent(true);
	//		myStaticK->SetStaticMesh(ExistingMat);
		//	myStaticK->PostDuplicate(true);
//			USceneComponent* root = myStaticK->GetAttachmentRoot();
		
			
			//myStaticK->DetachFromComponent(root);
			LoadedObject->Modify();
			FString x = FString("ddd");
			const FName NewStaticMeshName = *x;
			USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), NewStaticMeshName);
			
			FSCSEditorTreeNodePtrType NewNodePtr;

		//	USimpleConstructionScript* NodeSCS = NewNode->GetSCS();

			NewNodePtr = testTree[k]->AddChild(NewNode, false);


			if (NewNode->VariableName != NAME_None)
			{
				FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->VariableName);
			}

			UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
			myStaticK->SetStaticMesh(ExistingMat);

		//删除节点方法	
		//	USceneComponent* ChildInstance = Cast<USceneComponent>(testTree[k]->GetComponentTemplate());
		//	TSharedPtr<class FSCSEditorTreeNode > nod= testTree[k];
		//	nod->GetParent()->RemoveChild(nod);


		}

			



	}

*/





	
	//	UMaterial* BaseMaterial= MakeParentMaterial();
	//	if (BaseMaterial != nullptr)
	//	{
	//		MakeMaterialInstance(BaseMaterial);
	//	}

}
/*
bool FloadModule::RefreshFilteredState(FSCSEditorTreeNodePtrType TreeNode, bool bRecursive)
{
//	FString FilterText = FText::TrimPrecedingAndTrailing(GetFilterText()).ToString();
//	TArray<FString> FilterTerms;
//	FilterText.ParseIntoArray(FilterTerms, TEXT(" "), true);

	struct RefreshFilteredState_Inner
	{
		static void RefreshFilteredState(FSCSEditorTreeNodePtrType TreeNodeIn, const TArray<FString>& FilterTermsIn, bool bRecursiveIn)
		{
			if (bRecursiveIn)
			{
				for (FSCSEditorTreeNodePtrType Child : TreeNodeIn->GetChildren())
				{
					RefreshFilteredState(Child, FilterTermsIn, bRecursiveIn);
				}
			}

			FString DisplayStr = TreeNodeIn->GetDisplayString();

			bool bIsFilteredOut = false;
			for (const FString& FilterTerm : FilterTermsIn)
			{
				if (!DisplayStr.Contains(FilterTerm))
				{
					bIsFilteredOut = true;
				}
			}
			// if we're not recursing, then assume this is for a new node and we need to update the parent
			// otherwise, assume the parent was hit as part of the recursion
			TreeNodeIn->UpdateCachedFilterState(!bIsFilteredOut, !bRecursiveIn);
		}
	};

	RefreshFilteredState_Inner::RefreshFilteredState(TreeNode, FilterTerms, bRecursive);
	return TreeNode->IsFlaggedForFiltration();
}

*/
	



UMaterial* FloadModule::MakeParentMaterial()
{
	//UE_LOG(LogTemp, Warning, TEXT("MaterialBuilder11 BUTTON CLICKED!!!!"));
	//	TArray<UObject*> ImportedObjects = FbxBuilder->ImportAssets(TEXT("/Game/Assets/Mesh"));
	//	FString PathToLoad = "/Game/Assets/Textures/cheti";
	//	tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	//	Material = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(),
	//		UMaterialInstance::StaticClass(), TEXT("MaterialInstanceConstant'/Game/Assets/Material/M_1_Inst'")));
	//		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	//	DynamicMaterial->SetTextureParameterValue(FName("Diffuse"), tmpTexture);

	FString MaterialBaseName = "M_Material";
	FString PackageName = "/Game/Assets/Material/";
	PackageName += MaterialBaseName;
	UPackage* Package = CreatePackage(NULL, *PackageName);

	// create an unreal material asset 建立虚拟材质资源
	UMaterialFactoryNew * MaterialFactory = NewObject<UMaterialFactoryNew>();
	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(MaterialFactory->ResolveSupportedClass(), Package,
		*MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);
	//	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(MaterialFactory->ResolveSupportedClass(), Package, *MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);
	//  MaterialFactory->ResolveSupportedClass() 可改成UMaterial::StaticClass()
	//	UnrealMaterial->MarkPackageDirty();
	//	Package->MarkPackageDirty();




	if (UnrealMaterial != NULL)
	{
		FAssetRegistryModule::AssetCreated(UnrealMaterial);
		Package->FullyLoad();
		Package->SetDirtyFlag(true);
	}
	else
		return nullptr;












	// Diffuse 
	FStringAssetReference DiffuseAssetPath("/Game/Assets/Textures/cheti");
	UTexture* DiffuseTexture = Cast<UTexture>(DiffuseAssetPath.TryLoad());



	if (DiffuseTexture)
	{


		// make texture sampler 建立图片采样器（第一张常量图）
		UMaterialExpressionTextureSample* TextureExpression = NewObject<UMaterialExpressionTextureSample>(UnrealMaterial);
		TextureExpression->Texture = DiffuseTexture;
		TextureExpression->SamplerType = SAMPLERTYPE_Color;
		TextureExpression->MaterialExpressionEditorX = -500;
		TextureExpression->MaterialExpressionEditorY = -500;
		UnrealMaterial->Expressions.Add(TextureExpression);
		
		//		UMaterialExpressionConstant* ZeroExpression; //常量
		//		ZeroExpression = NewObject<UMaterialExpressionConstant>(UnrealMaterial);//一维常量
		//		ZeroExpression->R = 2.0f;
		//		UnrealMaterial->Expressions.Add(ZeroExpression);

		//		UMaterialExpressionConstant3Vector* V3;		//三维常量
		//		V3 = NewObject<UMaterialExpressionConstant3Vector>(UnrealMaterial);//三维常量
		//		V3->Constant = FColor::Red;
		//		V3->Constant = FLinearColor(value[0], value[1], value[2]);
		//		UnrealMaterial->Expressions.Add(V3);


		//一维变量
		UMaterialExpressionScalarParameter* Parameter1 = NewObject<UMaterialExpressionScalarParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Parameter1);
		Parameter1->SetEditableName("Power");
		Parameter1->DefaultValue = 0.5f;
		Parameter1->MaterialExpressionEditorX = -500;
		Parameter1->MaterialExpressionEditorY = -100;
		UnrealMaterial->Expressions.Add(Parameter1);


		// Tiling system 块节点系统乘法 表达式
		UMaterialExpressionMultiply* Multiply = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
		Multiply->MaterialExpressionEditorX = -250;
		Multiply->MaterialExpressionEditorY = -200;
		UnrealMaterial->Expressions.Add(Multiply);
		//FExpressionInput InputExpression;
		//InputExpression.Expression = Multiply;		

		//关系
		Multiply->B.Expression = Parameter1;
		Multiply->A.Connect(2, TextureExpression);//input链接output的序列号，序列号从0开始算



		//TextureExpression->SamplerType
		//TArray<FExpressionOutput> Outputs = TextureExpression->GetOutputs();// .Expression->GetOutputs();
		//FExpressionOutput* Output = Outputs.GetData();
		//Multiply->A.Expression = Output->MaskR;
		//TextureExpression->Coordinates.Expression = Multiply; //UVs连线
		UnrealMaterial->BaseColor.Expression = Multiply;




		TArray<FExpressionOutput> Outputs = UnrealMaterial->BaseColor.Expression->GetOutputs();
		FExpressionOutput* Output = Outputs.GetData();
		UnrealMaterial->BaseColor.Mask = Output->Mask;
		UnrealMaterial->BaseColor.MaskR = Output->MaskR;
		UnrealMaterial->BaseColor.MaskG = Output->MaskG;
		UnrealMaterial->BaseColor.MaskB = Output->MaskB;
		UnrealMaterial->BaseColor.MaskA = Output->MaskA;







		//第二张变量图
		UMaterialExpressionTextureSampleParameter2D* TextureExpression1 = NewObject<UMaterialExpressionTextureSampleParameter2D>(UnrealMaterial);
		TextureExpression1->Texture = DiffuseTexture;
		TextureExpression1->SamplerType = SAMPLERTYPE_Color;
		//	TextureExpression1->ParameterName= TEXT("RoughPic");
		TextureExpression1->SetEditableName("RoughPic");
		TextureExpression1->MaterialExpressionEditorX = -500;
		TextureExpression1->MaterialExpressionEditorY = 100;
		UnrealMaterial->Expressions.Add(TextureExpression1);//不加的话实例里面没有



		UMaterialExpressionVectorParameter * VP3;//三维变量
		VP3 = NewObject<UMaterialExpressionVectorParameter>(UnrealMaterial);
		VP3->SetEditableName("RoughnessColor");
		//	VP3->ParameterName = TEXT("RoughValue");				
		VP3->DefaultValue.R = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
		VP3->DefaultValue.G = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
		VP3->DefaultValue.B = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
		VP3->DefaultValue.A = 1.0f;
		VP3->MaterialExpressionEditorX = -500;
		VP3->MaterialExpressionEditorY = 400;
		UnrealMaterial->Expressions.Add(VP3);


		//第二乘法表达式
		UMaterialExpressionMultiply* Multiply1 = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Multiply1);
		Multiply1->MaterialExpressionEditorX = -250;
		Multiply1->MaterialExpressionEditorY = 300;
		UnrealMaterial->Expressions.Add(Multiply1);



		//关系	
		Multiply1->B.Expression = VP3;
		Multiply1->A.Expression = TextureExpression1;
		//TextureExpression1->Coordinates.Expression = Multiply1; //UVs连线
		UnrealMaterial->Roughness.Expression = Multiply1;

		TArray<FExpressionOutput> Outputs1 = UnrealMaterial->Roughness.Expression->GetOutputs();
		FExpressionOutput* Output1 = Outputs1.GetData();
		UnrealMaterial->Roughness.Mask = Output1->Mask;
		UnrealMaterial->Roughness.MaskR = Output1->MaskR;
		UnrealMaterial->Roughness.MaskG = Output1->MaskG;
		UnrealMaterial->Roughness.MaskB = Output1->MaskB;
		UnrealMaterial->Roughness.MaskA = Output1->MaskA;

		//	UMaterialExpression * Expression3D = CreateNewMaterialExpression(UMaterialExpressionFunctionOutput::StaticClass(), FVector3D(1, 2, 3), false, true);//一维常量
		//	UnrealMaterial->Expressions.Add(Expression3D);

		//UnrealMaterial->Specular.Expression = ZeroExpression;
		//	Multiply1->B.Expression = Expression3D;


		// Tiling 链接图片节点外连接乘法表达式
		//		TextureExpression->Coordinates.Expression = Multiply;
	}
	else
		return nullptr;



	// let the material update itself if necessary
	UnrealMaterial->PreEditChange(NULL);
	UnrealMaterial->PostEditChange();

	// make sure that any static meshes, etc using this material will stop using the FMaterialResource of the original 
	// material, and will use the new FMaterialResource created when we make a new UMaterial in place
	FGlobalComponentReregisterContext RecreateComponents;
	return (UnrealMaterial);
	//	UMaterialInstance *terrainMaterialInstance = UMaterialInstance::Parent=Create(UnrealMaterial, NULL);
}

void FloadModule::MakeMaterialInstance(UMaterial* UnrealMaterial)
{
	FString Name = "M_Material_Ins";
	FString PackageName1 = "/Game/Assets/Material/";
	PackageName1 += Name;
	UPackage* Package1 = CreatePackage(NULL, *PackageName1);

	UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
	Factory->InitialParent = UnrealMaterial;

	UMaterialInstanceConstant* UnrealMaterialInstance = (UMaterialInstanceConstant*)Factory->FactoryCreateNew(Factory->ResolveSupportedClass(), Package1,
		*Name, RF_Standalone | RF_Public, NULL, GWarn);
	if (UnrealMaterialInstance != NULL)
	{
		FAssetRegistryModule::AssetCreated(UnrealMaterialInstance);
		Package1->FullyLoad();
		Package1->SetDirtyFlag(true);
	}
	else
		return;
	FLinearColor CurrentColor;
	UTexture * A;
	float m;

	//针对图片变量
	FStringAssetReference DiffuseAssetPath("/Game/Assets/Textures/cheti1");
	UTexture* B = Cast<UTexture>(DiffuseAssetPath.TryLoad());
	UnrealMaterialInstance->GetTextureParameterValue(FName("RoughPic"), A);
	UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("RoughPic"), B);



	//针对向量变量
	UnrealMaterialInstance->GetVectorParameterValue(FName("RoughnessColor"), CurrentColor);
	CurrentColor = FLinearColor(0.5, 0.5, 0.7, 1);
	UnrealMaterialInstance->SetVectorParameterValueEditorOnly(FName("RoughnessColor"), CurrentColor);

	//针对数值变量
	m = 5;
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("Power"),m);



	//	UMaterialExpressionVectorParameter

}
/*
IAssetTools& FloadModule::Get() const
{
	check(FbxBuilder);
	return *FbxBuilder;
}
*/

UImportAndSaveUI::UImportAndSaveUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	bIsReimport = false;
	this->SetFlags(RF_Transactional);
	/*
	bAutomatedImportShouldDetectType = true;
	//Make sure we are transactional to allow undo redo
	

	StaticMeshImportData = CreateDefaultSubobject<UFbxBuilderStaticMeshImportData>(TEXT("StaticMeshImportData"));
	StaticMeshImportData->SetFlags(RF_Transactional);
	StaticMeshImportData->LoadOptions();

	SkeletalMeshImportData = CreateDefaultSubobject<UFbxBuilderSkeletalMeshImportData>(TEXT("SkeletalMeshImportData"));
	SkeletalMeshImportData->SetFlags(RF_Transactional);
	SkeletalMeshImportData->LoadOptions();

	AnimSequenceImportData = CreateDefaultSubobject<UFbxBuilderAnimSequenceImportData>(TEXT("AnimSequenceImportData"));
	AnimSequenceImportData->SetFlags(RF_Transactional);
	AnimSequenceImportData->LoadOptions();

	TextureImportData = CreateDefaultSubobject<UFbxBuilderTextureImportData>(TEXT("TextureImportData"));
	TextureImportData->SetFlags(RF_Transactional);
	TextureImportData->LoadOptions();
*/
}


#undef LOCTEXT_NAMESPACE
	
