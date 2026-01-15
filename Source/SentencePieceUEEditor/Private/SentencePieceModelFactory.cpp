#include "SentencePieceModelFactory.h"
#include "SentencePieceModel.h" // Includes from the Runtime module

USentencePieceModelFactory::USentencePieceModelFactory()
{
    // Tell Unreal this factory creates USentencePieceModel objects
    SupportedClass = USentencePieceModel::StaticClass();
    bCreateNew = false;
    bEditorImport = true;

    // Associate with the .model extension
    Formats.Add(TEXT("model;SentencePiece Model"));
}

UObject* USentencePieceModelFactory::FactoryCreateBinary(
    UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
    UObject* Context, const TCHAR* Type, const uint8*& Buffer,
    const uint8* BufferEnd, FFeedbackContext* Warn)
{
    // 1. Create the new Asset
    USentencePieceModel* NewAsset = NewObject<USentencePieceModel>(InParent, InClass, InName, Flags);

    // 2. Copy the file binary data into the asset's array
    int64 Bytes = BufferEnd - Buffer;
    NewAsset->ModelData.SetNumUninitialized(Bytes);
    FMemory::Memcpy(NewAsset->ModelData.GetData(), Buffer, Bytes);

    // 3. (Optional) Auto-detect config json if it sits next to the file
    // ERROR WAS HERE: We do not redeclare CurrentFilename. We use the one from UFactory.
    FString ConfigPath = FPaths::ChangeExtension(CurrentFilename, TEXT("")) + TEXT("_config.json");

    // (Optional logic to load config could go here)

    return NewAsset;
}