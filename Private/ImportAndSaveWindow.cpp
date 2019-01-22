// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "ImportAndSaveWindow.h"
#include "UnrealEd.h"
#include "Editor/ContentBrowser/Public/ContentBrowserModule.h"
//#include "Editor/ContentBrowser/Private/SPathView.h"
//#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "Editor/UnrealEd/Classes/Factories/FbxFactory.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "MainFrame.h"
#include "IDocumentation.h"
#include "PropertyEditorModule.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Developer/SourceControl/Public/SourceControlHelpers.h"
#include "Editor/UnrealEd/Public/Tests/AutomationEditorCommon.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "DesktopPlatformModule.h"
//#include "Runtime/ApplicationCore/Public/Lumin/LuminPlatformApplicationMisc.h"
#include <string>
#include "Factories/TextureFactory.h"
//#include "FbxBuilderImporter.h"
#include "FbxBuilderFactory.h"

#include "SNotificationList.h"
//#include "NotificationManager.h"
#include "AssetData.h"
#include "ObjectTools.h"
#include "Engine/Texture2D.h"
#include "IClassTypeActions.h"
#include "FileManagerGeneric.h"
#include "AssetToolsModule.h"
#include "PackageTools.h"
#include "Editor/ContentBrowser/Public/ContentBrowserModule.h"
#include "AlexOBJ.h"

#include "Framework/Application/SlateApplication.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"

#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Editor/UnrealEd/Classes/Factories/MaterialFactoryNew.h"

#include "Runtime/Engine/Public/ComponentReregisterContext.h"
#include "Runtime/Engine/Classes/Materials/MaterialExpression.h"

//#include "Editor/AIGraph/Classes/AIGraphTypes.h"

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



#include "IDetailsView.h"

#define LOCTEXT_NAMESPACE "FBXOption"





