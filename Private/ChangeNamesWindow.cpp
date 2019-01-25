// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "ChangeNamesWindow.h"
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
#include "Editor/UnrealEd/Public/Tests/AutomationEditorCommon.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "DesktopPlatformModule.h"
#include <string>
#include "Factories/TextureFactory.h"
//#include "FbxBuilderImporter.h"
#include "FbxBuilderFactory.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
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

#define LOCTEXT_NAMESPACE "ChangeOption"





void ChangeNamesWindow::Construct(const FArguments& InArgs)
{
//	ImportUI = InArgs._ImportUI;
	WidgetWindow = InArgs._WidgetWindow;
	bIsObjFormat = InArgs._IsObjFormat;
//	UMs = InArgs._ImportMaterials;
//	OnPreviewFbxImport = InArgs._OnPreviewFbxImport;
	WidthNum = 2;
//	check (ImportUI);
	
	Options.Add(MakeShareable(new FString("Add Before")));
	Options.Add(MakeShareable(new FString("Delete")));
	Options.Add(MakeShareable(new FString("Delete First _")));

	CurrentItem = Options[0];

	Options2.Add(MakeShareable(new FString("Add After")));
	Options2.Add(MakeShareable(new FString("Delete")));
	Options2.Add(MakeShareable(new FString("Delete Last _")));
	CurrentItem2 = Options2[0];



	TSharedPtr<SBox> ImportTypeDisplay;
	TSharedPtr<SBox> ImportTypeDisplay1;
	TSharedPtr<SBox> ImportTypeDisplay2;
	TSharedPtr<SBox> ImportTypeDisplay3;
	TSharedPtr<SHorizontalBox> FbxHeaderButtons;
	TSharedPtr<SBox> InspectorBox;

	MeshAddPath="/";
//	MeshAddPath = "/Mesh/";

	PicAddPath="/";
//	PicAddPath = "/Texture/";

	MatAddPath="/";

	startnum = 0;
//	MatAddPath = "/Material/";

	FrontText = FText::FromString("");
	TailText = FText::FromString("");
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
				SAssignNew(ImportTypeDisplay1, SBox)	//盒子组合
			]
			+ SVerticalBox::Slot()//.FillHeight(1)	//挂点1 位置
			.AutoHeight()
			.Padding(2)	//填充
			[
				SAssignNew(ImportTypeDisplay2, SBox)	//盒子组合
			]
			+ SVerticalBox::Slot()//.FillHeight(1)	//挂点1 位置
			.AutoHeight()
			.Padding(2)	//填充
			[
				SAssignNew(ImportTypeDisplay3, SBox)	//盒子组合
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
				+ SUniformGridPanel::Slot(2, 0)
				[
					SAssignNew(ImportButton, SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("AssetOptionWindow_Set", "Set"))
					.IsEnabled(this, &ChangeNamesWindow::CanSet)
					.OnClicked(this, &ChangeNamesWindow::OnSet)
				]
				+ SUniformGridPanel::Slot(3, 0)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("AssetOptionWindow_Cancel", "Cancel"))
					.ToolTipText(LOCTEXT("FbxOptionWindow_Cancel_ToolTip", "Cancels importing this FBX file"))
					.OnClicked(this, &ChangeNamesWindow::OnCancel)
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
				.Text(LOCTEXT("First_CurrentFileTitle", "Begin :"))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点 Combobox
			.Padding(5, 0, 0, 0)
			.MaxWidth(300.f)
			.Padding(2)
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&Options)
				.OnSelectionChanged(this, &ChangeNamesWindow::OnSelectionChanged)
				.OnGenerateWidget(this, &ChangeNamesWindow::MakeWidgetForOption)
				.InitiallySelectedItem(CurrentItem)
				[
					SNew(STextBlock)
					.Text(this, &ChangeNamesWindow::GetCurrentItemLabel)
				]
			]
			+ SHorizontalBox::Slot() //挂点4 
			.Padding(5, 0, 0, 0)
			.MaxWidth(300.f)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(SEditableTextBox)
				.MinDesiredWidth(200.f)
				.Text(this, &ChangeNamesWindow::GetInputText)//自设ddd   InArgs._FullPath
				.OnTextCommitted(this, &ChangeNamesWindow::OnInputChanged)
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
				.Text(LOCTEXT("Last_CurrentFileTitle", "End    :"))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点 Combobox
			.Padding(5, 0, 0, 0)
			.MaxWidth(300.f)
			.Padding(2)
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&Options2)
				.OnSelectionChanged(this, &ChangeNamesWindow::OnSelectionChanged2)
				.OnGenerateWidget(this, &ChangeNamesWindow::MakeWidgetForOption2)
				.InitiallySelectedItem(CurrentItem2)
				[
					SNew(STextBlock)
					.Text(this, &ChangeNamesWindow::GetCurrentItemLabel2)
				]
			]
			+ SHorizontalBox::Slot() //挂点4 
			.Padding(5, 0, 0, 0)
			.MaxWidth(300.f)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(SEditableTextBox)
				.MinDesiredWidth(200.f)
				.Text(this, &ChangeNamesWindow::GetInputText2)//自设ddd   InArgs._FullPath
				.OnTextCommitted(this, &ChangeNamesWindow::OnInputChanged2)
			]

		]
	);



	ImportTypeDisplay3->SetContent(
		SNew(SBorder)
		.Padding(FMargin(3))//外边距
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SHorizontalBox)	//水平盒子
			+ SHorizontalBox::Slot() //挂点3
			.AutoWidth()
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(STextBlock)	//文字控件 文件路径
				.Font(FEditorStyle::GetFontStyle("CurveEd.LabelFont"))
				.Text(LOCTEXT("Start_NumTitle", "Add Num From: "))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点3
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			.MaxWidth(50.f)
			[
				SNew(SCheckBox)	//文字控件 文件路径
				.OnCheckStateChanged(this, &ChangeNamesWindow::OnAddNumCheckStateChanged)
			]
			+ SHorizontalBox::Slot() //挂点 Combobox
			.Padding(5, 0, 0, 0)
			.MaxWidth(50.f)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(SSpinBox<int>)
				.MinDesiredWidth(50.f)
				.MaxValue(5000)
				.MinValue(0)
				.OnValueChanged(this, &ChangeNamesWindow::OnNumChanged)

			]
			+ SHorizontalBox::Slot() //挂点3
			.AutoWidth()
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(STextBlock)	//文字控件 文件路径
				.Font(FEditorStyle::GetFontStyle("CurveEd.LabelFont"))
				.Text(LOCTEXT("Width_NumTitle", "   Num Width: "))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点 Combobox
			.Padding(5, 0, 0, 0)
			.MaxWidth(50.f)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(SSpinBox<int>)
				.MinDesiredWidth(50.f)
				.MaxValue(5)
				.MinValue(1)
				.Value(2)
				.OnValueChanged(this, &ChangeNamesWindow::OnNumWidthChanged)

			]
		]
	);

	ImportTypeDisplay1->SetContent(
		SNew(SBorder)
		.Padding(FMargin(3))//外边距
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SHorizontalBox)	//水平盒子
			+ SHorizontalBox::Slot() //挂点3
			.AutoWidth()
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(STextBlock)	//文字控件 文件路径
				.Font(FEditorStyle::GetFontStyle("CurveEd.LabelFont"))
				.Text(LOCTEXT("Middle_StrTitle", "Change Middle: "))//当前文档
			]
			+ SHorizontalBox::Slot() //挂点3
			.AutoWidth()
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			.AutoWidth()
			[
				SNew(SCheckBox)	//文字控件 文件路径
				.OnCheckStateChanged(this, &ChangeNamesWindow::OnChangeCheckStateChanged)
			]
			+ SHorizontalBox::Slot() //挂点4 
			.Padding(5, 0, 0, 0)
			.MaxWidth(300.f)
			.VAlign(VAlign_Center)	//垂直对齐中心对齐
			[
				SNew(SEditableTextBox)
				.MinDesiredWidth(200.f)
				.Text(this, &ChangeNamesWindow::GetInputText1)//自设ddd   InArgs._FullPath
				.OnTextCommitted(this, &ChangeNamesWindow::OnInputChanged1)
			]

		]

	);


}

