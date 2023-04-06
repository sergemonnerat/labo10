/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	mLcd.h	
Auteur et Date :	Monnerat Serge 8.5.20xx

But : module LCD permettant de piloter l'�cran LCD

Modifications
Date		Faite	Ctrl		Description
------------------------------------------------------------
*/

#ifndef __MLCD__
#define __MLCD__

#include "def.h"

//------------------------------------------------------------
// Configuration du module mLcd
//------------------------------------------------------------
void mLcd_Setup(void);

//-----------------------------------------------------------------------------  
//	Ouverture de l'interface (init du display)
//-----------------------------------------------------------------------------
void mLcd_Open(void);

//-----------------------------------------------------------------------------
//	Ecriture sur le LCD complet
// aChar: Pointeur sur la cha�ne de charact�re � afficher 32 caract�res
//-----------------------------------------------------------------------------
void mLcd_WriteEntireDisplay(char *aChar);

//-----------------------------------------------------------------------------
// Ecriture sur le LCD d'un caract�re � un endroit pr�cis
// aChar: Le caract�re que l'on veut afficher
// aXpos: Position horizontale du 1er caract�re
// aYPos: Position verticale du 1er caract�re  
//-----------------------------------------------------------------------------
void mLcd_Write(unsigned char aChar,unsigned char aXpos,unsigned char aYPos);


#endif
