/*
 * mDelay.c
 *
 *  Created on: Apr 3, 2014
 *      Author: serge.monnerat
 */


#include "iPit.h"
#include "mDelay.h"

#define kPit0Per 1
#define kPit1Per 10

//------------------------------------------------------------
// Configuration du module mDelay
//------------------------------------------------------------
void mDelay_Setup(void)
{
	// Configuration des PIT
	iPit_Config(kPit0Per,kPit1Per);
	
	// D�marrage du PIT0
	iPit_StartPit(kPit0);
	
	// Initialise la structure des compteurs de temps
	iPit_InitDelay();
}


//------------------------------------------------------------
// Lecture du flag indiquant que 500ms sont �chus
// Retour  : �tat du flag
//------------------------------------------------------------
bool mDelay_GetFlag(void)
{
  return iPit_GetFlag(); 
}

//------------------------------------------------------------
// Reset du flag indiquant que 500ms sont �chus
// Retour  : �tat du flag
//------------------------------------------------------------
void mDelay_ResetFlag(void)
{
		iPit_ResetFlag(); 
}

//------------------------------------------------------------
// Configuration des compteurs de temps
// aDelay	: le temps � �couler
// Retour	: le num�ro du compteur de temps (-1) si plus de 
//					compteurs libres
//------------------------------------------------------------
int mDelay_GetDelay(unsigned int aDelay)
{			
	return iPit_GetDelay(aDelay);
} 

//------------------------------------------------------------
// Contr�le si le d�lais est �chu
// aDelayNb	: le num�ro du compteur de temps
// Retour		: l'�tat du flag
//------------------------------------------------------------
bool mDelay_IsDelayDone(unsigned int aDelayNb)
{
	return iPit_IsDelayDone(aDelayNb);
}

//------------------------------------------------------------
// Lib�re un compteur de temps
// aDelayNb	: le num�ro du compteur de temps
//------------------------------------------------------------
void mDelay_DelayRelease(unsigned int aDelayNb)
{
	iPit_DelayRelease(aDelayNb);
}

