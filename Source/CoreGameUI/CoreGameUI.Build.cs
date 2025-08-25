using UnrealBuildTool;

public class CoreGameUI : ModuleRules {
    public CoreGameUI(ReadOnlyTargetRules target) : base(target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
            ]);

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject", "Engine",
                "Slate", "SlateCore", "UMG",
            ]);
    }
}
