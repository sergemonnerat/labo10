/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	main.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier 
------------------------------------------------------------
*/

#include "mLeds.h"
#include "mSwitch.h"
#include "mDelay.h"
#include "mLcd.h"
#include "mRs232.h"
#include "mCpu.h"
#include "def.h"
#include "gInput.h"
#include "gCompute.h"
#include "gOutput.h"
#include "gTerminal.h"
#include "assert.h"
#include "stdio.h"

//-------------------------------------------------------------------------
// Déclaration des constantes
//-------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Déclaration des variables globales
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Programme principal
//-----------------------------------------------------------------------------
int main(void) 
{   
	// Congiguration générale de la CPU
	mCpu_Setup();
	
	// Configuration du module LEDS
	mLeds_Setup();
	
	// Configuration du module Switch
	mSwitch_Setup();
	
	// Configuration du module Delay
	mDelay_Setup();
	
	// Configuration du module LCD
	mLcd_Setup();
	
	// Configuration du module Rs232
	mRs232_Setup();
	
	// Enable des interruptions au niveau de la CPU
	// --> Primask
	EnableInterrupts;
	
	// Ouverture du module LCD
	mLcd_Open();
	
	// Ouverture du module Rs232
	mRs232_Open();
	
	// Configuration du gestionnaire d'entrées
	gInput_Setup();
	
	// Configuration du gestionnaire de calculs
	gCompute_Setup();
	
	// Configuration du gestionnaire de sorties
	gOutput_Setup();
	
	// Configuration du gestionnaire terminal
	gTerminal_Setup();

	// Boucle infinie
	for(;;) 
		{
			// Exécution des gestionnaires
			gInput_Execute();
			gCompute_Execute();
			gOutput_Execute();
			gTerminal_Execute();
		}
}


