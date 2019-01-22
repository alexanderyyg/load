//#include "Test_mp.h"
#include "SDDFileTree.h"
#include "Widgets/Layout/SBox.h"
#include "FileManagerGeneric.h"
#include "HAL/FileManager.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "IDocumentation.h"
#include "DDFileTreeItem.h"
#include "Widgets/Text/STextBlock.h"
#include "Runtime/Slate/Public/Framework/Views/ITypedTableView.h"

#define LOCTEXT_NAMESPACE "ContentOption" 

void SDDFileTree::Construct(const FArguments& InArgs)
{
	SelectStr = "";
	WidgetWindow = InArgs._WidgetWindow;
	newPath = InArgs._StringPath;
	//  OwnerHUD = InArgs._OwnerHUD;
	RebuildFileTree(); /* <构建目录树数据 */
	TSharedPtr<SBox> ImportTypeDisplay;
	this->ChildSlot
	[
		SNew(SBox)
		.MaxDesiredHeight(InArgs._MaxWindowHeight)
		.MaxDesiredWidth(InArgs._MaxWindowWidth)
		[
			SNew(SVerticalBox)   //VerticalBox垂直的盒子
			+ SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				SAssignNew(DDFileTreeView, SDDFileTreeView)
				.SelectionMode(ESelectionMode::Single) // 只允许选中一个项目
				.ClearSelectionOnClick(false) // 不允许不选中内容
				.TreeItemsSource(&Directories)
				.OnGenerateRow(this, &SDDFileTree::DDFileTree_OnGenerateRow)
				.OnGetChildren(this, &SDDFileTree::DDFileTree_OnGetChildren)
				.OnSelectionChanged(this, &SDDFileTree::DDFileTree_OnSelectionChanged)
				.OnExpansionChanged(this, &SDDFileTree::DDFileTree_OnExpansionChanged)
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
					SAssignNew(GetButton, SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("OptionWindow_GetContent", "OK"))
					.IsEnabled(this, &SDDFileTree::CanGet)
					.OnClicked(this, &SDDFileTree::OnGet)
				]
				+ SUniformGridPanel::Slot(3, 0)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("OptionWindow_CancelContent", "Cancel"))
					.ToolTipText(LOCTEXT("FbxOptionWindow_Cancel_ToolTip", "Cancels Get this folder"))
					.OnClicked(this, &SDDFileTree::OnCancel)
				]
			]

		]
	];

	//Build the tree view of the above core data
	/*
	DDFileTreeView =
		SNew(SDDFileTreeView)
		.SelectionMode(ESelectionMode::Single) // 只允许选中一个项目
		.ClearSelectionOnClick(false) // 不允许不选中内容
		.TreeItemsSource(&Directories)
		.OnGenerateRow(this, &SDDFileTree::DDFileTree_OnGenerateRow)
		.OnGetChildren(this, &SDDFileTree::DDFileTree_OnGetChildren)
		.OnSelectionChanged(this, &SDDFileTree::DDFileTree_OnSelectionChanged)
		;
*/
	/*
	// Expand the root  by default
	for( auto RootDirIt( Directories.CreateConstIterator() ); RootDirIt; ++RootDirIt )
	{
	  const auto& Dir = *RootDirIt;
	  DDFileTreeView->SetItemExpansion( Dir, true );
	}

	// Select the first item by default
	if( Directories.Num() > 0 )
	{
	  DDFileTreeView->SetSelection( Directories[ 0 ] );
	}

	*/

//	this->ChildSlot.AttachWidget(DDFileTreeView.ToSharedRef());
}
 




bool SDDFileTree::CanGet()  const
{
	// do test to see if we are ready to import
	if (SelectStr != "")
		return true;
	else
		return false;

}

SDDFileTree::~SDDFileTree()
{
 
}
 
