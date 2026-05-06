using UnrealBuildTool;

public class CoreGameUI : ModuleRules {
    public CoreGameUI(ReadOnlyTargetRules target) : base(target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "BlueprintUtilities",
            ]);

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject", "Engine",
                "InputCore",
                "Slate", "SlateCore", "UMG",
            ]);
    }
}