void ChangeNamesWindow::OnChangeCheckStateChanged(ECheckBoxState NewCheckedState)
{
	ChangeCheck = (NewCheckedState == ECheckBoxState::Checked);
}

void ChangeNamesWindow::OnAddNumCheckStateChanged(ECheckBoxState NewCheckedState)
{
	AddNumCheck = (NewCheckedState == ECheckBoxState::Checked);
}

void ChangeNamesWindow::OnNumChanged(int32 value)
{
	startnum = value;
//	FText Message1 = FText::FromString(FString::Printf(*FString("FileName=%d\n"), value));
//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);	
}

void ChangeNamesWindow::OnNumWidthChanged(int32 value)
{
	WidthNum = value;
	//	FText Message1 = FText::FromString(FString::Printf(*FString("FileName=%d\n"), value));
	//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);	
}

TSharedRef<SWidget> ChangeNamesWindow::MakeWidgetForOption(TSharedPtr<FString> InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

void ChangeNamesWindow::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	CurrentItem = NewValue;
}

FText ChangeNamesWindow::GetCurrentItemLabel() const
{
	if (CurrentItem.IsValid())
	{
		return FText::FromString(*CurrentItem);
	}

	return LOCTEXT("InvalidComboEntryText", "<<Invalid option>>");
}


