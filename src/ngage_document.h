/** @file ngage_document.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef NGAGE_DOCUMENT_H
#define NGAGE_DOCUMENT_H

#include <akndoc.h>

class CNGageAppUi;
class CEikApplication;

class CNGageDocument : public CAknDocument
{
public:
    static CNGageDocument* NewL(CEikApplication& aApp);
    static CNGageDocument* NewLC(CEikApplication& aApp);

    ~CNGageDocument();

public:
    CEikAppUi* CreateAppUiL();

private:
    void ConstructL();
    CNGageDocument(CEikApplication& aApp);
};

#endif /* NGAGE_DOCUMENT_H */
