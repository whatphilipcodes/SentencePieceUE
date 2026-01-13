#include "Modules/ModuleManager.h"

class FSentencePieceUEModule : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FSentencePieceUEModule, SentencePieceUE)