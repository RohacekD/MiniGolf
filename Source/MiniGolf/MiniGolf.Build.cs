// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniGolf : ModuleRules
{
    public MiniGolf(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { "FMODStudio", "Slate", "SlateCore" });
	}
}
