#pragma once
#include "Engine/StaticMesh.h"
#include "Editor/StaticMeshEditor/Public/StaticMeshEditorModule.h"
#include "AssetTypeActions_Base.h"


class FAlexAssetActions :public FAssetTypeActions_Base
{
public:
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override; //ϵͳ���� ȷ�ϸ������Ƿ���action
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override; //������淵��true ��ô�������� ��Ӳ˵���
	virtual FText GetName() const override; //��Դ������ͼģʽ�� ����tip��ʾ
	virtual UClass* GetSupportedClass() const override; //��Դ����ͼ����ɫ
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
	virtual bool IsImportedAsset() const override { return true; }//����Ҫ����ϵ�Ƿ���reimport

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

