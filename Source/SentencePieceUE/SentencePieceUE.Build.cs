using UnrealBuildTool;
using System.IO;

public class SentencePieceUE : ModuleRules
{
    public SentencePieceUE(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Projects"
            }
        );

        // --- ThirdParty Integration ---
        
        // 1. Include Paths
        string ThirdPartyPath = Path.Combine(PluginDirectory, "Source", "ThirdParty");
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "sentencepiece_src", "src"));

        // 2. Link Libraries
        string LibPath = "";
        string LibFileName = "";

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            LibPath = Path.Combine(ThirdPartyPath, "Lib", "Win64");
            LibFileName = "sentencepiece.lib";
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            LibPath = Path.Combine(ThirdPartyPath, "Lib", "Mac");
            LibFileName = "libsentencepiece.a";
        }

        if (LibPath != "")
        {
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, LibFileName));
        }
    }
}