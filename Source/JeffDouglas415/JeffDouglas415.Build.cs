// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JeffDouglas415 : ModuleRules
{
	public JeffDouglas415(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });
	}
}
