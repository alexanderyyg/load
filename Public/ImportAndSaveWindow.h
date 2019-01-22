// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "AssetRegistryModule.h"
#include "ImportAndSaveUI.h"
#include "InputCoreTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Reply.h"
#include "ContentBrowserModule.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWindow.h"
#include "SDDFileTree.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"

#include "Runtime/Core/Public/Logging/TokenizedMessage.h"
#include "Runtime/Core/Public/Logging/MessageLog.h"


#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Runtime/SlateCore/Public/Types/SlateEnums.h"
#include "AlexStorage.h"
#include "AlexMaterial.h"

UENUM(BlueprintType)
enum class EImportResult : uint8
{
	Failure,
	Success,
	Cancelled
};

class SButton;



class FSaveErrorOutputDevice : public FOutputDevice
{
public:
	virtual void Serialize(const TCHAR* InData, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		if (Verbosity == ELogVerbosity::Error || Verbosity == ELogVerbosity::Warning)
		{
			EMessageSeverity::Type Severity = EMessageSeverity::Info;
			if (Verbosity == ELogVerbosity::Error)
			{
				Severity = EMessageSeverity::Error;
			}
			else if (Verbosity == ELogVerbosity::Warning)
			{
				Severity = EMessageSeverity::Warning;
			}

			if (ensure(Severity != EMessageSeverity::Info))
			{
				ErrorMessages.Add(FTokenizedMessage::Create(Severity, FText::FromName(InData)));
			}
		}
	}

	virtual void Flush() override
	{
		if (ErrorMessages.Num() > 0)
		{
			FMessageLog EditorErrors("EditorErrors");
			EditorErrors.NewPage(FText::FromString("Save Output"));
			EditorErrors.AddMessages(ErrorMessages);
			EditorErrors.Open();
			ErrorMessages.Empty();
		}
	}

private:
	// Holds the errors for the message log.
	TArray< TSharedRef< FTokenizedMessage > > ErrorMessages;
};





class SImportAndSaveWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImportAndSaveWindow)
		: _WidgetWindow()
		, _ForcedImportType()
		, _IsObjFormat(false)
		, _MaxWindowHeight(0.0f)
		, _MaxWindowWidth(0.0f)
		{}
//		: _ImportUI(NULL)
//		, _ImportMaterials(NULL)

		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( TOptional<EFBXBuilderImportTypeB>, ForcedImportType )
		SLATE_ARGUMENT( bool, IsObjFormat )
		SLATE_ARGUMENT( float, MaxWindowHeight)
		SLATE_ARGUMENT(float, MaxWindowWidth)
	SLATE_END_ARGS()
//		SLATE_ARGUMENT(UImportAndSaveUI*, ImportUI)
//		SLATE_ARGUMENT(TArray<UAlexMaterial> *, ImportMaterials)
//		SLATE_EVENT(FOnPreviewFbxImport, OnPreviewFbxImport)

public:
	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override { return true; }
	FText InputFilePath;
	FText OutputFilePath;

	strPath  newPath;
	void OnInputChanged(const FText& InText, ETextCommit::Type);
	void OnExportChanged(const FText& InText, ETextCommit::Type);
//	void SetInputPath(const FText& NewText, ETextCommit::Type);
	FText GetInputText() const;
	FText GetExportText() const;

	FString MeshAddPath;
	FString PicAddPath;
	FString MatAddPath;

	void ImportFBXAndMaterial();
	FReply OnImport()
	{
		this->ImportFBXAndMaterial();
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
		return OnImport();
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
	SImportAndSaveWindow()
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

	FReply OnGetSavePath();
//	FReply OnGetExportFolderName();
//	TArray<UAlexMaterial>* UMs;
private:

	bool CanImport() const;
	FReply OnPreviewClick() const;
	FText GetImportTypeDisplayText() const;

private:
//	UImportAndSaveUI*	ImportUI;
	TSharedPtr<class IDetailsView> DetailsView;
	TWeakPtr< SWindow > WidgetWindow;
	TSharedPtr< SButton > ImportButton;
	bool			bShouldImport;
	bool			bShouldImportAll;
	bool			bIsObjFormat;

//	FOnPreviewFbxImport OnPreviewFbxImport;
};