void SImportAndSaveWindow::Construct(const FArguments& InArgs)
{
//	ImportUI = InArgs._ImportUI;
	WidgetWindow = InArgs._WidgetWindow;
	bIsObjFormat = InArgs._IsObjFormat;
//	UMs = InArgs._ImportMaterials;
//	OnPreviewFbxImport = InArgs._OnPreviewFbxImport;

//	check (ImportUI);
	
	TSharedPtr<SBox> ImportTypeDisplay;
	TSharedPtr<SBox> ImportTypeDisplay2;
	TSharedPtr<SHorizontalBox> FbxHeaderButtons;
	TSharedPtr<SBox> InspectorBox;

	MeshAddPath="/";
//	MeshAddPath = "/Mesh/";

	PicAddPath="/";
//	PicAddPath = "/Texture/";

	MatAddPath="/";
//	MatAddPath = "/Material/";

	InputFilePath = FText::FromString("D:/");
	newPath.InText = FText::FromString("Content/");
	this->ChildSlot
	[
		//查阅https://docs.unrealengine.com/en-us/Programming/Slate/Widgets
		//查阅https://forums.unrealengine.com/community/community-content-tools-and-tutorials/61539-tutorial-slate-slistview-example?89917-Tutorial-Slate-SListView-Example=
		SNew(SBox)
		.MaxDesiredHeight(InArgs._MaxWindowHeight)
		.MaxDesiredWidth(InArgs._MaxWindowWidth)
		[

			SNew(SVerticalBox)   //VerticalBox垂直的盒子
			+ SVerticalBox::Slot()//.FillHeight(1)	//挂点1 位置
			.AutoHeight()
			.Padding(2)	//填充
			[
				SAssignNew(ImportTypeDisplay, SBox)	//盒子组合
			]
			+ SVerticalBox::Slot()//.FillHeight(1)	//挂点1 位置
			.AutoHeight()
			.Padding(2)	//填充
			[
				SAssignNew(ImportTypeDisplay2, SBox)	//盒子组合
			]

	//listview实验
			+SVerticalBox::Slot() //挂点4
			.AutoHeight()
			.Padding(2)
			[
				SAssignNew(InspectorBox, SBox)
				.MaxDesiredHeight(650.0f)
				.WidthOverride(400.0f)
			]
			+ SVerticalBox::Slot()	//挂点5
			.AutoHeight()
			.HAlign(HAlign_Right)	//水平右对齐
			.Padding(2)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(2)
				+ SUniformGridPanel::Slot(0, 0)
				[
					IDocumentation::Get()->CreateAnchor(FString("Engine/Content/FBX/ImportOptions"))//锚点
				]
				+ SUniformGridPanel::Slot(2, 0)
				[
					SAssignNew(ImportButton, SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("FbxOptionWindow_Import", "Import"))
					.IsEnabled(this, &SImportAndSaveWindow::CanImport)
					.OnClicked(this, &SImportAndSaveWindow::OnImport)
				]
				+ SUniformGridPanel::Slot(3, 0)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("FbxOptionWindow_Cancel", "Cancel"))
					.ToolTipText(LOCTEXT("FbxOptionWindow_Cancel_ToolTip", "Cancels importing this FBX file"))
					.OnClicked(this, &SImportAndSaveWindow::OnCancel)
				]
			]
			
		]
	
	];
	





	ImportTypeDisplay->SetContent(
		SNew(SBorder)
		.Padding(FMargin(3))//外边距
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SHorizontalBox)	//水平盒子
			+ SHorizontalBox::Slot() //挂点3
			.AutoWidth()
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			.AutoWidth()
			[
				SNew(STextBlock)	//文字控件 文件路径
				.Font(FEditorStyle::GetFontStyle("CurveEd.LabelFont"))
				.Text(LOCTEXT("Import_CurrentFileTitle", "Current Import Folder: "))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点4 
			.Padding(5, 0, 0, 0)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			.MaxWidth(1600.f)
		//	.AutoWidth()
			[
				SNew(SEditableTextBox)
		//		.MinDesiredWidth(800.f)
				.Text(this, &SImportAndSaveWindow::GetInputText)//自设ddd   InArgs._FullPath
				.OnTextCommitted(this, &SImportAndSaveWindow::OnInputChanged)				
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SBox)
				.HAlign(HAlign_Right)
				[
					SAssignNew(FbxHeaderButtons, SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(2.0f, 0.0f))
					[
						SNew(SButton)
						.Text(LOCTEXT("FbxOptionWindow_SelectImport", "...Select Folder"))
						.OnClicked(this, &SImportAndSaveWindow::OnGetFolderName)
					]
				]
			]

		]
	);
	ImportTypeDisplay2->SetContent(
		SNew(SBorder)
		.Padding(FMargin(3))//外边距
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SHorizontalBox)	//水平盒子
			+ SHorizontalBox::Slot() //挂点3
			.AutoWidth()
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			.AutoWidth()
			[
				SNew(STextBlock)	//文字控件 文件路径
				.Font(FEditorStyle::GetFontStyle("CurveEd.LabelFont"))
				.Text(LOCTEXT("Export_CurrentFileTitle", "Current Save Folder  : "))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点4 
			.Padding(5, 0, 0, 0)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			.MaxWidth(1600.f)
		//	.AutoWidth()
			[
				SNew(SEditableTextBox)
		//		.MinDesiredWidth(800.f)
				.Text(this, &SImportAndSaveWindow::GetExportText)//自设ddd   InArgs._FullPath
				.OnTextCommitted(this, &SImportAndSaveWindow::OnExportChanged)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SBox)
				.HAlign(HAlign_Right)
				[
					SAssignNew(FbxHeaderButtons, SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(2.0f, 0.0f))
					[
						SNew(SButton)
						.Text(LOCTEXT("FbxOptionWindow_SelectSave", "...Select Folder"))
						.OnClicked(this, &SImportAndSaveWindow::OnGetSavePath)
					]
				]
			]

		]
	);


}

FReply SImportAndSaveWindow::OnPreviewClick() const
{
	//Pop a preview window to let the user see the content of the fbx file
	return FReply::Handled();
}

void SImportAndSaveWindow::OnInputChanged(const FText& InText, ETextCommit::Type)
{
	InputFilePath =InText;

}

FText SImportAndSaveWindow::GetInputText() const
{
	return (InputFilePath);
}

void SImportAndSaveWindow::OnExportChanged(const FText& InText, ETextCommit::Type)
{
	newPath.InText = InText;

}

FText SImportAndSaveWindow::GetExportText() const
{
	return (newPath.InText);
}
/*
FReply SImportAndSaveWindow::OnGetExportFolderName()
{
	return FReply::Handled();
}
*/

