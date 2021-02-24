// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PuzzleProj : ModuleRules
{
	public PuzzleProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate","SlateCore"});
    }
}


//{
//  "FileVersion": 3,
//  "EngineAssociation": "4.22",
//  "Category": "",
//  "Description": "",
//  "Modules": [
//    {
//      "Name": "PuzzleProj",
//      "Type": "Runtime",
//      "LoadingPhase": "Default",
//      "AdditionalDependencies": [
//        "Engine",
//        "UMGEditor"
//      ]
//    }
//  ]
//}