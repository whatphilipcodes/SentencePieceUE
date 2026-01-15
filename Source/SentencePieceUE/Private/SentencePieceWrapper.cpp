#include "SentencePieceWrapper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

// Include the library header inside the .cpp only
// This prevents macro collisions (like 'check') with Unreal
THIRD_PARTY_INCLUDES_START
#include "sentencepiece_processor.h"
THIRD_PARTY_INCLUDES_END

bool USentencePieceWrapper::LoadFromAsset(USentencePieceModel* ModelAsset)
{
    if (!ModelAsset || ModelAsset->ModelData.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid Model Asset"));
        return false;
    }

    // Clean up old processor
    if (ProcessorHandle)
    {
        delete static_cast<sentencepiece::SentencePieceProcessor*>(ProcessorHandle);
        ProcessorHandle = nullptr;
    }

    auto* Processor = new sentencepiece::SentencePieceProcessor();

    // SentencePiece supports loading from a "Serialized Proto" (which is what the .model file is)
    // We convert the TArray<uint8> to a std::string_view or std::string
    std::string Blob(
        reinterpret_cast<const char*>(ModelAsset->ModelData.GetData()),
        ModelAsset->ModelData.Num()
    );

    const auto Status = Processor->LoadFromSerializedProto(Blob);

    if (!Status.ok())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load model from asset: %s"), UTF8_TO_TCHAR(Status.ToString().c_str()));
        delete Processor;
        return false;
    }

    ProcessorHandle = Processor;

    // Optional: If you stored the JSON in the asset, load it here
    // if (!ModelAsset->TokenizerConfigJson.IsEmpty()) { ... parsing logic ... }

    return true;
}

TArray<int32> USentencePieceWrapper::EncodeAsIds(const FString &InputText)
{
    TArray<int32> Result;
    if (!ProcessorHandle)
        return Result;

    auto *Processor = static_cast<sentencepiece::SentencePieceProcessor *>(ProcessorHandle);
    std::string InputStd = TCHAR_TO_UTF8(*InputText);
    std::vector<int> OutputIds;

    Processor->Encode(InputStd, &OutputIds);

    Result.SetNum(OutputIds.size());
    for (size_t i = 0; i < OutputIds.size(); ++i)
    {
        Result[i] = OutputIds[i];
    }

    return Result;
}

TArray<FString> USentencePieceWrapper::EncodeAsPieces(const FString &InputText)
{
    TArray<FString> Result;
    if (!ProcessorHandle)
        return Result;

    auto *Processor = static_cast<sentencepiece::SentencePieceProcessor *>(ProcessorHandle);
    std::string InputStd = TCHAR_TO_UTF8(*InputText);
    std::vector<std::string> OutputPieces;

    Processor->Encode(InputStd, &OutputPieces);

    for (const auto &Piece : OutputPieces)
    {
        Result.Add(UTF8_TO_TCHAR(Piece.c_str()));
    }

    return Result;
}

FString USentencePieceWrapper::DecodeIds(const TArray<int32> &Ids)
{
    if (!ProcessorHandle || Ids.Num() == 0)
        return FString();

    auto *Processor = static_cast<sentencepiece::SentencePieceProcessor *>(ProcessorHandle);

    std::vector<int> InputIds;
    InputIds.reserve(Ids.Num());
    for (int32 Id : Ids)
    {
        InputIds.push_back(Id);
    }

    std::string OutputStd;
    Processor->Decode(InputIds, &OutputStd);

    return UTF8_TO_TCHAR(OutputStd.c_str());
}

void USentencePieceWrapper::BeginDestroy()
{
    Super::BeginDestroy();
    if (ProcessorHandle)
    {
        delete static_cast<sentencepiece::SentencePieceProcessor *>(ProcessorHandle);
        ProcessorHandle = nullptr;
    }
}