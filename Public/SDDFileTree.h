#pragma once
 
/* <节点数据结构 */
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/Widgets/Images/SThrobber.h"
#include "Widgets/Views/STreeView.h"
#include "DDFileTreeItem.h"
 
typedef STreeView<FDDFileTreeItemPtr> SDDFileTreeView;
 
/**
* <目录树Slate
*/
class strPath
{
public:
	strPath()
	{

	};
	FText InText;
};
typedef strPath * PathPtr;
 
class SDDFileTree : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SDDFileTree) 
		: _WidgetWindow()
		, _StringPath()
		, _MaxWindowHeight(0.0f)
		, _MaxWindowWidth(0.0f)
		{}
		SLATE_ARGUMENT(TSharedPtr<SWindow>, WidgetWindow)
		SLATE_ARGUMENT(PathPtr, StringPath)
		SLATE_ARGUMENT(float, MaxWindowHeight)
		SLATE_ARGUMENT(float, MaxWindowWidth)
	SLATE_END_ARGS()
//	SLATE_ARGUMENT(TWeakObjectPtr<class AHUD>, OwnerHUD)
public:
	TWeakPtr< SWindow > WidgetWindow;
	//TWeakObjectPtr<AHUD> OwnerHUD;

	/** <刷新目录 */

	//bool DoRefresh;

public:
	TSharedPtr< SButton > GetButton;
	void Construct(const FArguments& InArgs);
	PathPtr  newPath;
	FString  SelectStr;

	TSet< FString > LastExpandedPaths;
	TArray<TSharedRef<FDDFileTreeItem>> TreeRootItems;

	FDDFileTreeItemPtr FindItemRecursive(const FString& Path) const;
//	TSharedRef<FDDFileTreeItem> RootDir;
	/** Destructor */
	~SDDFileTree();

	/** @return <返回当前被选中的目录 */
	FDDFileTreeItemPtr GetSelectedDirectory() const;

	/** <选择目录 */
	void SelectDirectory(const FDDFileTreeItemPtr& CategoryToSelect);

	/** @return <返回节点是否展开 */
	bool IsItemExpanded(const FDDFileTreeItemPtr Item) const;


	bool IsItemSelected(const FDDFileTreeItemPtr Item) const;

	void AddFolderChild( FDDFileTreeItemPtr ParentPtr);
	void GetFolderChild(FString Folder, TArray<FString> *Childs);



	FReply OnGet()
	{
	//	this->ImportFBXAndMaterial();
	//	bShouldImport = true;
		newPath->InText = FText::FromString(SelectStr);

		if (WidgetWindow.IsValid())
		{
			WidgetWindow.Pin()->RequestDestroyWindow();
		}
		return FReply::Handled();
	}

	FReply OnCancel()
	{
	//	bShouldImport = false;
	//	bShouldImportAll = false;
		if (WidgetWindow.IsValid())
		{
			WidgetWindow.Pin()->RequestDestroyWindow();
		}
		return FReply::Handled();
	}

private:
	bool CanGet() const;
	/** <生成单个节点元素 */
	TSharedRef<ITableRow> DDFileTree_OnGenerateRow(FDDFileTreeItemPtr Item, const TSharedRef<STableViewBase>& OwnerTable);

	/** <获得子节点 */
	void DDFileTree_OnGetChildren(FDDFileTreeItemPtr Item, TArray< FDDFileTreeItemPtr >& OutChildren);

	/** <当选中项发生变化时 */
	void DDFileTree_OnSelectionChanged(FDDFileTreeItemPtr Item, ESelectInfo::Type SelectInfo);


	void DDFileTree_OnExpansionChanged(FDDFileTreeItemPtr Item, bool bIsExpanded);

	/** <构建目录树数据 */
	void RebuildFileTree();

	/** <重写Tick方便以后实现目录刷新 */
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	/** <TreeView控件 */
	TSharedPtr< SDDFileTreeView > DDFileTreeView;

	/** <目录树的数据 */
	TArray< FDDFileTreeItemPtr > Directories;
 
};