// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#include "CombatComponents.h"

#define LOCTEXT_NAMESPACE "FCombatComponentsModule"

void FCombatComponentsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCombatComponentsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCombatComponentsModule, CombatComponents)