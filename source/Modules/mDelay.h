/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	mDelay.h
Auteur et Date :	Monnerat Serge 8.5.20xx

But : module Delay permettant de realiser des attentes

Modifications
Date		Faite	Ctrl		Description
------------------------------------------------------------
*/

#ifndef __MDELAY__
#define __MDELAY__

//------------------------------------------------------------
// Configuration du module mDelay
//------------------------------------------------------------
void mDelay_Setup(void);

//------------------------------------------------------------
// Lecture du flag indiquant que 500ms sont échus
// Retour  : état du flag
//------------------------------------------------------------
bool mDelay_GetFlag(void);

//------------------------------------------------------------
// Reset du flag indiquant que 500ms sont échus
// Retour  : état du flag
//------------------------------------------------------------
void mDelay_ResetFlag(void);

//------------------------------------------------------------
// Configuration des compteurs de temps
// aDelay	: le temps à écouler
// Retour	: le numéro du compteur de temps (-1) si plus de 
//					compteurs libres
//------------------------------------------------------------
int mDelay_GetDelay(unsigned int aDelay);

//------------------------------------------------------------
// Contrôle si le délais est échu
// aDelayNb	: le numéro du compteur de temps
// Retour		: l'état du flag
//------------------------------------------------------------
bool mDelay_IsDelayDone(unsigned int aDelayNb);

//------------------------------------------------------------
// Libère un compteur de temps
// aDelayNb	: le numéro du compteur de temps
//------------------------------------------------------------
void mDelay_DelayRelease(unsigned int aDelayNb);

#endif
