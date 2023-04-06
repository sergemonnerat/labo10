/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland.
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
// Lecture du flag indiquant que 500ms sont �chus
// Retour  : �tat du flag
//------------------------------------------------------------
bool mDelay_GetFlag(void);

//------------------------------------------------------------
// Reset du flag indiquant que 500ms sont �chus
// Retour  : �tat du flag
//------------------------------------------------------------
void mDelay_ResetFlag(void);

//------------------------------------------------------------
// Configuration des compteurs de temps
// aDelay	: le temps � �couler
// Retour	: le num�ro du compteur de temps (-1) si plus de 
//					compteurs libres
//------------------------------------------------------------
int mDelay_GetDelay(unsigned int aDelay);

//------------------------------------------------------------
// Contr�le si le d�lais est �chu
// aDelayNb	: le num�ro du compteur de temps
// Retour		: l'�tat du flag
//------------------------------------------------------------
bool mDelay_IsDelayDone(unsigned int aDelayNb);

//------------------------------------------------------------
// Lib�re un compteur de temps
// aDelayNb	: le num�ro du compteur de temps
//------------------------------------------------------------
void mDelay_DelayRelease(unsigned int aDelayNb);

#endif
