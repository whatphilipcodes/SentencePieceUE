#include "SentencePieceModelFactory.h"
#include "SentencePieceModel.h" // <--- CRITICAL: You must include this!
#include "Misc/Paths.h"

USentencePieceModelFactory::USentencePieceModelFactory()
{
    SupportedClass = USentencePieceModel::StaticClass();
    bCreateNew = false;
    bEditorImport = true;
    Formats.Add(TEXT("model;SentencePiece Model"));
}

UObject* USentencePieceModelFactory::FactoryCreateBinary(
    UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
    UObject* Context, const TCHAR* Type, const uint8*& Buffer,
    const uint8* BufferEnd, FFeedbackContext* Warn)
{
    USentencePieceModel* NewAsset = NewObject<USentencePieceModel>(InParent, InClass, InName, Flags);

    int64 Bytes = BufferEnd - Buffer;
    NewAsset->ModelData.SetNumUninitialized(Bytes);
    FMemory::Memcpy(NewAsset->ModelData.GetData(), Buffer, Bytes);

    // Set Defaults
    NewAsset->bDoLowerCase = false;
    NewAsset->bAddBosToken = false;
    NewAsset->bAddEosToken = false;

    return NewAsset;
}