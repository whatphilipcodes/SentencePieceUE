#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SentencePieceModel.generated.h"

/**
 * Represents a binary .model file with specific tokenizer settings.
 */
UCLASS(BlueprintType)
class SENTENCEPIECEUE_API USentencePieceModel : public UObject
{
    GENERATED_BODY()

public:
    // The raw binary data of the .model file
    UPROPERTY()
    TArray<uint8> ModelData;

    // --- Configuration Options ---

    /** * If true, the input text will be lowercased before tokenization.
     * Matches HF config: "do_lower_case"
     */
    UPROPERTY(EditAnywhere, Category = "Config")
    bool bDoLowerCase = false;

    /** * If true, the Beginning of Sentence (BOS) token is prepended to the sequence.
     * Matches HF config: "add_bos_token"
     */
    UPROPERTY(EditAnywhere, Category = "Config")
    bool bAddBosToken = false;

    /** * If true, the End of Sentence (EOS) token is appended to the sequence.
     * Matches HF config: "add_eos_token"
     */
    UPROPERTY(EditAnywhere, Category = "Config")
    bool bAddEosToken = false;
};