TSharedRef<SWidget> ChangeNamesWindow::MakeWidgetForOption2(TSharedPtr<FString> InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

void ChangeNamesWindow::OnSelectionChanged2(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	CurrentItem2 = NewValue;
}

FText ChangeNamesWindow::GetCurrentItemLabel2() const
{
	if (CurrentItem2.IsValid())
	{
		return FText::FromString(*CurrentItem2);
	}

	return LOCTEXT("InvalidComboEntryText", "<<Invalid option>>");
}






FReply ChangeNamesWindow::OnPreviewClick() const
{
	//Pop a preview window to let the user see the content of the fbx file
	return FReply::Handled();
}


void ChangeNamesWindow::OnInputChanged(const FText& InText, ETextCommit::Type)
{
	FrontText = InText;

}

FText ChangeNamesWindow::GetInputText() const
{
	return (FrontText);
}



void ChangeNamesWindow::OnInputChanged1(const FText& InText, ETextCommit::Type)
{
	ChangeText = InText;

}

FText ChangeNamesWindow::GetInputText1() const
{
	return (ChangeText);
}

void ChangeNamesWindow::OnInputChanged2(const FText& InText, ETextCommit::Type)
{
	TailText = InText;

}

FText ChangeNamesWindow::GetInputText2() const
{
	return (TailText);
}

FReply ChangeNamesWindow::OnGetExportFolderName()
{
	return FReply::Handled();
}
//进行信息搜集
FReply ChangeNamesWindow::OnGetFolderName()
{

	bool bOpened = true;
//	UAlexStorage * AlexStorageTool;
	FString OpenFolderIn;

//	AlexStorageTool = new UAlexStorage();




//	DetailsView->SetObject(ImportUI, true);
	return FReply::Handled();
}



void ChangeNamesWindow::SetNames()
{
	//初始化材质对象列表
//	TArray<UAlexMaterial> UMs;
	FString FrontStr = FrontText.ToString();
	FString TailStr = TailText.ToString();

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	TArray<UObject*> Objs;
	UObject* myObj;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	int32 curNum = startnum;
	FText Message1;
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		myObj = Cast<UObject>(SelectedAssets[i].GetAsset());
		Objs.Add(myObj);

		FString OldName = myObj->GetName();

//		Message1 = FText::FromString(FString::Printf(TEXT("%s"), *OldName));
//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		int32 oldnamelen = OldName.Len();

		FString NewName;
		FString BodyName;
		FString firststyle = GetCurrentItemLabel().ToString();
//		Message1 = FText::FromString(FString::Printf(TEXT("%s"), *firststyle));
//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		if (firststyle == "Add Before")
		{
			NewName = FrontStr + OldName;
			BodyName = OldName;
		}
		else if(firststyle=="Delete")
		{
			if (OldName.StartsWith(FrontStr, ESearchCase::CaseSensitive))
			{
				NewName = OldName.Right(oldnamelen - FrontStr.Len());
				BodyName=OldName.Right(oldnamelen - FrontStr.Len());
			}
			else
			{
				NewName = FrontStr ;
				BodyName = OldName;
			}
		}
		else if (firststyle == "Delete First _")
		{
			int strpos= OldName.Find("_", ESearchCase::IgnoreCase, ESearchDir::FromStart)+1;
			if (strpos != -1)
			{
				NewName = OldName.Right(oldnamelen - strpos);
				BodyName=OldName.Right(oldnamelen - strpos);
			}
			else
			{
				NewName = FrontStr;
				BodyName = OldName;
			}
		}



		FString NewName2;
		FString BodyName2;
		int32 bodylen = BodyName.Len();
		int32 newnamelen = NewName.Len();
		FString laststyle = GetCurrentItemLabel2().ToString();
		if (laststyle == "Add After")
		{
			BodyName2 = BodyName;
			NewName2 = NewName + TailStr;
		}
		else if (laststyle == "Delete")
		{
			if (NewName.EndsWith(TailStr,ESearchCase::CaseSensitive))
			{
				NewName2 = NewName.Left(newnamelen - TailStr.Len());
				BodyName2 = BodyName.Left(bodylen - TailStr.Len());
			}
			else
			{
				BodyName2 = BodyName;
				NewName2 = NewName ;

			}
		}
		else if (laststyle == "Delete Last _")
		{
			int strpos = NewName.Find("_", ESearchCase::IgnoreCase, ESearchDir::FromEnd) ;

			if ((strpos != -1)&&(strpos>= newnamelen- oldnamelen))
			{
				NewName2 = NewName.Left(strpos);
				BodyName2 = BodyName.Left(bodylen- newnamelen + strpos+1);
			}
			else
			{
				BodyName2 = BodyName;
				NewName2 = NewName;
			}
		}
		FString NewName3;
		if (ChangeCheck)
		{
			FString ChangeStr = ChangeText.ToString();
			NewName3=NewName2.Replace(*BodyName2, *ChangeStr, ESearchCase::CaseSensitive);
		}
		else
		{
			NewName3 = NewName2;
		}
		FString NewNameFinal;
		if (AddNumCheck)
		{
			FString NumStrR = FString::Printf(TEXT("%d"), curNum);
			int rw = NumStrR.Len();
			int cylzero = WidthNum - rw;
			for (int i=0;i<cylzero;i++)
			{
				NewName3 = NewName3 + "0";
			}
			NewNameFinal = NewName3 + NumStrR;// FString::Printf(TEXT("%s%d"), *NewName3, curNum);
		}
		else
		{

			NewNameFinal = NewName3;
		}
//		Message1 = FText::FromString(FString::Printf(TEXT("%s"), *NewNameFinal));
//		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		curNum++;
		
		TArray<FAssetRenameData> AssetsAndNames;
		const FString PackagePath = FPackageName::GetLongPackagePath(myObj->GetOutermost()->GetName());
		new(AssetsAndNames) FAssetRenameData(myObj, PackagePath, NewNameFinal);
		AssetToolsModule.Get().RenameAssets(AssetsAndNames);



	}

/*
	FString OpenFolder = FrontText.ToString();
	FString ExportFolder1;
	if (OpenFolder.StartsWith("/"))
		ExportFolder1 = TEXT("/Game") + OpenFolder;
	else
		ExportFolder1 = TEXT("/Game/") + OpenFolder;

	if (!ExportFolder1.StartsWith("/Game") || ExportFolder1.Contains(" ") || ExportFolder1.Contains("\\") || ExportFolder1.Contains(":") || ExportFolder1.Contains(":") || ExportFolder1.Contains("*") || ExportFolder1.Contains("?") || ExportFolder1.Contains("\"") || ExportFolder1.Contains("<") || ExportFolder1.Contains(">") || ExportFolder1.Contains("|") || ExportFolder1.Contains("'") || ExportFolder1.Contains(",") || ExportFolder1.Contains(".") || ExportFolder1.Contains("&") || ExportFolder1.Contains("!") || ExportFolder1.Contains("~") || ExportFolder1.Contains("@") || ExportFolder1.Contains("#") || ExportFolder1.Contains("//"))
	{
		FText Message = FText::FromString("Message");
		FText Message1 = FText::FromString("Material Folder is not correct!");
		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message);
		return;
	}
	FString MatFolder;
	int32 Exlen = ExportFolder1.Len();
	if (ExportFolder1.EndsWith("/"))
		MatFolder = ExportFolder1.Left(Exlen - 1);
	else
		MatFolder = ExportFolder1;
*/
/*
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
*/

/*
	FString GetChar = TailText.ToString();
	FString ExportFolder2;
	if (GetChar.StartsWith("/"))
		ExportFolder2 = TEXT("/Game") + GetChar;
	else
		ExportFolder2 = TEXT("/Game/") + GetChar;

	if (!ExportFolder2.StartsWith("/Game") || ExportFolder2.Contains(" ") || ExportFolder2.Contains("\\") || ExportFolder2.Contains(":") || ExportFolder2.Contains(":") || ExportFolder2.Contains("*") || ExportFolder2.Contains("?")|| ExportFolder2.Contains("\"") || ExportFolder2.Contains("<") || ExportFolder2.Contains(">") || ExportFolder2.Contains("|") || ExportFolder2.Contains("'") || ExportFolder2.Contains(",") || ExportFolder2.Contains(".") || ExportFolder2.Contains("&") || ExportFolder2.Contains("!") || ExportFolder2.Contains("~") || ExportFolder2.Contains("@") || ExportFolder2.Contains("#") || ExportFolder2.Contains("//"))
	{
		FText Message = FText::FromString("Message");
		FText Message1 = FText::FromString("Pic Folder is not correct!");
		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message);
		return ;
	}
	FString PicFolder;
	Exlen = ExportFolder2.Len();
	if (ExportFolder2.EndsWith("/"))
		PicFolder = ExportFolder2.Left(Exlen - 1);
	else
		PicFolder = ExportFolder2;


	FString ObjectPath;
	TArray<FString> ImportedFileNames;
	TArray<FString> MeshNames;
	FString BasePackageName;
	FString TextureName;
	UStaticMesh* StaticMeshFirst = nullptr;





//	AlexStorageTool->LoadPic(OpenFolder, ExportFolder,&UMs, &ImportedFileNames);




	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	TArray<UStaticMesh*> Objs;
	UStaticMesh* myObj;




	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
	for (int32 i = 0; i < SelectedAssets.Num(); i++)
	{
		if (SelectedAssets[i].GetClass() == UStaticMesh::StaticClass())
		{
			myObj = Cast<UStaticMesh>(SelectedAssets[i].GetAsset());
			Objs.Add(myObj);
		//	MeshNames.Add(myObj->GetName());
		//	Asset
		//	ImportedFileNames.Add(myObj->GetPathName());
	
		//	FText Message1 = FText::FromString(myObj->GetPathName());
		//	FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		//	FString Name = myObj->GetPathName();
			FString Name0 = myObj->GetName();
			FString Name = "";
			int32 lenofname = Name0.Len();
			if (Name0.EndsWith("_SM"))
				Name = Name0.Left(lenofname - 3);
			else
				continue;
			FString TextureFileName = PicFolder+"/" + Name+"_T."+Name+"_T";
			FStringAssetReference DiffuseAssetPath(TextureFileName);
			UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
			if (A == nullptr)
			{

				continue;
			}
			FString MatName =  Name + "_M";

			UMaterialInstanceConstant * UnrealMaterialInstance = CreateMaterialInstance(MatName, MatFolder);
			if (UnrealMaterialInstance == nullptr)
				break;
			UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("Texture"), A);

			for (FStaticMaterial& MeshMat : myObj->StaticMaterials)
			{


				MeshMat.MaterialInterface = UnrealMaterialInstance;

				

			}


		}

	}
	UWorld* world = GEditor->GetEditorWorldContext().World();
*/

	/*
	for (TActorIterator<AStaticMeshActor> ActorItr(world); ActorItr; ++ActorItr)
	{
		AStaticMeshActor* actor = *ActorItr;
		if (ActorItr->IsSelected())
		{		
			FString Name0 = ActorItr->GetActorLabel();
		//	ActorItr->SetActorLabel();
		//	FString NameNum = ActorItr->GetName();

		//	int32 positon2=NameNum.Find("_", ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		//	FString Name0 = NameNum.Left(positon2);

	
			int32 lenofname = Name0.Len();
			FString Name;
		
	//		FText title_text1;
	//		title_text1 = FText::FromString(FString::Printf(TEXT("%s"), *Name0));
	//		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);

			if (Name0.EndsWith("_SM"))
				Name = Name0.Left(lenofname - 3);
			else
				continue;
			FString TextureFileName = PicFolder + "/" + Name + "_T." + Name + "_T";
			FStringAssetReference DiffuseAssetPath(TextureFileName);
			UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
			if (A == nullptr)
			{
				continue;
			}
			FString MatName = Name + "_M";

			UMaterialInstanceConstant * UnrealMaterialInstance = CreateMaterialInstance(MatName, MatFolder);
			if (UnrealMaterialInstance == nullptr)
				break;
			UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("Texture"), A);


			int matNum = actor->GetStaticMeshComponent()->GetNumMaterials();
			for (int i = 0; i < matNum; i++)
			{
				actor->GetStaticMeshComponent()->SetMaterial(i, UnrealMaterialInstance);
	//			FString Name = "";
	//			actor->MakeMIDForMaterial()
			}
		}
	}

*/

}





