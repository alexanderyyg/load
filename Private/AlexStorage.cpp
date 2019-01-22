#include "AlexStorage.h"




FString UAlexStorage::FillStorage(const FString& StartPath)
{
	FString returnName="";

	TArray<UObject*> ReturnObjects;
	FString FileTypes, AllExtensions;
	TArray<UFactory*> Factories;

	// Get the list of valid factories
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* CurrentClass = (*It);

		if (CurrentClass->IsChildOf(UFactory::StaticClass()) && !(CurrentClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UFactory* Factory = Cast<UFactory>(CurrentClass->GetDefaultObject());
			if (Factory->bEditorImport)
			{
				Factories.Add(Factory);
			}
		}
	}

	TMultiMap<uint32, UFactory*> FilterIndexToFactory;

	// Generate the file types and extensions represented by the selected factories
	ObjectTools::GenerateFactoryFileExtensions(Factories, FileTypes, AllExtensions, FilterIndexToFactory);

	//	FileTypes = FString::Printf(TEXT("All Files (%s)|%s|%s"), *AllExtensions, *AllExtensions, *FileTypes);
	FileTypes = TEXT("TGA (*.tga *.png *.jpg)|*.tga;*.png;*.jpg");
	
	bool bOpened;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bOpened = false;
	int32 FilterIndex = -1;

	if (DesktopPlatform)
	{
		void* ParentWindowWindowHandle = NULL;//


		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
		const TSharedPtr<SWindow>& MainFrameParentWindow = MainFrameModule.GetParentWindow();
		if (MainFrameParentWindow.IsValid() && MainFrameParentWindow->GetNativeWindow().IsValid())
		{
			ParentWindowWindowHandle = MainFrameParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}

		bOpened = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowWindowHandle,TEXT("GetFile"), 
			FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT), 
			OpenFolder) ;


	}
	if (bOpened)
	{
		if (FPaths::DirectoryExists(OpenFolder))
		{
			returnName = OpenFolder;
		}
	}

	return returnName;

}


