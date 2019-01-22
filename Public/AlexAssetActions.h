#pragma once
#include "Engine/StaticMesh.h"
#include "Editor/StaticMeshEditor/Public/StaticMeshEditorModule.h"
#include "AssetTypeActions_Base.h"


class FAlexAssetActions :public FAssetTypeActions_Base
{
public:
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override; //系统调用 确认该类型是否有action
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override; //如果上面返回true 那么调用下面 添加菜单项
	virtual FText GetName() const override; //资源在缩略图模式下 鼠标的tip提示
	virtual UClass* GetSupportedClass() const override; //资源缩略图的颜色
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override
	{
		EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

		for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
		{
			auto Mesh = Cast<UStaticMesh>(*ObjIt);
			if (Mesh != NULL)
			{
				IStaticMeshEditorModule* StaticMeshEditorModule = &FModuleManager::LoadModuleChecked<IStaticMeshEditorModule>("StaticMeshEditor");
				StaticMeshEditorModule->CreateStaticMeshEditor(Mode, EditWithinLevelEditor, Mesh);
			}
		}
	}
	virtual bool IsImportedAsset() const override { return true; }//很重要，关系是否能reimport

	void CombineClicked();

	void ReimportClicked();


	virtual class UThumbnailInfo* GetThumbnailInfo(UObject* Asset) const override;

	/** Handler to provide the LOD sub-menu. Hides away LOD actions - includes Import LOD sub menu */
	void GetLODMenu(class FMenuBuilder& MenuBuilder, TArray<TWeakObjectPtr<UStaticMesh>> Meshes);

	/** Handler to provide the list of LODs that can be imported or reimported */
	void GetImportLODMenu(class FMenuBuilder& MenuBuilder, TArray<TWeakObjectPtr<UStaticMesh>> Objects);

	/** Handler for calling import methods */
	static void ExecuteImportMeshLOD(UObject* Mesh, int32 LOD);

	/** Handler for when CopyLODDData is selected */
	void ExecuteCopyLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects);

	/** Whether there is a valid static mesh to copy LOD from */
	bool CanCopyLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects) const;

	/** Handler for when PasteLODDData is selected */
	void ExecutePasteLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects);

	/** Whether there is a valid static meshes to copy LOD to*/
	bool CanPasteLODSettings(TArray<TWeakObjectPtr<UStaticMesh>> Objects) const;

	/** Handler for when SaveGeneratedLODsInPackage is selected */
	void ExecuteSaveGeneratedLODsInPackage(TArray<TWeakObjectPtr<UStaticMesh>> Objects);

	/** Handler for when RemoveVertexColors is selected */
	void ExecuteRemoveVertexColors(TArray<TWeakObjectPtr<UStaticMesh>> Objects);


	TWeakObjectPtr<UStaticMesh> LODCopyMesh;



};