void GetInfoOfFileName(FString Name, FString &StartWith, FString &MiddleWtih, int32 &num)
{
	int32 firststrpos = -1;
	Name.FindChar(TCHAR('_'), firststrpos);
	//	Name.FindLastChar(TCHAR('_'), secondpos);
	if ((firststrpos >= 0) && (firststrpos < Name.Len()))
	{
		StartWith = Name.Left(firststrpos);

		FString start = StartWith + "_";

		int32 fulllen = Name.Len() - firststrpos - 1;

		FString FullMeshName = Name.Right(fulllen);

		int32 line1, line2;
		FullMeshName.FindChar(TCHAR('_'), line1);
		FullMeshName.FindLastChar(TCHAR('_'), line2);

		if (line2 < FullMeshName.Len() - 1)
		{
			FString Middle = FullMeshName.Left(line2);
			FString Count = FullMeshName.Right(fulllen - line2 - 1);

			bool IsNum = true;
			for (int32 x = 0;x < Count.Len();x++)
			{
				TCHAR countstr = Count[x];
				//	FText Message1 = FText::FromString(FString::Printf(*FString("Count=%c\n"), countstr));
				//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);

				if ((countstr == TCHAR('1')) || (countstr == TCHAR('2')) || (countstr == TCHAR('3')) || (countstr == TCHAR('4')) || (countstr == TCHAR('5')) || (countstr == TCHAR('6')) || (countstr == TCHAR('7')) || (countstr == TCHAR('8')) || (countstr == TCHAR('9')) || (countstr == TCHAR('0')))
				{
					continue;
				}
				else
				{

					IsNum = false;
					break;
				}

			}
			if (IsNum)
			{
				MiddleWtih = Middle;
				num = FCString::Atoi(*Count);
				return;
			}
			else
			{

				MiddleWtih = FullMeshName;
				num = -1;
				return;
			}
		}
		else
		{
			StartWith = Name;
		}


	}
}


class ClassOfFile
{
public:
	ClassOfFile(UAlexOBJ * newone,FString firststr,FString middlestr,int32 num)
	{
		Start = firststr;
		Middle = middlestr;
		ContainOBJS.Add(*newone);
		minicount = num;
		FirstOBJS = newone;
	}
	~ClassOfFile()
	{

	}
	FString Start;
	FString Middle;
	TArray<UAlexOBJ> ContainOBJS;
	int32 minicount;
	UAlexOBJ * FirstOBJS;
	void Add(UAlexOBJ * newone,FString firststr,FString middlestr,int32 num)
	{
		ContainOBJS.Add(*newone);
		if ((num < minicount))// && (num >= 0))
		{
			minicount = num;
			FirstOBJS = newone;

		}

	}
	bool TryAdd(UAlexOBJ * newone, FString firststr, FString middlestr, int32 num)
	{
	//	FString firststr = "";
	//	FString middlestr = "";
	//	int32 num = -1;
	//	GetInfoOfFileName(newone->SaveMeshName, firststr, middlestr, num);
		if ((firststr == Start) && (middlestr == Middle))
		{
			Add(newone, firststr, middlestr, num);
			return true;
		}
		else
		{
			return false;
		}

	}
};

//控制类可加选队列，抽取队列数字最小的元素。
class ClassFileArray
{
public:
	ClassFileArray()
	{

	}
	~ClassFileArray()
	{

	}
	TArray<ClassOfFile> FileArray;

	void Add(UAlexOBJ * newobj)
	{
		FString firststr = "";
		FString middlestr = "";
		int32 num = -1;
		GetInfoOfFileName(newobj->SaveMeshName, firststr, middlestr, num);
		bool IsInArray = false;
		for (int x=0;x< FileArray.Num();x++)
		{
			if (FileArray[x].TryAdd(newobj, firststr, middlestr, num))
			{
				IsInArray = true;
				break;
			}
		}
		if ((!IsInArray) && (firststr != "") && (middlestr != ""))
		{
			ClassOfFile *newfile = new ClassOfFile(newobj, firststr, middlestr, num);
			FileArray.Add(*newfile);
		}

	}
	UAlexOBJ *Get(UAlexOBJ * input,bool isdelete)
	{
		FString firststr = "";
		FString middlestr = "";
		int32 num = -1;
		GetInfoOfFileName(input->SaveMeshName, firststr, middlestr, num);
		for (int x = 0;x < FileArray.Num();x++)
		{
			if ((firststr== FileArray[x].Start)&&(middlestr==FileArray[x].Middle))
			{
				if ((isdelete)&&(num!= FileArray[x].minicount))
				{
					TArray<UObject*> AssetsIn2;
					AssetsIn2.Add(input->OwnerMesh);
					ObjectTools::DeleteObjects(AssetsIn2, false);
					
				}
				return (FileArray[x].FirstOBJS);
			}
		}
		return(input);
	}
};

