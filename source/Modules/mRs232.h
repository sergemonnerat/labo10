/*
------------------------------------------------------------
Copyright 2003-201x Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	mRs232.h	
Author and date :	Monnerat Serge 06.06.20xx

Goal : outils afin de pouvoir exploiter le p�riph�rique RS232

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#ifndef __MRS232__
#define __MRS232__

#include "def.h"

//-------------------------------------------------------------------------
// Configuration du module SCI   
//-------------------------------------------------------------------------
void mRs232_Setup(void);

//-------------------------------------------------------------------------
// Ouverture des interfaces    
//-------------------------------------------------------------------------
void mRs232_Open(void);

//-------------------------------------------------------------------------
// Envoi d'un caract�re par RS232
// aChar	: la caract�re � transmettre
//-------------------------------------------------------------------------
void mRs232_WriteChar(unsigned char aChar);

//-------------------------------------------------------------------------
// Envoi d'une cha�ne de caract�res par RS232
// La tramsmission s'arr�te au caract�re NULL
// *aDataPtr	: pointeur sur la cha�ne de caract�res � transmettre, la cha�ne
//              doit-�tre termin�e par le caract�re NULL("ma cha�ne")
//-------------------------------------------------------------------------
void mRs232_WriteString(unsigned char *aDataPtr);

//-----------------------------------------------------------------------------
// Lecture d'un byte du buffer de r�ception. Le buffer est rempli par 
// l'interruption Receive data reg full
// *aBytePtr	  : pointeur sur la variable recevant la donn�e
// Retour 	    : true --> le buffer est vide, false le buffer est pas vide
//-----------------------------------------------------------------------------
bool mRs232_GetDataFromBuffer(UInt8 *aBytePtr);


#endif
