// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class load : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string RootPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }
    public load(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"load/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"load/Private",
				// ... add other private include paths required here ...
			}
			);

        PublicAdditionalLibraries.Add(Path.Combine(RootPartyPath, "lib/libfbxsdk.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(RootPartyPath, "lib/libfbxsdk.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(RootPartyPath, "lib/libfbxsdk-md.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(RootPartyPath, "lib/libfbxsdk-mt.lib"));
        PublicIncludePaths.Add(Path.Combine(RootPartyPath, "include"));

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Engine",
                "ApplicationCore",
                "InputCore",
                "Core",
                "Slate",
                "SlateCore",
                "ContentBrowser",
                "Analytics",
                "Landscape",
                "SourceControl",
                "StaticMeshEditor",
                "EditorStyle",
                "CoreUObject",
                "UnrealEd",
                "PropertyEditor",
                "LevelEditor",
                "RenderCore",
                "MeshUtilities",
                "AssetTools",
                "TargetPlatform",
                "DesktopPlatform",
                "MainFrame",
                "RawMesh",
                "FBX",
                "MessageLog",
                "MeshUtilities",
                "Kismet"

				// ... add other public dependencies that you statically link with here ...
			}
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Engine",
                "InputCore",
                "Core",
                "ApplicationCore",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "CoreUObject",
                "UnrealEd",
                "StaticMeshEditor",
                "PropertyEditor",
                "ContentBrowser",
                "LevelEditor",
                "MeshUtilities",
                "AssetTools",
                "RenderCore",
                "TargetPlatform",
                "DesktopPlatform",
                "MainFrame",
                "ClassViewer",
                "RawMesh",
                "FBX",
                "MessageLog",
                "MeshUtilities",
                "Kismet"

				// ... add private dependencies that you statically link with here ...	
			}
            );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                 "WorkspaceMenuStructure",

				// ... add any modules that your module loads dynamically here ...
			}
            );
	}
}
