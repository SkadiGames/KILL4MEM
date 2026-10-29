// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KILL4MEM : ModuleRules
{
	public KILL4MEM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"KILL4MEM",
			"KILL4MEM/Variant_Horror",
			"KILL4MEM/Variant_Horror/UI",
			"KILL4MEM/Variant_Shooter",
			"KILL4MEM/Variant_Shooter/AI",
			"KILL4MEM/Variant_Shooter/UI",
			"KILL4MEM/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
