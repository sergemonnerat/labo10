/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mLcd.c
Auteur et Date :	Monnerat Serge 8.5.20xx

Description dans le fichier iAd.h
------------------------------------------------------------
*/


#include "iPit.h"
#include "iDio.h"
#include "mLcd.h"


// Nb de caract�re max du lcd, ici 2 lignes de 16 caract�res
#define kMaxLcdCarac ((unsigned char)(32))

// Nb de ligne
#define kNbOfLine ((unsigned char)(2))

// Nb de caract�res par ligne
#define kNbOfChar ((unsigned char)(16))

// D�finition des commandes LCD --> data 0 � data 7
// Efface le display et address counter=0
#define kClearDisplay      				((unsigned char)(0x01))	
// Address counter=0	
#define kReturnHome      					((unsigned char)(0x02))
// Lcd en mode 2 lignes,bus de 8 bits			
#define k8BitsMode_2Lines   			((unsigned char)(0x38))
// Enclenche le display sans le curseur 		
#define kDisplayOnNoCursor				((unsigned char)(0x0c))	
// Enclenche le display avec le curseur qui clignote	
#define kDisplayOnWithCursorBlink	((unsigned char)(0x0f))	
// Eteint l'�cran		
#define kDisplayOff								((unsigned char)(0x08))	
// D�placement du curseur vers la droite		
#define kMoveCursorRight					((unsigned char)(0x14))
// D�placement du curseur vers la gauche			
#define kMoveCursorLeft						((unsigned char)(0x10))	
// D�but de la ligne 2		
#define kLineJump         				((unsigned char)(0xc0)) 
// Mode incr�ment, pas de shift �cran 		
#define kIncrModeShiftOff         ((unsigned char)(0x06)) 
// Set DDRAM address		
#define kSetDDRAMAdr              ((unsigned char)(0x80))  	
// Init value		
#define kInitValue              	((unsigned char)(0x30))  	


//-----------------------------------------------------------------------------
// Lecture du Busy Flag
// Retour : �tat du busy flag
//-----------------------------------------------------------------------------
static bool mLcd_ReadLcdBusy(void);

//-----------------------------------------------------------------------------
// Envoi des donn�es au LCD
// aData: les donn�es � transmettre au LCD
//-----------------------------------------------------------------------------
static void mLcd_SendLcdData(unsigned char);

//-----------------------------------------------------------------------------
// Envoi d'une commandes au LCD
// aCmd: la commande � transmettre au Lcd
//-----------------------------------------------------------------------------
static void mLcd_SendLcdCmd(unsigned char);

//------------------------------------------------------------
// Configuration du module mLcd
//------------------------------------------------------------
void mLcd_Setup(void)
{
	// Configuration de la fonctionnalit� de chaque PIN
	iDio_PinConfig(kPortC,kPin13,kAlternate1);
	iDio_PinConfig(kPortC,kPin16,kAlternate1);
	iDio_PinConfig(kPortB,kPin9,kAlternate1);
	iDio_PinConfig(kPortB,kPin20,kAlternate1);
	iDio_PinConfig(kPortE,kPin2,kAlternate1);
	iDio_PinConfig(kPortE,kPin3,kAlternate1);
	iDio_PinConfig(kPortE,kPin6,kAlternate1);
	iDio_PinConfig(kPortE,kPin18,kAlternate1);
	iDio_PinConfig(kPortE,kPin19,kAlternate1);
	iDio_PinConfig(kPortE,kPin20,kAlternate1);
	iDio_PinConfig(kPortE,kPin21,kAlternate1);
  
	// Configuration du Port C, RS et RW
  iDio_SetPortDirection(kPortC,kMaskIo13+kMaskIo16,kIoOutput);
  
  // Configuration du Port B, E et data0
  iDio_SetPortDirection(kPortB,kMaskIo9+kMaskIo20,kIoOutput);
  
  // Configuration du PortE, datas 1...7
  iDio_SetPortDirection(kPortE,kMaskIo2+kMaskIo3+kMaskIo6+kMaskIo18+kMaskIo19+kMaskIo20+kMaskIo21,kIoOutput);
	
}