UMaterialInstanceConstant* ChangeNamesWindow::CreateMaterialInstance(FString MaterialName, FString ExportPath)
{
	FString MatAddPath = "/";
	FString ExportFolder = ExportPath;
//	FString baseName = "MetaM2";
	FString baseName = "M2";
	FString PackageName1 = ExportFolder + MatAddPath;


	FString PackageName = "/Game/Resources/Materials/";//"/load" + MatAddPath + "base/";


	FString MaterialParent = "Material'" + PackageName + baseName + "." + baseName + "'";

	UMaterial * UnrealMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), UMaterial::StaticClass(),
		*MaterialParent));
	if (UnrealMaterial == nullptr)
	{
		FText Message1 = FText::FromString("BaseMaterial:Content/Resources/Materials/M2 is Empty");
		FMessageDialog::Open(EAppMsgType::Ok, Message1, &Message1);
		return nullptr;
	}

	FString Name = MaterialName;
	PackageName1 += Name;//新材质

	FString Package2 = PackageName1 + "." + Name;
	TArray<UObject*> AssetsIn2;
	FText title_text1;
/*	UObject *Asset2 = StaticLoadObject(UObject::StaticClass(), NULL, *Package2);

	title_text1 = FText::FromString(FString::Printf(TEXT("%s"), *Package2));
	FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);
	*/


	// Form a filter from the paths

