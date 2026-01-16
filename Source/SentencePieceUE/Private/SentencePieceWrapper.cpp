#include "SentencePieceWrapper.h"
#include "SentencePieceModel.h" // <--- CRITICAL: Defines USentencePieceModel
#include <string>               // <--- CRITICAL: Fixes 'string' is not a member of std

// --- Third Party Includes ---
THIRD_PARTY_INCLUDES_START
#include "sentencepiece_processor.h"
THIRD_PARTY_INCLUDES_END

bool USentencePieceWrapper::LoadFromAsset(USentencePieceModel* ModelAsset)
{
    // 1. Safety Checks
    if (!ModelAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("LoadFromAsset failed: Asset is null."));
        return false;
    }

    if (ModelAsset->ModelData.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("LoadFromAsset failed: Asset has no binary data."));
        return false;
    }

    // 2. Clean up old processor
    if (ProcessorHandle)
    {
        delete static_cast<sentencepiece::SentencePieceProcessor*>(ProcessorHandle);
        ProcessorHandle = nullptr;
    }

    // 3. Initialize Processor
    auto* Processor = new sentencepiece::SentencePieceProcessor();

    // Convert Unreal byte array to std::string for the library
    // We cast the raw data pointer to char*
    std::string Blob(
        reinterpret_cast<const char*>(ModelAsset->ModelData.GetData()),
        ModelAsset->ModelData.Num()
    );

    // Load from memory
    const auto Status = Processor->LoadFromSerializedProto(Blob);

    if (!Status.ok())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load model: %s"), UTF8_TO_TCHAR(Status.ToString().c_str()));
        delete Processor;
        return false;
    }

    ProcessorHandle = Processor;

    // 4. Apply Config from Asset
    bDoLowerCase = ModelAsset->bDoLowerCase;
    bAddBosToken = ModelAsset->bAddBosToken;
    bAddEosToken = ModelAsset->bAddEosToken;

    // 5. Cache IDs
    BosId = Processor->bos_id();
    EosId = Processor->eos_id();

    UE_LOG(LogTemp, Log, TEXT("SPM Loaded. LowerCase: %d, BOS: %d, EOS: %d"), bDoLowerCase, bAddBosToken, bAddEosToken);

    return true;
}

TArray<int32> USentencePieceWrapper::EncodeAsIds(const FString& InputText)
{
    TArray<int32> Result;
    if (!ProcessorHandle) return Result;

    // Apply Lowercase Config
    FString ProcessedText = bDoLowerCase ? InputText.ToLower() : InputText;

    auto* Processor = static_cast<sentencepiece::SentencePieceProcessor*>(ProcessorHandle);
    std::string InputStd = TCHAR_TO_UTF8(*ProcessedText);
    std::vector<int> OutputIds;

    Processor->Encode(InputStd, &OutputIds);

    // Apply BOS
    if (bAddBosToken && BosId != -1) Result.Add(BosId);

    // Body
    for (int Id : OutputIds) Result.Add(Id);

    // Apply EOS
    if (bAddEosToken && EosId != -1) Result.Add(EosId);

    return Result;
}

FString USentencePieceWrapper::DecodeIds(const TArray<int32>& Ids)
{
    if (!ProcessorHandle || Ids.Num() == 0) return FString();

    auto* Processor = static_cast<sentencepiece::SentencePieceProcessor*>(ProcessorHandle);

    std::vector<int> InputIds;
    for (int32 Id : Ids) InputIds.push_back(Id);

    std::string OutputStd;
    Processor->Decode(InputIds, &OutputStd);

    return UTF8_TO_TCHAR(OutputStd.c_str());
}

void USentencePieceWrapper::BeginDestroy()
{
    Super::BeginDestroy();
    if (ProcessorHandle)
    {
        delete static_cast<sentencepiece::SentencePieceProcessor*>(ProcessorHandle);
        ProcessorHandle = nullptr;
    }
}