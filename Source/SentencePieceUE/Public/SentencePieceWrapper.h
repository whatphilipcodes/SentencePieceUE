#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SentencePieceModel.h" // <--- Include your Asset definition
#include "SentencePieceWrapper.generated.h"

UCLASS(BlueprintType)
class SENTENCEPIECEUE_API USentencePieceWrapper : public UObject
{
	GENERATED_BODY()

public:
	// Load from the new Asset Type
	UFUNCTION(BlueprintCallable, Category = "SentencePiece")
	bool LoadFromAsset(USentencePieceModel* ModelAsset);

	UFUNCTION(BlueprintCallable, Category = "SentencePiece")
	TArray<int32> EncodeAsIds(const FString& InputText);

	UFUNCTION(BlueprintCallable, Category = "SentencePiece")
	FString DecodeIds(const TArray<int32>& Ids);

private:
	void* ProcessorHandle = nullptr;

	// Internal Configuration State
	bool bDoLowerCase = false;
	bool bAddBosToken = false;
	bool bAddEosToken = false;

	// Token IDs
	int32 BosId = -1;
	int32 EosId = -1;

	virtual void BeginDestroy() override;
};