//	if (Asset2 == nullptr)
//	{
		FAssetRegistryModule& AssetRegistryModule0 = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		TArray<FString> InPaths0;
		TArray<FAssetData> AssetList0;
		InPaths0.Add(ExportFolder);
		FARFilter Filter0;
		Filter0.bRecursivePaths = true;
		//获取目录资源数据流
		for (int32 PathIdx = 0; PathIdx < InPaths0.Num(); ++PathIdx)
		{
			new (Filter0.PackagePaths) FName(*InPaths0[PathIdx]);
		}


		//		title_text1 = FText::FromString(FString::Printf(TEXT("%s"), *ExportFolder));

		//		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);
		// Query for a list of assets in the selected paths
		AssetRegistryModule0.Get().GetAssets(Filter0, AssetList0);
		for (int32 AssetIdx = 0; AssetIdx < AssetList0.Num(); ++AssetIdx)
		{
			const FAssetData& AssetData = AssetList0[AssetIdx];
			UObject* Obj = AssetData.GetAsset();
			if (Obj)
			{
		//		title_text1 = FText::FromString(FString::Printf(TEXT("%s"), *Obj->GetName()));

		//		FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);

				if (Obj->GetName() == MaterialName)
				{
					UPackage * OldPack = Obj->GetOutermost();
					OldPack->ConditionalBeginDestroy();
					AssetsIn2.Add(Obj);
				}
			}
		}
