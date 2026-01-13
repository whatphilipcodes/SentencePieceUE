#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SentencePieceWrapper.generated.h"

/**
 * A wrapper around the Google SentencePiece processor.
 */
UCLASS(BlueprintType)
class SENTENCEPIECEUE_API USentencePieceWrapper : public UObject
{
    GENERATED_BODY()

public:
    // Load the model from a file path
    UFUNCTION(BlueprintCallable, Category = "SentencePiece")
    bool LoadModel(const FString &ModelPath);

    // Tokenize text into IDs
    UFUNCTION(BlueprintCallable, Category = "SentencePiece")
    TArray<int32> EncodeAsIds(const FString &InputText);

    // Tokenize text into String pieces
    UFUNCTION(BlueprintCallable, Category = "SentencePiece")
    TArray<FString> EncodeAsPieces(const FString &InputText);

    // Decode IDs back to string
    UFUNCTION(BlueprintCallable, Category = "SentencePiece")
    FString DecodeIds(const TArray<int32> &Ids);

private:
    // We use void* to hide the implementation details (PIMPL idiom)
    // so raw SP headers don't leak into the rest of your project.
    void *ProcessorHandle = nullptr;

    virtual void BeginDestroy() override;
};