void UAlexStorage::LoadPic(const FString InputPath, const FString ExportPath, TArray<UAlexMaterial> *Mats, TArray<FString> *Import)
{
	bool bOpened;
	if (FPaths::DirectoryExists(InputPath))
		bOpened = true;
	else
	{
		bOpened = false;
		return;
	}
	OpenFolder = InputPath;
	FString ExportFolder = ExportPath;
	TArray<FString> *ImportedNames;
	ImportedNames = Import;
	TArray<FString> ImportedFileNames;	
	if (bOpened)
	{
		UE_LOG(LogTemp, Warning, TEXT("Folder %s"), *OpenFolder);
		//��Դ�б�Ŀ¼
		//����Դ�б��е�·��ȫ��
		FString ObjectPath;
		ImportedFileNames.Empty();

		FString BasePackageName;
		FString TextureName;
		TArray<FString> FbxNames;
		if (FPaths::DirectoryExists(OpenFolder))
		{
			UTexture * UnrealTexture = NULL;
			UTexture* ExistingTexture = NULL;
			UPackage* TexturePackage = NULL;
			FString Path = OpenFolder + "/*.tga";
			FString FBXPath = OpenFolder + "/*.fbx";
			FFileManagerGeneric::Get().FindFiles(FbxNames, *FBXPath, true, false);

			for (int i = 0; i < FbxNames.Num(); i++)
			{
				FbxNames[i] = OpenFolder + "/" + FbxNames[i];

				if (!ImportedNames->Contains(FbxNames[i]))
					ImportedNames->Add(FbxNames[i]);
			}



			FFileManagerGeneric::Get().FindFiles(ImportedFileNames, *Path, true, false);
			for (int i = 0; i < ImportedFileNames.Num(); i++)
			{

				UE_LOG(LogTemp, Warning, TEXT("  File:%s"), *ImportedFileNames[i]);
				ImportedFileNames[i] = OpenFolder + "/" + ImportedFileNames[i];
				UE_LOG(LogTemp, Warning, TEXT("  File:%s"), *ImportedFileNames[i]);
				//��ȡ��׺��
				FString Extension = FPaths::GetExtension(ImportedFileNames[i]).ToLower();
				// name the texture with file name
				FString TextureName = FPaths::GetBaseFilename(ImportedFileNames[i]);
				BasePackageName = ExportFolder+ PicAddPath + TextureName;
				BasePackageName = PackageTools::SanitizePackageName(BasePackageName);
				//����޸ĳɿ��Ե�����ļ���
				TextureName = ObjectTools::SanitizeObjectName(TextureName);

				//����Դ�б��е�·��ȫ������������ ��Դ·��ȫ��.����
				ObjectPath = (BasePackageName + TEXT(".") + TextureName);

				//��ȡͼƬ��Դ
				ExistingTexture = LoadObject<UTexture>(NULL, *ObjectPath, nullptr, LOAD_Quiet | LOAD_NoWarn);
				if (!ExistingTexture)
				{
					//�����ھ��½�
					const FString Suffix(TEXT("")); //��׺
													//��ȡ��Դ���ع���
					FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
					FString FinalPackageName;

					AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, Suffix, FinalPackageName, TextureName);
					//����һ����Դ·��
					TexturePackage = CreatePackage(NULL, *FinalPackageName);
				}
				else
				{
					//������ھ���ԭ������Դλ��
					TexturePackage = ExistingTexture->GetOutermost();
				}
				TArray<uint8> DataBinary;
				//��ȡͼƬ������
				if (!ImportedFileNames[i].IsEmpty())
				{
					FFileHelper::LoadFileToArray(DataBinary, *ImportedFileNames[i]);
				}
				if (DataBinary.Num() > 0)
				{
					//ͼƬ�������׵�ַ
					const uint8* PtrTexture = DataBinary.GetData();
					auto TextureFact = NewObject<UTextureFactory>();
					TextureFact->AddToRoot();

					//֧�ָ�����ʾ
					TextureFact->SuppressImportOverwriteDialog();
					//��׺Ϊ����
					const TCHAR* TextureType = *Extension;

					//���빤����
					UnrealTexture = (UTexture*)TextureFact->FactoryCreateBinary(
						UTexture2D::StaticClass(), TexturePackage, *TextureName,
						RF_Standalone | RF_Public, NULL, TextureType,
						PtrTexture, PtrTexture + DataBinary.Num(), GWarn);
					if (UnrealTexture != NULL)
					{
						//��������
						//Make sure the AssetImportData point on the texture file and not on the fbx files since the factory point on the fbx file
						UnrealTexture->AssetImportData->Update(IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*(ImportedFileNames[i])));
						//����ע��
						// Notify the asset registry  
						FAssetRegistryModule::AssetCreated(UnrealTexture);

						// Set the dirty flag so this package will get saved later
						TexturePackage->SetDirtyFlag(true);
					}
					TextureFact->RemoveFromRoot();
					UE_LOG(LogTemp, Warning, TEXT("     Texture:%s"), *TextureName);

					AddPicToUAlexMaterial(TextureName, ExportFolder, Mats);
				}

			}
		}

	}

}