void SDDFileTree::DDFileTree_OnExpansionChanged(FDDFileTreeItemPtr Item, bool bIsExpanded)
{
	FDDFileTreeItemPtr rootitem = FindItemRecursive("Content");
		DDFileTreeView->SetSelection(Item);
//	if (ShouldAllowTreeItemChangedDelegate())
//	{

/*		TSet<FDDFileTreeItemPtr> ExpandedItemSet;
		DDFileTreeView->GetExpandedItems(ExpandedItemSet);

		const TArray<FDDFileTreeItemPtr> ExpandedItems = ExpandedItemSet.Array();

		LastExpandedPaths.Empty();
		for (int32 ItemIdx = 0; ItemIdx < ExpandedItems.Num(); ++ItemIdx)
		{
			const FDDFileTreeItemPtr Item = ExpandedItems[ItemIdx];
			if (!ensure(Item.IsValid()))
			{
				// All items must exist
				continue;
			}

			// Keep track of the last paths that we broadcasted for expansion reasons when filtering
			LastExpandedPaths.Add(Item->DirectoryPath);
		}
//	}*/
}

void SDDFileTree::GetFolderChild(FString Folder, TArray<FString> *Childs)
{
	FString ContentFolder;
	int32 len = Folder.Len();
	if(len>7)
		ContentFolder = FPaths::ProjectContentDir() + Folder.Right(len-7) + "/*";
	else
		ContentFolder = FPaths::ProjectContentDir() + "*";
//	TArray<FString> Childs;
	FFileManagerGeneric::Get().FindFiles((*Childs), *ContentFolder, false, true);
	return ;

}
void SDDFileTree::AddFolderChild(FDDFileTreeItemPtr ParentPtr)
{
//	FString ParentDir = Parent.GetDirectoryPath();// ->GetDirectoryPath();// .GetDirectoryPath();//路径长名
	FString ParentDir = ParentPtr->GetDirectoryPath();
	//	FText Message5 = FText::FromString(FString::Printf(TEXT("%s %s"), *ParentDir,*ParentDir2));
	//	FMessageDialog::Open(EAppMsgType::Ok, Message5, &Message5);
//		UE_LOG(LogTemp, Warning, TEXT("Item Selected: %s %s\n"), *ParentDir, *ParentDir2);
	TArray<FString> ChildNames;
	GetFolderChild(ParentDir, &ChildNames);
	FString ChildPath;
	for (int i = 0; i < ChildNames.Num(); i++)
	{
		if ((ParentDir == "Content") && ((ChildNames[i] == "Collections") || (ChildNames[i] == "Developers")))
			continue;

		ChildPath = ParentDir + "/" + (ChildNames)[i];
	//	UE_LOG(LogTemp, Warning, TEXT("child: %s \n"), *(ChildNames)[i]);
		TSharedRef<FDDFileTreeItem> EachSubDir = MakeShareable(new FDDFileTreeItem(ParentPtr, ChildPath, (ChildNames)[i]));
		ParentPtr->AddSubDirectory(EachSubDir);
		AddFolderChild(EachSubDir);

	}
	if (ParentDir == "Content")
	{
//		ParentPtr->展开
	}

}

FDDFileTreeItemPtr SDDFileTree::FindItemRecursive(const FString& Path) const
{
	
	for (auto TreeItemIt = TreeRootItems.CreateConstIterator(); TreeItemIt; ++TreeItemIt)
	{
		if ((*TreeItemIt)->DirectoryPath == Path)
		{
			// This root item is the path
			return *TreeItemIt;
		}
/*
		// Try to find the item under this root
		FDDFileTreeItemPtr Item = (*TreeItemIt)->FindItemRecursive(Path);
		if (Item.IsValid())
		{
			// The item was found under this root
			return Item;
		}
*/
	}

	return nullptr;
}