//获取对话框返回的路径
FReply SImportAndSaveWindow::OnGetSavePath()
{
/*
	TSharedPtr<SWindow> ParentWindow;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}*/
	const float SaveWindowWidth = 410.0f;
	const float SaveWindowHeight = 750.0f;
	FVector2D SaveWindowSize = FVector2D(SaveWindowWidth, SaveWindowHeight); // Max window size it can get based on current slate

	FSlateRect WorkAreaRect = FSlateApplicationBase::Get().GetPreferredWorkArea();
	FVector2D DisplayTopLeft(WorkAreaRect.Left, WorkAreaRect.Top);
	FVector2D DisplaySize(WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top);

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(DisplayTopLeft.X, DisplayTopLeft.Y);
	SaveWindowSize *= ScaleFactor;

	FVector2D WindowPosition = (DisplayTopLeft + (DisplaySize - SaveWindowSize) / 2.0f) / ScaleFactor;
	//	UImportAndSaveUI* ImportUI = NewObject<UImportAndSaveUI>();
	//	ImportUI->MeshTypeToImportB = FBXBuilderIT_AnimationB;
	//	ImportUI->OriginalImportTypeB= FBXBuilderIT_SkeletalMeshB;

	//	ImportUI->bOverrideFullName = false;
	//	ReimportUI->bImportAnimations = true;
	//	FBXImportOptions* ImportOptions = FbxImporter->GetImportOptions();

//	newPath= strPath::strPath();// = new strPath();
//	newPath.InText;// = OutputFilePath;

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(NSLOCTEXT("Contents", "ContentTitle", "Content Select"))
		.SizingRule(ESizingRule::Autosized)
		.AutoCenter(EAutoCenter::None)
		.ClientSize(SaveWindowSize)
		.ScreenPosition(WindowPosition);
	TSharedPtr<SDDFileTree> ContentWindow;
	bool bForceImportType = true;
//	ContentWindow->ArrayPath1.Add(OutputFilePath);
	Window->SetContent
	(
		SAssignNew(ContentWindow, SDDFileTree)
		.StringPath(&newPath)
		.WidgetWindow(Window)
		.MaxWindowHeight(SaveWindowHeight)
		.MaxWindowWidth(SaveWindowWidth)
	);
	//	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
	//	FSlateApplication::Get().AddWindow(Window);
	TSharedPtr<SWindow> TopWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
	FSlateApplication::Get().AddWindowAsNativeChild(Window, TopWindow.ToSharedRef(), true);

//	OutputFilePath = FText::FromString(ContentWindow->SelectStr);

//	OutputFilePath= FText::FromString();
	return FReply::Handled();
}


//进行信息搜集
FReply SImportAndSaveWindow::OnGetFolderName()
{

	bool bOpened = true;
	UAlexStorage * AlexStorageTool;
	FString OpenFolderIn;

	AlexStorageTool = new UAlexStorage();
	OpenFolderIn = AlexStorageTool->FillStorage("D:\\")+FString("");// , &ImportedFileNames);

	InputFilePath = FText::FromString(OpenFolderIn);


//	DetailsView->SetObject(ImportUI, true);
	return FReply::Handled();
}



void SImportAndSaveWindow::ImportFBXAndMaterial()
{
	//初始化材质对象列表
	TArray<UAlexMaterial> UMs;

	
	FString OpenFolder = InputFilePath.ToString();
	if (OpenFolder == "")
		return;
	if (OpenFolder.EndsWith("/") || OpenFolder.EndsWith("\\"))
	{
		int32 x = OpenFolder.Len();
		OpenFolder = OpenFolder.Left(x - 1);
	}
	if (!FPaths::DirectoryExists(OpenFolder))
	{
		FText Message = FText::FromString("Message");
		FText Message1 = FText::FromString("Import Folder is not exist!");
		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message);
		return;
	}

	

	FString GetChar = newPath.InText.ToString();
	FString ExportFolder2;
//	if (GetChar.StartsWith("/"))
//		ExportFolder2 = TEXT("/Game") + GetChar;
//	else
//		ExportFolder2 = TEXT("/Game/") + GetChar;
	int32 fromlen = GetChar.Len();
	if (GetChar.StartsWith("Content/"))
		ExportFolder2 = TEXT("/Game/") + GetChar.Right(fromlen - 8);
	else if (GetChar.StartsWith("Content"))
		ExportFolder2 = TEXT("/Game") + GetChar.Right(fromlen - 7);
