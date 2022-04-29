using UnrealBuildTool;

public class AdventureGame : ModuleRules
{
	public AdventureGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"UMG",
				"HeadMountedDisplay", 
				"GameplayTags",
				"EnhancedInput"
			}
		);
	}
}
