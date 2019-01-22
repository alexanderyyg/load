// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FbxBuilderTextureImportData.h"

UFbxBuilderTextureImportData::UFbxBuilderTextureImportData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

bool UFbxBuilderTextureImportData::CanEditChange(const UProperty* InProperty) const
{
	bool bMutable = Super::CanEditChange(InProperty);
	UObject* Outer = GetOuter();
	if(Outer && bMutable)
	{
		// Let the parent object handle the editability of our properties
		bMutable = Outer->CanEditChange(InProperty);
	}
	return bMutable;
}
