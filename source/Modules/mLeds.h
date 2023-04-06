/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	mLeds.h
Auteur et Date :	Monnerat Serge 8.5.20xx

But : module LED permettant de piloter les LEDs

Modifications
Date		Faite	Ctrl		Description
------------------------------------------------------------
*/

#ifndef __MLEDS__
#define __MLEDS__

#include"def.h"

// Masque des LED
typedef enum
{
  kMaskLed1=0x1,
  kMaskLed2=0x2,
  kMaskLed3=0x4,
  kMaskLed4=0x8,
  kMaskLed5=0x10,
  kMaskLed6=0x20,
  kMaskLed7=0x40,
  kMaskLed8=0x80,
  kMaskLedAll=0xFF
}LedMaskEnum;

// Etat des LED
typedef enum
{
  kLedOff,
  kLedOn
}LedStateEnum;

//-----------------------------------------------------------------------------
// Configuration des ports IO utilisés par les LEDS
//-----------------------------------------------------------------------------
void mLeds_Setup(void);

//-----------------------------------------------------------------------------
// On Off des LED
// aMask:   choix des LED sur lesquelles on veut agir (si bit à 1)
// aState:  choix de l'état des LED désirés
//-----------------------------------------------------------------------------
void mLeds_Write(LedMaskEnum aMask,LedStateEnum aState);

#endif