//	else
//		return;
//	FText Message5 = FText::FromString(FString::Printf(TEXT("%s"), *ExportFolder2));
//	FMessageDialog::Open(EAppMsgType::Ok, Message5, &Message5);
//	return;
	if (!ExportFolder2.StartsWith("/Game") || ExportFolder2.Contains(" ") || ExportFolder2.Contains("\\") || ExportFolder2.Contains(":") || ExportFolder2.Contains(":") || ExportFolder2.Contains("*") || ExportFolder2.Contains("?")|| ExportFolder2.Contains("\"") || ExportFolder2.Contains("<") || ExportFolder2.Contains(">") || ExportFolder2.Contains("|") || ExportFolder2.Contains("'") || ExportFolder2.Contains(",") || ExportFolder2.Contains(".") || ExportFolder2.Contains("&") || ExportFolder2.Contains("!") || ExportFolder2.Contains("~") || ExportFolder2.Contains("@") || ExportFolder2.Contains("#") || ExportFolder2.Contains("//"))
	{
		FText Message = FText::FromString("Message");
		FText Message1 = FText::FromString("Save Folder is not correct!");
		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message);
		return ;
	}
	FString ExportFolder;
	int32 Exlen = ExportFolder2.Len();
	if (ExportFolder2.EndsWith("/"))
		ExportFolder = ExportFolder2.Left(Exlen - 1);
	else
		ExportFolder = ExportFolder2;

	ClassFileArray ClassOfMeshs;
	FString ObjectPath;
	TArray<FString> ImportedFileNames;
	FString BasePackageName;
	FString TextureName;
	UStaticMesh* StaticMeshFirst = nullptr;

	UAlexStorage * AlexStorageTool;

	AlexStorageTool = new UAlexStorage();

	AlexStorageTool->LoadPic(OpenFolder, ExportFolder,&UMs, &ImportedFileNames);




	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	UBlueprint* LoadedObject = nullptr;// = NewObject<UBlueprint>();

	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		if (SelectedAssets[i].GetClass() == UBlueprint::StaticClass())
		{
			LoadedObject = Cast<UBlueprint>(SelectedAssets[i].GetAsset());
		}

	}
	bool CanChangeWithMini = true;
	for (int i = 0; i < ImportedFileNames.Num(); i++)
	{
		UFbxBuilderFactory * StaticFbxFactory;
		StaticFbxFactory = NewObject<UFbxBuilderFactory>();
		TArray<UAlexOBJ *>OBJS;
		StaticFbxFactory->AlexOBJS = &OBJS;
		StaticFbxFactory->EnableShowOption();

		StaticFbxFactory->ResetState();

		
		UPackage* ModelPackage = nullptr;
		FString Extension = FPaths::GetExtension(ImportedFileNames[i]).ToLower();

		FString FileName_EluModel = FPaths::GetBaseFilename(ImportedFileNames[i]);
		FString ModelPackageName = ExportFolder + MeshAddPath + FileName_EluModel;

		if (FPackageName::DoesPackageExist(ModelPackageName))
		{
			continue;
			ModelPackage = LoadPackage(nullptr, *ModelPackageName, LOAD_None);
			if (ModelPackage)
			{
				ModelPackage->FullyLoad();
			}

		}
		else
		{
			ModelPackage = CreatePackage(nullptr, *ModelPackageName);
			ModelPackage->FullyLoad();
		}


		//读取fbx前获取路径下资源
		TArray<FString> InPaths0;
		TArray<FAssetData> AssetList0;
		InPaths0.Add(ExportFolder);
		FAssetRegistryModule& AssetRegistryModule0 = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

		// Form a filter from the paths
		FARFilter Filter0;
		Filter0.bRecursivePaths = true;
		//获取目录资源数据流
		for (int32 PathIdx = 0; PathIdx < InPaths0.Num(); ++PathIdx)
		{
			new (Filter0.PackagePaths) FName(*InPaths0[PathIdx]);
		}

		// Query for a list of assets in the selected paths
		AssetRegistryModule0.Get().GetAssets(Filter0, AssetList0);



		//获取符合类型及前缀的UStaticMesh
		TArray<FString> MehsOldNames;
		for (int32 AssetIdx = 0; AssetIdx < AssetList0.Num(); ++AssetIdx)
		{
			const FAssetData& AssetData = AssetList0[AssetIdx];

			UObject* Obj = AssetData.GetAsset();
			if (Obj)
			{
				if ((Obj->GetClass() == UStaticMesh::StaticClass()) && (Obj->GetName().StartsWith(FileName_EluModel)))
				{
					if ((Obj->GetName().Len() > FileName_EluModel.Len()) && (Obj->GetName()[FileName_EluModel.Len()] != '_'))
					//新需求_变-
				//	if ((Obj->GetName().Len() > FileName_EluModel.Len()) && (Obj->GetName()[FileName_EluModel.Len()] != '-'))
						continue;
		
					MehsOldNames.Add(Obj->GetName());
				}
			}
		}
		StaticFbxFactory->AddToRoot();
		bool bImportCancelled = false;
		UObject* ImportedModel = StaticFbxFactory->ImportObject(UStaticMesh::StaticClass(),
			ModelPackage, FName(*FileName_EluModel),
			EObjectFlags::RF_Standalone | EObjectFlags::RF_Public,
			ImportedFileNames[i],
			nullptr,
			bImportCancelled);
		CanChangeWithMini = StaticFbxFactory->canchangewithmini;
//		StaticMeshFirst = Cast<UStaticMesh>(ImportedModel);
		FAssetRegistryModule::AssetCreated(ImportedModel);
		StaticFbxFactory->CleanUp();
		StaticFbxFactory->RemoveFromRoot();

		//获取路径下资源
		TArray<FString> InPaths;
		TArray<FAssetData> AssetList;
		InPaths.Add(ExportFolder);
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));


		// Form a filter from the paths
		FARFilter Filter;
		Filter.bRecursivePaths = true;
		//获取目录资源数据流
		for (int32 PathIdx = 0; PathIdx < InPaths.Num(); ++PathIdx)
		{
			new (Filter.PackagePaths) FName(*InPaths[PathIdx]);
		}

		// Query for a list of assets in the selected paths
		AssetRegistryModule.Get().GetAssets(Filter, AssetList);
		if (AssetList.Num() <= 0)
			continue;

		//获取符合类型及前缀的UStaticMesh
		TArray<UStaticMesh *> Meshs;
		for (int32 AssetIdx = 0; AssetIdx < AssetList.Num(); ++AssetIdx)
		{
			const FAssetData& AssetData = AssetList[AssetIdx];

			UObject* Obj = AssetData.GetAsset();
			if (Obj)
			{
				if ((Obj->GetClass() !=USkeletalMesh::StaticClass() )&&(Obj->GetClass() == UStaticMesh::StaticClass()) && (Obj->GetName().StartsWith(FileName_EluModel)))
				{

					if ((Obj->GetName().Len() > FileName_EluModel.Len()) && (Obj->GetName()[FileName_EluModel.Len()] != '_'))
						//新需求_变-
				//	if ((Obj->GetName().Len() > FileName_EluModel.Len()) && (Obj->GetName()[FileName_EluModel.Len()] != '-'))
						continue;
					if (MehsOldNames.Contains(Obj->GetName()))
						continue;
				
					//	FText Message1 = FText::FromString(FString::Printf(*FString("FileName=%s\n"), *AssetData.GetPackage()->GetName()));
					//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);	
					//新需求_变-

					
					int32 ch = -1;
					for (int objnum = 0; objnum < OBJS.Num(); objnum++)
					{
						if (OBJS[objnum]->MeshFileName == AssetData.GetPackage()->GetName())
						{
							ch = objnum;
							
							break;
						}
					}
					if (ch != -1)
					{

						UStaticMesh * meshsingle = Cast<UStaticMesh>(Obj);
						Meshs.Add(meshsingle);
						OBJS[ch]->SaveMeshName = meshsingle->GetName();
						OBJS[ch]->OwnerMesh = meshsingle;
						if (CanChangeWithMini)
						{
							ClassOfMeshs.Add(OBJS[ch]);
						}
				//		FString middle;
				//		FString first;
				//		int32 count;
				//		GetInfoOfFileName(meshsingle->GetName(), first, middle, count);
						
				//		FText Message1 = FText::FromString(FString::Printf(*FString("1Name=%s\n2Name=%s\nCount=%d\n"), *first,*middle,count));
				//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);



					}
				}
			}
		}




		if (Meshs.Num() == 0)
			continue;
		
		

		for (UStaticMesh * StaticMesh : Meshs)
		{
			for (int32 MatIndex = 0; MatIndex <= UMs.Num() - 1; ++MatIndex)
			{
				for (FStaticMaterial& MeshMat : StaticMesh->StaticMaterials)
				{
					UAlexMaterial * Mat = &(UMs[MatIndex]);
					FString title_text1 = FileName_EluModel + "_" + MeshMat.ImportedMaterialSlotName.ToString();
					title_text1 = PackageTools::SanitizePackageName(title_text1);
					if (Mat->Name.Equals(title_text1))
					{

						FString OldPackageName = PackageTools::SanitizePackageName(MeshMat.ImportedMaterialSlotName.ToString());

						OldPackageName = ExportFolder + MeshAddPath + OldPackageName + FString(".") + OldPackageName;
						UPackage * OldPack;
						UPackage* OldPackage = LoadPackage(nullptr, *OldPackageName, LOAD_None);
						UMaterialInterface * ExistingMat = LoadObject<UMaterialInterface>(NULL, *OldPackageName, nullptr, LOAD_Quiet | LOAD_NoWarn);

						FString MIPackageName = ExportFolder + MatAddPath + Mat->Name + FString(".") + Mat->Name;//FString("/Material/")
						UPackage* MIPackage = LoadPackage(nullptr, *MIPackageName, LOAD_None);
						UMaterialInstanceConstant * MIConstant = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), MIPackage, *MIPackageName));
						MeshMat.MaterialInterface = MIConstant;






						if (ExistingMat)
						{
							OldPack = ExistingMat->GetOutermost();
							OldPack->ConditionalBeginDestroy();
						}

						

					}

				}
			}//每个材质
			//存储
	//		StaticMesh->PostEditChange();
	//		StaticMesh->GetOutermost()->MarkPackageDirty();
	//		FString SingleStaticMeshPackageName = StaticMesh->GetOutermost()->GetName();
	//		FString FinalPackageFilename = FPackageName::LongPackageNameToFilename(SingleStaticMeshPackageName) + ".uasset";
	//		FSaveErrorOutputDevice SaveErrors;
	//		GUnrealEd->Exec(NULL, *FString::Printf(TEXT("OBJ SAVEPACKAGE PACKAGE=\"%s\" FILE=\"%s\" SILENT=true"), *SingleStaticMeshPackageName, *FinalPackageFilename), SaveErrors);

		}//每个Mesh
