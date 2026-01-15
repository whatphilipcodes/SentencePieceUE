#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SentencePieceModelFactory.generated.h"

UCLASS()
class USentencePieceModelFactory : public UFactory
{
    GENERATED_BODY()

public:
    USentencePieceModelFactory();

    // The function that runs when you drag a file in
    virtual UObject* FactoryCreateBinary(
        UClass* InClass,
        UObject* InParent,
        FName InName,
        EObjectFlags Flags,
        UObject* Context,
        const TCHAR* Type,
        const uint8*& Buffer,
        const uint8* BufferEnd,
        FFeedbackContext* Warn
    ) override;
};