void UAlexStorage::AddPicToUAlexMaterial(FString Filename, const FString ExportPath, TArray<UAlexMaterial> *Mats)
{

	FString ExprotFolder = ExportPath;
	FString Path = ExprotFolder+ PicAddPath;
	int FindIndex = -1;
	int32 position2=0;

	UMaterialInstanceConstant* UnrealMaterialInstance;
	for (int32 MatIndex = 0; MatIndex <=Mats->Num() - 1 ; ++MatIndex)
	{
		
		Mat = (*Mats)[MatIndex];
		UE_LOG(LogTemp, Warning, TEXT("     Mat:%s"), *(Mat.Name));
		if (Filename.StartsWith(Mat.Name, ESearchCase::IgnoreCase))
		{
			FString baseName = Mat.Name;
			FString PackageName = ExprotFolder+ MatAddPath;
			FString MaterialIns = "MaterialInstanceConstant'" + PackageName + baseName + "." + baseName + "'";



			UE_LOG(LogTemp, Warning, TEXT("     InMat:%s"), *(Mat.Name));

			int32 leng1 = Mat.Name.Len();
			if (Filename[leng1] == '_')
			//������_��-
		//	if (Filename[leng1] == '-')
			{
				FindIndex = MatIndex;



				if (Filename.EndsWith("BaseColor", ESearchCase::IgnoreCase))
				{

					position2 = Filename.Find("BaseColor", ESearchCase::IgnoreCase, ESearchDir::FromEnd);

					if (Filename[position2 - 1] == '_')
					//������_��-
				//	if (Filename[position2 - 1] == '-')
					{

						UnrealMaterialInstance = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), UMaterialInstanceConstant::StaticClass(),
							*MaterialIns));
						if (UnrealMaterialInstance == nullptr)
						{
							UE_LOG(LogTemp, Warning, TEXT("NoInstance"));
							return;
						}

						Mat.BaseColorPath = Path + Filename ;

						FStringAssetReference DiffuseAssetPath(Mat.BaseColorPath);
						UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
						UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("BaseColorTexture"), A);

					}

				}

				if (Filename.EndsWith("Normal", ESearchCase::IgnoreCase))
				{

					position2 = Filename.Find("Normal", ESearchCase::IgnoreCase, ESearchDir::FromEnd);

					if (Filename[position2 - 1] == '_')
					//������_��-
				//	if (Filename[position2 - 1] == '-')
					{
						UnrealMaterialInstance = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), UMaterialInstanceConstant::StaticClass(),
							*MaterialIns));
						if (UnrealMaterialInstance == nullptr)
						{
							UE_LOG(LogTemp, Warning, TEXT("NoInstance"));
							return;
						}

						Mat.NormalPath = Path + Filename ;
						FStringAssetReference DiffuseAssetPath(Mat.NormalPath);
						UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
						UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("NormalTexture"), A);


					}

				}

				if (Filename.EndsWith("OcclusionRoughnessMetallic", ESearchCase::IgnoreCase))
				{

					position2 = Filename.Find("OcclusionRoughnessMetallic", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
					if (Filename[position2 - 1] == '_')
					//������_��-
				//	if (Filename[position2 - 1] == '-')
					{
						UnrealMaterialInstance = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), UMaterialInstanceConstant::StaticClass(),
							*MaterialIns));
						if (UnrealMaterialInstance == nullptr)
						{
							UE_LOG(LogTemp, Warning, TEXT("NoInstance"));
							return;
						}
						Mat.OcclusionRoughnessMetallicPath = Path + Filename ;

						FStringAssetReference DiffuseAssetPath(Mat.OcclusionRoughnessMetallicPath);
						UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
						UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("OcclusionRoughnessMetallicTexture"), A);
					}

				}



			}

		}

	}
	if (FindIndex == -1)
	{
		Mat =UAlexMaterial();
	//	TArray<FString> LevelArr;
	//	Filename.ParseIntoArray(LevelArr, TEXT("_"), true);

		//û�о���ѭ��position2����Ϊ-1
		if(Filename.EndsWith("BaseColor", ESearchCase::IgnoreCase))
			position2 = Filename.Find("BaseColor", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (Filename.EndsWith("Normal", ESearchCase::IgnoreCase))
			position2 = Filename.Find("Normal", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (Filename.EndsWith("OcclusionRoughnessMetallic", ESearchCase::IgnoreCase))
			position2 = Filename.Find("OcclusionRoughnessMetallic", ESearchCase::IgnoreCase, ESearchDir::FromEnd);


		Mat.Name=Filename.Left(position2 - 1);
	//	Mat.Name = LevelArr[0]+'_'+ LevelArr[1];

		UE_LOG(LogTemp, Warning, TEXT("\n    CreateNameInput:%s"), *Mat.Name);
		UnrealMaterialInstance =CreateMaterialInstance(Mat.Name, ExprotFolder);
		if (UnrealMaterialInstance == nullptr)
			return;
		Mat.MaterialPath = ExprotFolder + MatAddPath + Mat.Name;
		if (Filename.EndsWith("BaseColor", ESearchCase::IgnoreCase))
		{

			position2 = Filename.Find("BaseColor", ESearchCase::IgnoreCase, ESearchDir::FromEnd);


			if (Filename[position2 - 1] == '_')
			//������_��-
		//	if (Filename[position2 - 1] == '-')
			{
				Mat.BaseColorPath = Path + Filename ;

				FStringAssetReference DiffuseAssetPath(Mat.BaseColorPath);
				UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
				UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("BaseColorTexture"), A);
				
			}

		}

		if (Filename.EndsWith("Normal", ESearchCase::IgnoreCase))
		{

			position2 = Filename.Find("Normal", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (Filename[position2 - 1] == '_')
				//������_��-
		//	if (Filename[position2 - 1] == '-')
			{
				Mat.NormalPath = Path + Filename ;
				FStringAssetReference DiffuseAssetPath(Mat.NormalPath);
				UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
				UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("NormalTexture"), A);


			}

		}

		if (Filename.EndsWith("OcclusionRoughnessMetallic", ESearchCase::IgnoreCase))
		{

			position2 = Filename.Find("OcclusionRoughnessMetallic", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (Filename[position2 - 1] == '_')
				//������_��-
		//	if (Filename[position2 - 1] == '-')
			{
				Mat.OcclusionRoughnessMetallicPath = Path + Filename ;
				
				FStringAssetReference DiffuseAssetPath(Mat.OcclusionRoughnessMetallicPath);
				UTexture* A = Cast<UTexture>(DiffuseAssetPath.TryLoad());
				UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("OcclusionRoughnessMetallicTexture"), A);
			}

		}
		Mats->Add(Mat);
//		UE_LOG(LogTemp, Warning, TEXT("\n    MaterialNum:%d"), Mats->Num);
	}



}


