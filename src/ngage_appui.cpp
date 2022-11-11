/** @file ngage_appui.cpp
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#include <avkon.hrh>
#include <aknnotewrappers.h>

#include "ngage_appui.h"
#include "ngage_appview.h"

void CNGageAppUi::ConstructL()
{
    BaseConstructL();

    iAppView = CNGageAppView::NewL(ClientRect());

    AddToStackL(iAppView);
}

CNGageAppUi::CNGageAppUi()
{
    RProcess Proc;

    iAppView = NULL;

    if (KErrNone == Proc.Create(_L("E:\\System\\Apps\\GizmoBoy\\game.exe"), _L("")))
    {
        Proc.Resume();
        Proc.Close();
        User::After(10000000);
        Exit();
    }
    else
    {
        Exit();
    }
}

CNGageAppUi::~CNGageAppUi()
{
    if (iAppView)
    {
        RemoveFromStack(iAppView);
        delete iAppView;
        iAppView = NULL;
    }
}

void CNGageAppUi::HandleCommandL(TInt aCommand)
{
    /* No implementation required. */
}
