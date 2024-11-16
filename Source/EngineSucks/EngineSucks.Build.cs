// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EngineSucks : ModuleRules
{
	public EngineSucks(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		// Core Dependency Modules
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		// AI Dependency Modules
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "Navmesh", "NavigationSystem" });
		
        // User Interface Modules
        PublicDependencyModuleNames.AddRange(new string[] { "UMG" });
        
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