//存储大单位mesh可能用不到
//		StaticMeshFirst->PostEditChange();
//		ModelPackage->MarkPackageDirty();
//		FString SingleStaticMeshPackageName = StaticMeshFirst->GetOutermost()->GetName();
//		FString FinalPackageFilename = FPackageName::LongPackageNameToFilename(SingleStaticMeshPackageName)+".uasset";
//		FSaveErrorOutputDevice SaveErrors;
//		GUnrealEd->Exec(NULL, *FString::Printf(TEXT("OBJ SAVEPACKAGE PACKAGE=\"%s\" FILE=\"%s\" SILENT=true"), *ModelPackage->GetName(), *FinalPackageFilename), SaveErrors);
		
		if (LoadedObject == nullptr)
		{
			int32 position = -1;
			FString BodyName = Meshs[0]->GetOutermost()->GetName();
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
			FString BasePackageName1= FileName_EluModel;
			UObject *Result = NewObjects[0];
			FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
			TArray<FAssetRenameData> AssetsAndNames;
			const FString PackagePath = FPackageName::GetLongPackagePath(Result->GetOutermost()->GetName());
			new(AssetsAndNames) FAssetRenameData(Result, PackagePath, BasePackageName1);
			AssetToolsModule.Get().RenameAssets(AssetsAndNames);
			//复制第三步转移
			FString BasePackageName2 = "/load/" + BasePackageName1 + "." + BasePackageName1;
			UObject *Asset2 = StaticLoadObject(UObject::StaticClass(), NULL, *BasePackageName2);

			TArray<UObject*> AssetsIn2;
			TArray<UObject*> NewObjects2;

			AssetsIn2.Add(Asset2);

			ObjectTools::DuplicateObjects(AssetsIn2, TEXT(""), ExportFolder, /*bOpenDialog=*/false, &NewObjects2);

			
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

			ObjectTools::DeleteObjects(AssetsIn2, false);
			UObject *Result2 = NewObjects2[0];
			LoadedObject = Cast<UBlueprint>(Result2);
		}
		LoadedObject->Modify();


	

		/*

		int checkobj = -1;

		for (UStaticMesh * ExistingMat : Meshs)
		{

				FString x = ExistingMat->GetName();
				const FName NewStaticMeshName = *x;
			
				for (int objnum = 0; objnum < OBJS.Num(); objnum++)
				{

					//新需求_变-
					if (OBJS[objnum]->SaveMeshName == x)
					{

						checkobj = objnum;
						break;
					}

				}
				if (checkobj == -1)
				{

					break;
				}
				//新需求_变-
				USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), *x);
			//	USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), NewStaticMeshName);

		//		LoadedObject->SimpleConstructionScript->GetDefaultSceneRootNode()->AddChildNode(NewNode, true);
				if (NewNode->VariableName != NAME_None)
				{
					FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->VariableName);
				}
				UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
				myStaticK->SetStaticMesh(ExistingMat);

				OBJS[checkobj]->OwnNode = NewNode;
				OBJS[checkobj]->OwnComponent =myStaticK;
				if(OBJS.Num()==0)
					LoadedObject->SimpleConstructionScript->GetDefaultSceneRootNode()->AddChildNode(NewNode, true);

//			}//对应之前必须保存的方法
		}//每个Mesh
		if (checkobj == -1)
		{
			FText Message1 = FText::FromString("The model name is wrong!");
			FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
			return;
		}
		*/
	//	return;
		//新需求_变-
		for (int32 k = 0; k < OBJS.Num(); k++)
		{

			if(OBJS[k]->SaveMeshName !="")
			{
		//		UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(OBJS[k]->OwnComponent);



				if (OBJS[k]->ParentName == "RootNode")
				{
					USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), *OBJS[k]->SaveMeshName);
					if (NewNode->GetVariableName() != NAME_None)
					{
						FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->GetVariableName());
					}					
					LoadedObject->SimpleConstructionScript->GetDefaultSceneRootNode()->AddChildNode(NewNode, true);


					UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
					if (CanChangeWithMini)
					{
						UStaticMesh * FirstMesh = ClassOfMeshs.Get(OBJS[k], true)->OwnerMesh;
						myStaticK->SetStaticMesh(FirstMesh);
					}
					else
					{
						myStaticK->SetStaticMesh(OBJS[k]->OwnerMesh);
					}
					myStaticK->SetRelativeLocation(FVector(OBJS[k]->Locat.X, -(OBJS[k]->Locat.Y), OBJS[k]->Locat.Z ));

					myStaticK->SetRelativeRotation(FQuat::MakeFromEuler(FVector(OBJS[k]->Rota.Z, -OBJS[k]->Rota.X, -OBJS[k]->Rota.Y)));
					OBJS[k]->OwnNode = NewNode;
			//		OBJS[k]->OwnComponent = myStaticK;
				}
				else// if(OBJS[k]->ParentNode->SaveMeshName!="")
				{
					if (OBJS[k]->ParentNode!=nullptr)
					{
						USCS_Node* NewNode = LoadedObject->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), *OBJS[k]->SaveMeshName);


						if (NewNode->GetVariableName() != NAME_None)
						{
							FBlueprintEditorUtils::ValidateBlueprintChildVariables(LoadedObject, NewNode->GetVariableName());
						}

						OBJS[k]->ParentNode->OwnNode->AddChildNode(NewNode, true);



						UStaticMeshComponent* myStaticK = Cast<UStaticMeshComponent>(NewNode->ComponentTemplate);
						if (CanChangeWithMini)
						{
							UStaticMesh * FirstMesh = ClassOfMeshs.Get(OBJS[k], true)->OwnerMesh;
							myStaticK->SetStaticMesh(FirstMesh);
						}
						else
						{
							myStaticK->SetStaticMesh(OBJS[k]->OwnerMesh);
						}
				//		myStaticK->SetStaticMesh(OBJS[k]->OwnerMesh);
						//		myStaticK->AttachTo(OBJS[k]->ParentNode->OwnComponent);


						myStaticK->SetRelativeLocation(FVector(OBJS[k]->Locat.X, -(OBJS[k]->Locat.Y), OBJS[k]->Locat.Z));

						myStaticK->SetRelativeRotation(FQuat::MakeFromEuler(FVector(OBJS[k]->Rota.Z, -OBJS[k]->Rota.X, -OBJS[k]->Rota.Y)));

					}
				}
				


			}



		}




	}//每个导入大Mesh名
	LoadedObject->PostEditChange();
	LoadedObject->GetOutermost()->MarkPackageDirty();
	FString SingleStaticMeshPackageName = LoadedObject->GetOutermost()->GetName();
	FString FinalPackageFilename = FPackageName::LongPackageNameToFilename(SingleStaticMeshPackageName) + ".uasset";
	FSaveErrorOutputDevice SaveErrors;
	GUnrealEd->Exec(NULL, *FString::Printf(TEXT("OBJ SAVEPACKAGE PACKAGE=\"%s\" FILE=\"%s\" SILENT=true"), *SingleStaticMeshPackageName, *FinalPackageFilename), SaveErrors);

	LoadedObject->OnCompiled();
	LoadedObject->BroadcastCompiled();





}


bool SImportAndSaveWindow::CanImport()  const
{
	// do test to see if we are ready to import
	return true;

}

#undef LOCTEXT_NAMESPACE
