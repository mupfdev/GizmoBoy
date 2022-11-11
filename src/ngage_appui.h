/** @file ngage_appui.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef NGAGE_APPUI_H
#define NGAGE_APPUI_H

#include <aknappui.h>

class CNGageAppView;

class CNGageAppUi : public CAknAppUi
{
public:
    void ConstructL();

    CNGageAppUi();
    ~CNGageAppUi();

public:
    void HandleCommandL(TInt aCommand);

private:
    CNGageAppView* iAppView;
};

#endif /* NGAGE_APPUI_H */
