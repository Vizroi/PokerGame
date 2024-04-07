// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PokerLib : ModuleRules
{
	public PokerLib(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
	}
}
