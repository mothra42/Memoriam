// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SeattleSlowJam2022 : ModuleRules
{
	public SeattleSlowJam2022(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
