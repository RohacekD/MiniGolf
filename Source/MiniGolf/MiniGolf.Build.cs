// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniGolf : ModuleRules
{
    public MiniGolf(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { "FMODStudio", "Slate", "SlateCore" });

        // https://forums.unrealengine.com/t/is-c-really-this-slow-in-unreal-4/83905/7#post918034
        // tanis - start faster compile time for small projects
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
        // tanis - end
    }
}
