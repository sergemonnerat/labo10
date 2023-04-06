/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�nierie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier :	gOutput.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier gOutput.h
------------------------------------------------------------
*/

#include "gOutput.h"
#include "gMBox.h"
#include "mLeds.h"
#include "mDelay.h"
#include "mLcd.h"
#include "def.h"

//-------------------------------------------------------------------------
// D�claration des constantes
//-------------------------------------------------------------------------

// Les temps d'attente pour chaque LED
#define kLedDelay (1000/kPITTime)

//-----------------------------------------------------------------------------
// D�claration des variables globales
//-----------------------------------------------------------------------------

// Enum�ration des �tats possible de la machine d'�tat
typedef enum
{
  kAffichage,
  kWait1Sec
}AffStateMachineEnum;

// Variable de la machine d'�tat de l'affichage
static AffStateMachineEnum sAffState=kAffichage;

// Nu du Delay de chaque LED
static unsigned int sDelayLed=0;

// Compteur utilis� pour afficher l'�tat de chaque erreur
static unsigned int sAlarmNb=0;

//-----------------------------------------------------------------------------
// Configuration du gestionnaire
//-----------------------------------------------------------------------------
void gOutput_Setup(void)
{
  // Ecriture du message de base
  mLcd_WriteEntireDisplay("Central Alarme  Alarme 0 OFF    ");
}

//-----------------------------------------------------------------------------
// Ex�cution du gestionnaire
//-----------------------------------------------------------------------------
void gOutput_Execute(void)
{
  unsigned int i,j;
  
  // Lecture des interrupteurs et commande des LED
  for(i=0,j=1;i<kNbOfErr;i++,j<<=1)
    {
      // affectation de l'�tat des interrupteurs aux LEDS
      if(gCompute.ErrTab[i]==true)
        {
          mLeds_Write(j,kLedOn);
        }
      else
        {
          mLeds_Write(j,kLedOff);
        }
    }
  
  // Affichage de l'�tat de chaque erreur
  switch(sAffState)
    {
      case kAffichage:
    
      // Affichage du message
      if(gCompute.ErrTab[sAlarmNb]==true)
        {
          mLcd_Write(sAlarmNb+0x30,7,1);
          mLcd_Write('O',9,1);
          mLcd_Write('N',10,1);
          mLcd_Write(' ',11,1);
        }
      else
        {
          mLcd_Write(sAlarmNb+0x30,7,1);
          mLcd_Write('O',9,1);
          mLcd_Write('F',10,1);
          mLcd_Write('F',11,1);
        }
    
      // Prise d'un delay
      sDelayLed=mDelay_GetDelay((unsigned int)kLedDelay);
      
      // prochain �tat
      sAffState=kWait1Sec;
      
   break;
   
    case kWait1Sec:
      // Si le delay est �chu
      if(mDelay_IsDelayDone(sDelayLed)==true)
        {
         // On rel�che le delay
         mDelay_DelayRelease(sDelayLed);
         
         // Erreur suivante
         sAlarmNb++;
         
         // Saturation
         if(sAlarmNb>=kNbOfErr)
          {
           sAlarmNb=0; 
          }
         // Prochain �tat
         sAffState=kAffichage; 
        }
   break; 
  }
}
