// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "AssetRegistryModule.h"
#include "ChangeNamesUI.h"
#include "InputCoreTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Reply.h"
#include "ContentBrowserModule.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWindow.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "ImportAndSaveUI.h"
#include "Runtime/Core/Public/Logging/TokenizedMessage.h"
#include "Runtime/Core/Public/Logging/MessageLog.h"
#include "Runtime/Slate/Public/Widgets/Input/SComboBox.h"

#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Runtime/SlateCore/Public/Types/SlateEnums.h"
#include "AlexStorage.h"
#include "AlexMaterial.h"


class SButton;








class ChangeNamesWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(ChangeNamesWindow)
		: _WidgetWindow()
		, _IsObjFormat(false)
		, _MaxWindowHeight(0.0f)
		, _MaxWindowWidth(0.0f)
		{}
//		: _ImportUI(NULL)
//		, _ImportMaterials(NULL)
//		, _ForcedImportType()
		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( bool, IsObjFormat )
		SLATE_ARGUMENT( float, MaxWindowHeight)
		SLATE_ARGUMENT(float, MaxWindowWidth)
	SLATE_END_ARGS()
//		SLATE_ARGUMENT(UChangeNamesUI*, ImportUI)
//		SLATE_ARGUMENT(TArray<UAlexMaterial> *, ImportMaterials)
//		SLATE_EVENT(FOnPreviewFbxImport, OnPreviewFbxImport)
//		SLATE_ARGUMENT(TOptional<EFBXBuilderImportTypeB>, ForcedImportType)
public:
	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override { return true; }
	FText FrontText;
	FText TailText;
	FText ChangeText;

	TSharedPtr<FString> CurrentItem;
	TArray<TSharedPtr<FString>> Options;
//	TSharedPtr<SComboBox> MyComboBox;

	TSharedRef<SWidget> MakeWidgetForOption(TSharedPtr<FString> InOption);
	void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type);
	FText GetCurrentItemLabel() const;

	TSharedPtr<FString> CurrentItem2;
	TArray<TSharedPtr<FString>> Options2;
	//	TSharedPtr<SComboBox> MyComboBox;

	TSharedRef<SWidget> MakeWidgetForOption2(TSharedPtr<FString> InOption);
	void OnSelectionChanged2(TSharedPtr<FString> NewValue, ESelectInfo::Type);
	FText GetCurrentItemLabel2() const;

	void OnNumChanged(int32 value);

	int32 startnum;
	int32 WidthNum;
//	void OnCheckStateChanged();


	bool ChangeCheck = false;
	bool AddNumCheck = false;
	void OnInputChanged(const FText& InText, ETextCommit::Type);
	void OnInputChanged1(const FText& InText, ETextCommit::Type);
	void OnInputChanged2(const FText& InText, ETextCommit::Type);
//	void SetInputPath(const FText& NewText, ETextCommit::Type);
	void OnChangeCheckStateChanged(ECheckBoxState NewCheckedState);
	void OnAddNumCheckStateChanged(ECheckBoxState NewCheckedState);
	void OnNumWidthChanged(int32 value);


	FText GetInputText() const;
	FText GetInputText1() const;
	FText GetInputText2() const;

	FString MeshAddPath;
	FString PicAddPath;
	FString MatAddPath;

	void SetNames();

	FReply OnSet()
	{
		this->SetNames();
		bShouldImport = true;
		if ( WidgetWindow.IsValid() )
		{
			WidgetWindow.Pin()->RequestDestroyWindow();
		}
		return FReply::Handled();
	}

	FReply OnImportAll()
	{
		bShouldImportAll = true;
		return OnSet();
	}

	FReply OnCancel()
	{
		bShouldImport = false;
		bShouldImportAll = false;
		if ( WidgetWindow.IsValid() )
		{
			WidgetWindow.Pin()->RequestDestroyWindow();
		}
		return FReply::Handled();
	}

	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override
	{
		if( InKeyEvent.GetKey() == EKeys::Escape )
		{
			return OnCancel();
		}

		return FReply::Unhandled();
	}

	bool ShouldImport() const
	{
		return bShouldImport;
	}

	bool ShouldImportAll() const
	{
		return bShouldImportAll;
	}
	/*
	ChangeNamesWindow()
		: ImportUI(NULL)
		, bShouldImport(false)
		, bShouldImportAll(false)
	{}
	*/
	//listview²¿·Ö

	FReply ButtonPressed();
	FString InputText;
	/* Adds a new textbox with the string to the list */
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);

	/* The list of strings */
	TArray<TSharedPtr<FString>> Items;

	/* The actual UI list */
	TSharedPtr< SListView< TSharedPtr<FString> > > ListViewWidget;

	FReply OnGetFolderName();
	FReply OnGetExportFolderName();

	UMaterialInstanceConstant* CreateMaterialInstance(FString MaterialName, FString ExportPath);
//	TArray<UAlexMaterial>* UMs;
private:

	bool CanSet() const;
	FReply OnPreviewClick() const;
	FText GetImportTypeDisplayText() const;

private:
//	UChangeNamesUI*	ImportUI;
	TSharedPtr<class IDetailsView> DetailsView;
	TWeakPtr< SWindow > WidgetWindow;
	TSharedPtr< SButton > ImportButton;
	bool			bShouldImport;
	bool			bShouldImportAll;
	bool			bIsObjFormat;

//	FOnPreviewFbxImport OnPreviewFbxImport;
};