UMaterialInstanceConstant* UAlexStorage::CreateMaterialInstance(FString MaterialName, FString ExportPath)
{
	FString ExportFolder = ExportPath;
	FString baseName = "MetaM1";
	FString PackageName1 = ExportFolder+ MatAddPath;
	FString PackageName = "/load"+ MatAddPath +"base/";
	FString MaterialParent ="Material'"+ PackageName + baseName +"."+ baseName + "'";
	UMaterial * UnrealMaterial= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), UMaterial::StaticClass(), 
		*MaterialParent));
	if (UnrealMaterial == nullptr)
	{
		UnrealMaterial=MakeBaseMaterial();
	}
	UE_LOG(LogTemp, Warning, TEXT("\n    CreateName:%s"), *MaterialName);

	FString Name = MaterialName;
	PackageName1 += Name;
	UPackage* Package1 = CreatePackage(NULL, *PackageName1);
	UE_LOG(LogTemp, Warning, TEXT("\n    CreateName:%s"), *PackageName1);
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
	FLinearColor CurrentColor;
	UTexture * A;
	float m,p;

	//���ͼƬ����

	UnrealMaterialInstance->GetTextureParameterValue(FName("BaseColorTexture"), A);
	UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("BaseColorTexture"), A);


	UnrealMaterialInstance->GetTextureParameterValue(FName("NormalTexture"), A);
	UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("NormalTexture"), A);


	UnrealMaterialInstance->GetTextureParameterValue(FName("OcclusionRoughnessMetallicTexture"), A);
	UnrealMaterialInstance->SetTextureParameterValueEditorOnly(FName("OcclusionRoughnessMetallicTexture"), A);

	//�����������
	UnrealMaterialInstance->GetVectorParameterValue(FName("LightColor"), CurrentColor);
	CurrentColor = FLinearColor(1, 1, 1, 1);
	UnrealMaterialInstance->SetVectorParameterValueEditorOnly(FName("LightColor"), CurrentColor);

	//�����ֵ����
	p = 0;
	m = 1;
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("Power"), p);
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("MetallicScale"), m);
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("RoughnessScale"), m);
	UnrealMaterialInstance->SetScalarParameterValueEditorOnly(FName("OcclusionScale"), m);

	//	UMaterialExpressionVectorParameter
	return (UnrealMaterialInstance);
}


