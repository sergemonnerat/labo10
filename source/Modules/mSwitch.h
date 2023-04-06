/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	mSwitch.h
Auteur et Date :	Monnerat Serge 8.5.20xx

But : module Switch permettant d'acqu�rir l'�tat des interrupteurs
			et poussoirs

Modifications
Date		Faite	Ctrl		Description
------------------------------------------------------------
*/

#ifndef __MSWITCH__
#define __MSWITCH__

#include"def.h"

// Enum�r� des interrupteurs (SW0 � Sw7 sur le sch�ma)
typedef enum
{
	kSw1=0,
	kSw2=1,
	kSw3=2,
	kSw4=3,
	kSw5=4,
	kSw6=5,
	kSw7=6,
	kSw8=7
}SwitchEnum;

// Enum�r� des boutons poussoirs (SW0...SW3 sur le sch�ma)
typedef enum
{
	kPushButSW1=0,
	kPushButSW2=1,
	kPushButSW3=2,
	kPushButSW4=3
}PushButEnum;

//-----------------------------------------------------------------------------
// Configuration des ports IO utilis�s par les interrupteurs et les poussoirs
//-----------------------------------------------------------------------------
void mSwitch_Setup(void);

//-------------------------------------------------------------------------
// Lecture des interrupteurs
// aSwitchNb: quel interrupteur veut-on lire
// Retour: �tat du switch
//-------------------------------------------------------------------------
bool mSwitch_ReadSwitch(SwitchEnum aSwitch);

//-------------------------------------------------------------------------
// Lecture des boutons poussoir
// aPushButNb: quel boutons poussoir veut-on lire
// Retour: �tat du bouton poussoir
//-------------------------------------------------------------------------
bool mSwitch_ReadPushBut(PushButEnum aPushButNb);

#endif
