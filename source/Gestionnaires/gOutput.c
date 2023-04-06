/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingénierie, Switzerland.
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
// Déclaration des constantes
//-------------------------------------------------------------------------

// Les temps d'attente pour chaque LED
#define kLedDelay (1000/kPITTime)

//-----------------------------------------------------------------------------
// Déclaration des variables globales
//-----------------------------------------------------------------------------

// Enumération des états possible de la machine d'état
typedef enum
{
  kAffichage,
  kWait1Sec
}AffStateMachineEnum;

// Variable de la machine d'état de l'affichage
static AffStateMachineEnum sAffState=kAffichage;

// Nu du Delay de chaque LED
static unsigned int sDelayLed=0;

// Compteur utilisé pour afficher l'état de chaque erreur
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
// Exécution du gestionnaire
//-----------------------------------------------------------------------------
void gOutput_Execute(void)
{
  unsigned int i,j;
  
  // Lecture des interrupteurs et commande des LED
  for(i=0,j=1;i<kNbOfErr;i++,j<<=1)
    {
      // affectation de l'état des interrupteurs aux LEDS
      if(gCompute.ErrTab[i]==true)
        {
          mLeds_Write(j,kLedOn);
        }
      else
        {
          mLeds_Write(j,kLedOff);
        }
    }
  
  // Affichage de l'état de chaque erreur
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
      
      // prochain état
      sAffState=kWait1Sec;
      
   break;
   
    case kWait1Sec:
      // Si le delay est échu
      if(mDelay_IsDelayDone(sDelayLed)==true)
        {
         // On relâche le delay
         mDelay_DelayRelease(sDelayLed);
         
         // Erreur suivante
         sAlarmNb++;
         
         // Saturation
         if(sAlarmNb>=kNbOfErr)
          {
           sAlarmNb=0; 
          }
         // Prochain état
         sAffState=kAffichage; 
        }
   break; 
  }
}