void SDDFileTree::RebuildFileTree()
{
	Directories.Empty();
	//~~~~~~~~~~~~~~~~~~~
	//Root Level


	TSharedRef<FDDFileTreeItem> RootDir = MakeShareable(new FDDFileTreeItem(NULL, TEXT("Content"), FString("Content")));
	TreeRootItems.Add(RootDir);
	Directories.Add(RootDir);
//	TSharedRef<FDDFileTreeItem> RootDir2 = MakeShareable(new FDDFileTreeItem(NULL, TEXT("Content/"), FString("Content/")));
//	Directories.Add(RootDir2);
	//~~~~~~~~~~~~~~~~~~~


	//Root Category
	FDDFileTreeItemPtr ParentCategory = RootDir;

	AddFolderChild(RootDir);

	/*
	FString ParentPath;
	ParentPath = RootDir->GetDirectoryPath();
	//Add
	FDDFileTreeItemPtr EachSubDir = MakeShareable(new FDDFileTreeItem(RootDir, ParentPath + "/" + "Joy","Joy" ));
	RootDir->AddSubDirectory(EachSubDir);

	//Add
	ParentPath = RootDir->GetDirectoryPath();
	EachSubDir = MakeShareable(new FDDFileTreeItem(RootDir, ParentPath +"/"+ "Song","Song" ));
	RootDir->AddSubDirectory(EachSubDir);

	//Add
	ParentPath = EachSubDir->GetDirectoryPath();
	FDDFileTreeItemPtr SongDir = MakeShareable(new FDDFileTreeItem(EachSubDir, ParentPath + "/" + "Dance","Dance" ));
	EachSubDir->AddSubDirectory(SongDir);

	//Add
	ParentPath = EachSubDir->GetDirectoryPath();
	SongDir = MakeShareable(new FDDFileTreeItem(EachSubDir, ParentPath + "/" + "Rainbows","Rainbows" ));
	EachSubDir->AddSubDirectory(SongDir);
	//Add
	ParentPath = RootDir->GetDirectoryPath();
	EachSubDir = MakeShareable(new FDDFileTreeItem(RootDir,ParentPath + "/" + "Butterflies" ,"Butterflies" ));
	RootDir->AddSubDirectory(EachSubDir);
	*/
	//Refresh

	if (DDFileTreeView.IsValid())
	{
		DDFileTreeView->RequestTreeRefresh();
	}
 
}
 




TSharedRef<ITableRow> SDDFileTree::DDFileTree_OnGenerateRow(FDDFileTreeItemPtr Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!Item.IsValid())
	{
		return SNew(STableRow< FDDFileTreeItemPtr >, OwnerTable)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("Your Namespace", "twns", "THIS WAS NULL SOMEHOW"))
		];

	}

	return SNew(STableRow< FDDFileTreeItemPtr >, OwnerTable)
	[
		SNew(STextBlock)
		.Text(FText::FromString(Item->GetDisplayName()))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 12))
		.ColorAndOpacity(FLinearColor(1, 0, 1, 1))
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FIntPoint(-2, 2))
	];
}
 
void SDDFileTree::DDFileTree_OnGetChildren(FDDFileTreeItemPtr Item, TArray< FDDFileTreeItemPtr >& OutChildren)
{
	const auto& SubCategories = Item->GetSubDirectories();
	OutChildren.Append(SubCategories);
}
 
//Key function for interaction with user!
void SDDFileTree::DDFileTree_OnSelectionChanged(FDDFileTreeItemPtr Item, ESelectInfo::Type SelectInfo)
{
	//Selection Changed!
	SelectStr = Item->GetDirectoryPath();
//	SelectStr = Item->GetDisplayName();

	UE_LOG(LogTemp, Warning, TEXT("Item Selected: %s"), *Item->GetDisplayName());
}
 
FDDFileTreeItemPtr SDDFileTree::GetSelectedDirectory() const
{

	if (DDFileTreeView.IsValid())
	{
		auto SelectedItems = DDFileTreeView->GetSelectedItems();
		if (SelectedItems.Num() > 0)
		{
			const auto& SelectedCategoryItem = SelectedItems[0];
			return SelectedCategoryItem;
		}

	}
	return NULL;
}
 
void SDDFileTree::SelectDirectory(const FDDFileTreeItemPtr& CategoryToSelect)
{
	if (ensure(DDFileTreeView.IsValid()))
	{
		DDFileTreeView->SetSelection(CategoryToSelect);
	}
}
 
//is the tree item expanded to show children?
bool SDDFileTree::IsItemExpanded(const FDDFileTreeItemPtr Item) const
{
	return DDFileTreeView->IsItemExpanded(Item);
}
 
bool SDDFileTree::IsItemSelected(const FDDFileTreeItemPtr Item) const
{
	return DDFileTreeView->IsItemSelected(Item);
}




void SDDFileTree::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// Call parent implementation
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	//can do things here every tick
}
#undef LOCTEXT_NAMESPACE