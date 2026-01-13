#include "SentencePieceWrapper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

// Include the library header inside the .cpp only
// This prevents macro collisions (like 'check') with Unreal
THIRD_PARTY_INCLUDES_START
#include "sentencepiece_processor.h"
THIRD_PARTY_INCLUDES_END

bool USentencePieceWrapper::LoadModel(const FString &ModelPath)
{
    // Clean up existing processor if any
    if (ProcessorHandle)
    {
        delete static_cast<sentencepiece::SentencePieceProcessor *>(ProcessorHandle);
        ProcessorHandle = nullptr;
    }

    FString AbsPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*ModelPath);

    if (!FPaths::FileExists(AbsPath))
    {
        UE_LOG(LogTemp, Error, TEXT("SentencePiece Model not found at: %s"), *AbsPath);
        return false;
    }

    auto *Processor = new sentencepiece::SentencePieceProcessor();

    // Convert FString to std::string for the library
    std::string PathStd = TCHAR_TO_UTF8(*AbsPath);

    const auto Status = Processor->Load(PathStd);

    if (!Status.ok())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load SentencePiece model: %s"), UTF8_TO_TCHAR(Status.ToString().c_str()));
        delete Processor;
        return false;
    }

    ProcessorHandle = Processor;
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