//-----------------------------------------------------------------------------  
//	Ouverture de l'interface (init du display)
//-----------------------------------------------------------------------------
void mLcd_Open(void)
{
  char   aDelayNb=0;
  
  // Delay suivant la mise sous tension de 15 ms 
  aDelayNb=iPit_GetDelay(15/kPITTime);
  while(!iPit_IsDelayDone(aDelayNb));
	iPit_DelayRelease(aDelayNb);
	
	// Envoi de la commande function set
	mLcd_SendLcdCmd(kInitValue);
	
	// Attente active de 4.1ms
	aDelayNb=iPit_GetDelay(5/kPITTime);
	while(!iPit_IsDelayDone(aDelayNb));
	iPit_DelayRelease(aDelayNb);
	
	// Envoi de la commande function set
	mLcd_SendLcdCmd(kInitValue);
	
	// Attente active de 100 us (min possible 1 ms)
	aDelayNb=iPit_GetDelay(kPITTime/kPITTime);
	while(!iPit_IsDelayDone(aDelayNb));
	iPit_DelayRelease(aDelayNb);
	
	// Envoi de la commande function set
	mLcd_SendLcdCmd(kInitValue);
	
	// ----------------------------------------
	// A partir d'ci on peut utiliser le busy
	// ----------------------------------------
	
	// Envoi de la commande function set: 2 lignes, interface 8 bits
	while(true==mLcd_ReadLcdBusy());
	mLcd_SendLcdCmd(k8BitsMode_2Lines);
	
	// Display OFF
	while(true==mLcd_ReadLcdBusy());
	mLcd_SendLcdCmd(kDisplayOff);
	
	// Efface l'�cran
	while(true==mLcd_ReadLcdBusy());
	mLcd_SendLcdCmd(kClearDisplay);
	
	// Entry Mode set, mode incr�ment pas de shift �cran
	while(true==mLcd_ReadLcdBusy());	
	mLcd_SendLcdCmd(kIncrModeShiftOff);
	
	// Display ON, pas de curseur
	while(true==mLcd_ReadLcdBusy());			
	mLcd_SendLcdCmd(kDisplayOnNoCursor);
}

//-----------------------------------------------------------------------------
// Envoi d'une commandes au LCD
// aCmd: la commande � transmettre au Lcd
//-----------------------------------------------------------------------------
static void mLcd_SendLcdCmd(unsigned char aCmd)
{
  // Bit E (start data read/write) � 0
 	iDio_SetPort(kPortB,kMaskIo9,kIoOff);
 	
  // Ecriture de la commande sur le port E et B
 	iDio_PortLcd(aCmd);
  
  // Bit RS=0 --> on s�lectionne les registres d'instruction
  iDio_SetPort(kPortC,kMaskIo13,kIoOff);
  
  // Bit RW=0 --> on �crit
  iDio_SetPort(kPortC,kMaskIo16,kIoOff);
    
  // Bit E (start data read/write) � 1
  // Le bits E doit �tre � 1 pendant au moins 230nsec
  iDio_SetPort(kPortB,kMaskIo9,kIoOn);
  
  // Bit E (start data read/write) � 0
	// Les data sont lues � ce moment l� par l'affichage
	iDio_SetPort(kPortB,kMaskIo9,kIoOff);
}

//-----------------------------------------------------------------------------
// Envoi des donn�es au LCD
// aData: les donn�es � transmettre au LCD
//-----------------------------------------------------------------------------
static void mLcd_SendLcdData(unsigned char aData)
{
  // Bit E (start data read/write) � 0
 	iDio_SetPort(kPortB,kMaskIo9,kIoOff);
  
  // Bit RS=1 --> on s�lectionne les registres de donn�es
  iDio_SetPort(kPortC,kMaskIo13,kIoOn);
  
  // Bit RW=0 --> on �crit
  iDio_SetPort(kPortC,kMaskIo16,kIoOff);
  
  // Ecriture la donn�e sur le port E
  iDio_PortLcd(aData);
    
  // Bit E (start data read/write) � 1
  // Le bits E doit �tre � 1 pendant au moins 230nsec
  iDio_SetPort(kPortB,kMaskIo9,kIoOn);
  
  // Bit E (start data read/write) � 0
  // Les data sont lues � ce moment l� par l'affichage
	iDio_SetPort(kPortB,kMaskIo9,kIoOff);
}

