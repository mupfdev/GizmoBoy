/** @file ngage.cpp
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#include "ngage_application.h"

GLDEF_C TInt E32Dll(TDllReason /* aReason */)
{
    return KErrNone;
}

EXPORT_C CApaApplication* NewApplication()
{
    return (new CNGageApplication);
}