UMaterial* UAlexStorage::MakeBaseMaterial()
{
	UE_LOG(LogTemp, Warning, TEXT("MaterialBuilder11 BUTTON CLICKED!!!!"));
	//	TArray<UObject*> ImportedObjects = FbxBuilder->ImportAssets(TEXT("/Game/Assets/Mesh"));
	//	FString PathToLoad = "/Game/Assets"+PicAddPath+"cheti";
	//	tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	//	Material = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(),
	//		UMaterialInstance::StaticClass(), TEXT("MaterialInstanceConstant'/Game/Assets/Material/M_1_Inst'")));
	//		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	//	DynamicMaterial->SetTextureParameterValue(FName("Diffuse"), tmpTexture);

	FString MaterialBaseName = "MetaM1";
	FString PackageName = "/load"+ MatAddPath +"base/";
	PackageName += MaterialBaseName;
	UPackage* Package = CreatePackage(NULL, *PackageName);

	// create an unreal material asset �������������Դ
	UMaterialFactoryNew * MaterialFactory = NewObject<UMaterialFactoryNew>();
	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(MaterialFactory->ResolveSupportedClass(), Package,
		*MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);
	//	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(MaterialFactory->ResolveSupportedClass(), Package, *MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);
	//  MaterialFactory->ResolveSupportedClass() �ɸĳ�UMaterial::StaticClass()
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




	//��1�ű���ͼ
	UMaterialExpressionTextureSampleParameter2D* TextureBaseColor = NewObject<UMaterialExpressionTextureSampleParameter2D>(UnrealMaterial);
	TextureBaseColor->SamplerType = SAMPLERTYPE_Color;
	//	TextureExpression1->ParameterName= TEXT("RoughPic");
	TextureBaseColor->SetEditableName("BaseColorTexture");
	TextureBaseColor->MaterialExpressionEditorX = -700;
	TextureBaseColor->MaterialExpressionEditorY = -500;
	UnrealMaterial->Expressions.Add(TextureBaseColor);//���ӵĻ�ʵ������û��



	UMaterialExpressionVectorParameter * VP3;//��ά����
	VP3 = NewObject<UMaterialExpressionVectorParameter>(UnrealMaterial);
	VP3->SetEditableName("LightColor");
	//	VP3->ParameterName = TEXT("RoughValue");				
	VP3->DefaultValue.R = 1.0f;// 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
	VP3->DefaultValue.G = 1.0f;//0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
	VP3->DefaultValue.B = 1.0f;//0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
	VP3->DefaultValue.A = 1.0f;
	VP3->MaterialExpressionEditorX = -700;
	VP3->MaterialExpressionEditorY = -300;
	UnrealMaterial->Expressions.Add(VP3);


	//��1�˷����ʽ
	UMaterialExpressionMultiply* Multiply1 = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
	UnrealMaterial->Expressions.Add(Multiply1);
	Multiply1->MaterialExpressionEditorX = -250;
	Multiply1->MaterialExpressionEditorY = -400;
	UnrealMaterial->Expressions.Add(Multiply1);



	//��ϵ	
	Multiply1->B.Expression = VP3;
	Multiply1->A.Expression = TextureBaseColor;
	//TextureExpression1->Coordinates.Expression = Multiply1; //UVs����
	UnrealMaterial->BaseColor.Expression = Multiply1;

	TArray<FExpressionOutput> Outputs1 = UnrealMaterial->BaseColor.Expression->GetOutputs();
	FExpressionOutput* Output1 = Outputs1.GetData();
	UnrealMaterial->BaseColor.Mask = Output1->Mask;
	UnrealMaterial->BaseColor.MaskR = Output1->MaskR;
	UnrealMaterial->BaseColor.MaskG = Output1->MaskG;
	UnrealMaterial->BaseColor.MaskB = Output1->MaskB;
	UnrealMaterial->BaseColor.MaskA = Output1->MaskA;






		// make texture sampler ����ͼƬ����������2�ű���ͼ��
	UMaterialExpressionTextureSampleParameter2D* TextureNormal = NewObject<UMaterialExpressionTextureSampleParameter2D>(UnrealMaterial);
	TextureNormal->SamplerType = SAMPLERTYPE_Color;
	TextureNormal->SetEditableName("NormalTexture");
	TextureNormal->MaterialExpressionEditorX = -700;
	TextureNormal->MaterialExpressionEditorY = 500;
	UnrealMaterial->Expressions.Add(TextureNormal);//���ӵĻ�ʵ������û��


		//һά����
		UMaterialExpressionScalarParameter* Parameter1 = NewObject<UMaterialExpressionScalarParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Parameter1);
		Parameter1->SetEditableName("Power");
		Parameter1->DefaultValue = 0.0f;
		Parameter1->MaterialExpressionEditorX = -700;
		Parameter1->MaterialExpressionEditorY = 700;
		UnrealMaterial->Expressions.Add(Parameter1);


		// Tiling system ��ڵ�ϵͳ�˷� ���ʽ
		UMaterialExpressionPower* PowerEx = NewObject<UMaterialExpressionPower>(UnrealMaterial);
	//	UMaterialExpressionMultiply* Multiply = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
		PowerEx->MaterialExpressionEditorX = -250;
		PowerEx->MaterialExpressionEditorY = 700;
		UnrealMaterial->Expressions.Add(PowerEx);
		//FExpressionInput InputExpression;
		//InputExpression.Expression = Multiply;		

		//��ϵ
		PowerEx->Base.Expression =  TextureNormal;
		PowerEx->Exponent.Expression =Parameter1;
	//	PowerEx->A.Connect(2, TextureExpression);//input����output�����кţ����кŴ�0��ʼ��
		UnrealMaterial->Normal.Expression = PowerEx;




		TArray<FExpressionOutput> Outputs = UnrealMaterial->Normal.Expression->GetOutputs();
		FExpressionOutput* Output = Outputs.GetData();
		UnrealMaterial->Normal.Mask = Output->Mask;
		UnrealMaterial->Normal.MaskR = Output->MaskR;
		UnrealMaterial->Normal.MaskG = Output->MaskG;
		UnrealMaterial->Normal.MaskB = Output->MaskB;
		UnrealMaterial->Normal.MaskA = Output->MaskA;




		//��3�ű���ͼ
		UMaterialExpressionTextureSampleParameter2D* TextureOcclusionRoughnessMetallic = NewObject<UMaterialExpressionTextureSampleParameter2D>(UnrealMaterial);
		TextureOcclusionRoughnessMetallic->SamplerType = SAMPLERTYPE_Color;
		//	TextureExpression1->ParameterName= TEXT("RoughPic");
		TextureOcclusionRoughnessMetallic->SetEditableName("OcclusionRoughnessMetallicTexture");
		TextureOcclusionRoughnessMetallic->MaterialExpressionEditorX = -700;
		TextureOcclusionRoughnessMetallic->MaterialExpressionEditorY = 100;
		UnrealMaterial->Expressions.Add(TextureOcclusionRoughnessMetallic);//���ӵĻ�ʵ������û��



		//һά��������B��ӦM
		UMaterialExpressionScalarParameter* Parameter2 = NewObject<UMaterialExpressionScalarParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Parameter2);
		Parameter2->SetEditableName("MetallicScale");
		Parameter2->DefaultValue =1.0f;
		Parameter2->MaterialExpressionEditorX = -500;
		Parameter2->MaterialExpressionEditorY = -200;
		UnrealMaterial->Expressions.Add(Parameter2);

		//��3�˷����ʽ
		UMaterialExpressionMultiply* Multiply2 = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Multiply2);
		Multiply2->MaterialExpressionEditorX = -250;
		Multiply2->MaterialExpressionEditorY = -50;
		UnrealMaterial->Expressions.Add(Multiply2);



		//��ϵ	
		Multiply2->A.Expression = Parameter2;
		Multiply2->B.Connect(3, TextureOcclusionRoughnessMetallic);
		UnrealMaterial->Metallic.Expression = Multiply2;

		TArray<FExpressionOutput> Outputs2 = UnrealMaterial->Metallic.Expression->GetOutputs();
		FExpressionOutput* Output2 = Outputs2.GetData();
		UnrealMaterial->Metallic.Mask = Output2->Mask;
		UnrealMaterial->Metallic.MaskR = Output2->MaskR;
		UnrealMaterial->Metallic.MaskG = Output2->MaskG;
		UnrealMaterial->Metallic.MaskB = Output2->MaskB;
		UnrealMaterial->Metallic.MaskA = Output2->MaskA;






		//һά��������G��ӦR
		UMaterialExpressionScalarParameter* Parameter3 = NewObject<UMaterialExpressionScalarParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Parameter3);
		Parameter3->SetEditableName("RoughnessScale");
		Parameter3->DefaultValue = 1.0f;
		Parameter3->MaterialExpressionEditorX = -350;
		Parameter3->MaterialExpressionEditorY = 200;
		UnrealMaterial->Expressions.Add(Parameter3);

		//��1�˷����ʽ
		UMaterialExpressionMultiply* Multiply3 = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Multiply3);
		Multiply3->MaterialExpressionEditorX = -250;
		Multiply3->MaterialExpressionEditorY = 350;
		UnrealMaterial->Expressions.Add(Multiply3);



		//��ϵ	
		Multiply3->A.Expression = Parameter3;
		Multiply3->B.Connect(2, TextureOcclusionRoughnessMetallic);
		UnrealMaterial->Roughness.Expression = Multiply3;

		TArray<FExpressionOutput> Outputs3 = UnrealMaterial->Roughness.Expression->GetOutputs();
		FExpressionOutput* Output3 = Outputs3.GetData();
		UnrealMaterial->Roughness.Mask = Output3->Mask;
		UnrealMaterial->Roughness.MaskR = Output3->MaskR;
		UnrealMaterial->Roughness.MaskG = Output3->MaskG;
		UnrealMaterial->Roughness.MaskB = Output3->MaskB;
		UnrealMaterial->Roughness.MaskA = Output3->MaskA;








		//һά��������R��ӦO
		UMaterialExpressionScalarParameter* Parameter4 = NewObject<UMaterialExpressionScalarParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Parameter4);
		Parameter4->SetEditableName("OcclusionScale");
		Parameter4->DefaultValue = 1.0f;
		Parameter4->MaterialExpressionEditorX = -500;
		Parameter4->MaterialExpressionEditorY = 400;
		UnrealMaterial->Expressions.Add(Parameter4);

		//��1�˷����ʽ
		UMaterialExpressionMultiply* Multiply4 = NewObject<UMaterialExpressionMultiply>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(Multiply4);
		Multiply4->MaterialExpressionEditorX = -250;
		Multiply4->MaterialExpressionEditorY = 550;
		UnrealMaterial->Expressions.Add(Multiply4);



		//��ϵ	
		Multiply4->A.Expression = Parameter4;
		Multiply4->B.Connect(1, TextureOcclusionRoughnessMetallic);
		UnrealMaterial->AmbientOcclusion.Expression = Multiply4;

		TArray<FExpressionOutput> Outputs4 = UnrealMaterial->AmbientOcclusion.Expression->GetOutputs();
		FExpressionOutput* Output4 = Outputs4.GetData();
		UnrealMaterial->AmbientOcclusion.Mask = Output4->Mask;
		UnrealMaterial->AmbientOcclusion.MaskR = Output4->MaskR;
		UnrealMaterial->AmbientOcclusion.MaskG = Output4->MaskG;
		UnrealMaterial->AmbientOcclusion.MaskB = Output4->MaskB;
		UnrealMaterial->AmbientOcclusion.MaskA = Output4->MaskA;






		//	UMaterialExpression * Expression3D = CreateNewMaterialExpression(UMaterialExpressionFunctionOutput::StaticClass(), FVector3D(1, 2, 3), false, true);//һά����
		//	UnrealMaterial->Expressions.Add(Expression3D);

		//UnrealMaterial->Specular.Expression = ZeroExpression;
		//	Multiply1->B.Expression = Expression3D;


		// Tiling ����ͼƬ�ڵ������ӳ˷����ʽ
		//		TextureExpression->Coordinates.Expression = Multiply;
	



	// let the material update itself if necessary
	UnrealMaterial->PreEditChange(NULL);
	UnrealMaterial->PostEditChange();

	// make sure that any static meshes, etc using this material will stop using the FMaterialResource of the original 
	// material, and will use the new FMaterialResource created when we make a new UMaterial in place
	FGlobalComponentReregisterContext RecreateComponents;
	return (UnrealMaterial);
	//	UMaterialInstance *terrainMaterialInstance = UMaterialInstance::Parent=Create(UnrealMaterial, NULL);
}
