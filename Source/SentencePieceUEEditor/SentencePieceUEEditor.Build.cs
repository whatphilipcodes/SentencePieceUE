using UnrealBuildTool;

public class SentencePieceUEEditor : ModuleRules
{
    public SentencePieceUEEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",       // <--- The Editor tools
                "SentencePieceUE" // <--- Link to your runtime module
            }
        );
    }
}