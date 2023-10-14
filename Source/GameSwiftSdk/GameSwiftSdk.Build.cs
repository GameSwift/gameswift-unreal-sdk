using System.IO;
using UnrealBuildTool;

public class GameSwiftSdk : ModuleRules
{
	public GameSwiftSdk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"HTTP",
			"Json",
			"JsonUtilities",
			"Engine"
		});
	}
}