//	}
//	else // (Asset2 != nullptr)
//	{
//		AssetsIn2.Add(Asset2);

//	}
	if(AssetsIn2.Num()>0)
		ObjectTools::DeleteObjects(AssetsIn2, false);

	int32 exlen = ExportFolder.Len();
	FString MidPath = ExportFolder.Right(exlen-6);
	FString OutPath = FPaths::ProjectContentDir() + MidPath + MatAddPath + Name+".uasset";

//	title_text1 = FText::FromString(FString::Printf(TEXT("%s"), *OutPath));
//	FMessageDialog::Open(EAppMsgType::Ok, title_text1, &title_text1);

	if (FPaths::DirectoryExists(OutPath))
	{

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		PlatformFile.DeleteFile(*OutPath);

	}
	UPackage* Package1;
	if (FPackageName::DoesPackageExist(PackageName1))
	{
		Package1 = LoadPackage(nullptr, *PackageName1, LOAD_None);
	//	if (Package1)
	//	{
	//		Package1->FullyLoad();
	//	}
	}
	{
		Package1 = CreatePackage(NULL, *PackageName1);

	}
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
		return nullptr;

	UTexture * A;


	//针对图片变量

	UnrealMaterialInstance->GetTextureParameterValue(FName("Texture"), A);
	UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("Texture"), A);

	/*
	//针对向量变量
	UnrealMaterialInstance->GetVectorParameterValue(FName("LightColor"), CurrentColor);
	CurrentColor = FLinearColor(1, 1, 1, 1);
	UnrealMaterialInstance->SetVectorParameterValueEditorOnly(FName("LightColor"), CurrentColor);

	//针对数值变量
	p = 0;
	m = 1;
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("Power"), p);
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("MetallicScale"), m);
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("RoughnessScale"), m);
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("OcclusionScale"), m);
*/
	//	UMaterialExpressionVectorParameter
	return (UnrealMaterialInstance);
}


bool ChangeNamesWindow::CanSet()  const
{
	// do test to see if we are ready to import
	return true;

}

#undef LOCTEXT_NAMESPACE
