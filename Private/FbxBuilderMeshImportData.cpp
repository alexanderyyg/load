// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FbxBuilderMeshImportData.h"
#include "UnrealEd.h"
#include "UObject/UnrealType.h"

UFbxBuilderMeshImportData::UFbxBuilderMeshImportData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NormalImportMethod = FBXBuilderNIM_ComputeNormals;
	NormalGenerationMethod = EFBXBuilderNormalGenerationMethod::MikkTSpace;
	bBakePivotInVertex = false;
}

bool UFbxBuilderMeshImportData::CanEditChange(const UProperty* InProperty) const
{
	bool bMutable = Super::CanEditChange(InProperty);
	UObject* Outer = GetOuter();
	if(Outer && bMutable)
	{
		// Let the parent object handle the editability of our properties
		bMutable = Outer->CanEditChange(InProperty);
	}

	static FName NAME_NormalGenerationMethod("NormalGenerationMethod");
	if( bMutable && InProperty->GetFName() == NAME_NormalGenerationMethod )
	{
		// Normal generation method is ignored if we are importing both tangents and normals
		return NormalImportMethod == FBXBuilderNIM_ImportNormalsAndTangents ? false : true;
	}


	return bMutable;
}