//-----------------------------------------------------------------------------
// Lecture du Busy Flag
// Retour : �tat du busy flag
//-----------------------------------------------------------------------------
static bool mLcd_ReadLcdBusy(void)
{
 	bool aVal;
 	
 	// Configuration du Port B --> data 0
 	iDio_SetPortDirection(kPortB,kMaskIo20,kIoInput);
 	// Configuration du PortE, datas 1...7
 	iDio_SetPortDirection(kPortE,kMaskIo2+kMaskIo3+kMaskIo6+kMaskIo18+kMaskIo19+kMaskIo20+kMaskIo21,kIoInput);

 	// Bit E (start data read/write) � 0
 	iDio_SetPort(kPortB,kMaskIo9,kIoOff);

	// Bit RS=0 --> on s�lectionne les registres d'instruction
  iDio_SetPort(kPortC,kMaskIo13,kIoOff);
  
  // Bit RW=1, read
  iDio_SetPort(kPortC,kMaskIo16,kIoOn);
  	
	// Bit E (start data read/write) � 1
	// E (start read) doit �tre ins�r� pendant au moins 230ns
	// Les donn�es sont valides apr�s 120ns.
	iDio_SetPort(kPortB,kMaskIo9,kIoOn);
 	
 	// Lecture du data 7 --> busy flag
 	// On lit le flag avant de redescendre E afin d'�tre sur
 	// de ne pas le manquer car il est valide uniquement pendant
 	// 300 ns apr�s que redescende � 0
 	aVal=iDio_GetPort(kPortE,kMaskIo19);
 	
 	// Bit E (start data read/write) � 0
 	iDio_SetPort(kPortB,kMaskIo9,kIoOff);
 	
 	// Configuration du Port B --> data 0
 	iDio_SetPortDirection(kPortB,kMaskIo20,kIoOutput);
 	// Configuration du PortE, datas 1...7
 	iDio_SetPortDirection(kPortE,kMaskIo2+kMaskIo3+kMaskIo6+kMaskIo18+kMaskIo19+kMaskIo20+kMaskIo21,kIoOutput);

 	// Retour de l'�tat du busy flag
 	return aVal; 
}


//-----------------------------------------------------------------------------
//	Ecriture sur le LCD complet
// aChar: Pointeur sur la cha�ne de charact�re � afficher 32 caract�res
//-----------------------------------------------------------------------------
void mLcd_WriteEntireDisplay(char *aChar)
{

	unsigned int i;
	
	// Attente du busy flag=0
	while(true==mLcd_ReadLcdBusy());
	
	// AC � 0, d�but de la 1�re ligne
	mLcd_SendLcdCmd(kReturnHome);
	
	// Envoi des 32 caract�res � l'affichage
	// Attention il faut position manuellement l'Address Counter
	// au d�but de la 2e ligne lors de l'�criture de cette derni�re
	for (i=0;i<kMaxLcdCarac;i++) 
		{
	  	// Attente du busy=0
			while(true==mLcd_ReadLcdBusy());
			
			// Envoi du caract�re au LCD
			mLcd_SendLcdData(*aChar);
			
			// Si fin de 1�re ligne on passe � la 2e
			if((kNbOfChar-1)==i)
				{
					// Attente du busy flag=0
					while(true==mLcd_ReadLcdBusy());
	
					// 2e ligne
					mLcd_SendLcdCmd(kLineJump);
				}
			
			// Caract�re suivant
			aChar++;
	  }
}

//-----------------------------------------------------------------------------
// Ecriture sur le LCD d'un caract�re � un endroit pr�cis
// aChar: Le caract�re que l'on veut afficher
// aXpos: Position horizontale du 1er caract�re
// aYPos: Position verticale du 1er caract�re  
//-----------------------------------------------------------------------------
void mLcd_Write(unsigned char aChar,unsigned char aXpos,unsigned char aYPos)
{
	unsigned int aTmp=0;
		
	// Modification de l'Address Counter AC
	// Calcul de l'adresse DDRAM
	aTmp=kSetDDRAMAdr+aXpos+(aYPos*0x40);
	
	// Attente du busy flag=0
	while(true==mLcd_ReadLcdBusy());
	
	// Set DDRAM Address Counter
	mLcd_SendLcdCmd(aTmp);
	
	// Attente du busy flag=0
	while(true==mLcd_ReadLcdBusy());
	
	// Envoi du caract�re au LCD
	mLcd_SendLcdData(aChar);
}
