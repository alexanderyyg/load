#pragma once
 
/* <�ڵ����ݽṹ */
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/Widgets/Images/SThrobber.h"
#include "Widgets/Views/STreeView.h"
#include "DDFileTreeItem.h"
 
typedef STreeView<FDDFileTreeItemPtr> SDDFileTreeView;
 
/**
* <Ŀ¼��Slate
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

	/** <ˢ��Ŀ¼ */

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

	/** @return <���ص�ǰ��ѡ�е�Ŀ¼ */
	FDDFileTreeItemPtr GetSelectedDirectory() const;

	/** <ѡ��Ŀ¼ */
	void SelectDirectory(const FDDFileTreeItemPtr& CategoryToSelect);

	/** @return <���ؽڵ��Ƿ�չ�� */
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
	/** <���ɵ����ڵ�Ԫ�� */
	TSharedRef<ITableRow> DDFileTree_OnGenerateRow(FDDFileTreeItemPtr Item, const TSharedRef<STableViewBase>& OwnerTable);

	/** <����ӽڵ� */
	void DDFileTree_OnGetChildren(FDDFileTreeItemPtr Item, TArray< FDDFileTreeItemPtr >& OutChildren);

	/** <��ѡ������仯ʱ */
	void DDFileTree_OnSelectionChanged(FDDFileTreeItemPtr Item, ESelectInfo::Type SelectInfo);


	void DDFileTree_OnExpansionChanged(FDDFileTreeItemPtr Item, bool bIsExpanded);

	/** <����Ŀ¼������ */
	void RebuildFileTree();

	/** <��дTick�����Ժ�ʵ��Ŀ¼ˢ�� */
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	/** <TreeView�ؼ� */
	TSharedPtr< SDDFileTreeView > DDFileTreeView;

	/** <Ŀ¼�������� */
	TArray< FDDFileTreeItemPtr > Directories;
 
};