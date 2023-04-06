/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingénierie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier :	gTerminal.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier gTerminal.h
------------------------------------------------------------
*/

#include "mRs232.h"
#include "gMBox.h"
#include "gTerminal.h"

// Etat possible du terminal
typedef enum
{
  kTerminalInit,
  kPrintMenu,
  kPrintAlarm 
}TerminalStateEnum;

static TerminalStateEnum sTerminalState;

//-----------------------------------------------------------------------------
// Configuration du gestionnaire
//-----------------------------------------------------------------------------
void gTerminal_Setup(void)
{
  // Initialisation de la machine d'ï¿½tat
  sTerminalState=kTerminalInit;
}


//-----------------------------------------------------------------------------
// ExÃ©cution du gestionnaire
//-----------------------------------------------------------------------------
void gTerminal_Execute(void)
{
  UInt8 aRs232Buff[200];
  UInt8 aByte;
  UInt8 aByteCounter;
  UInt8 i;
  
  // Machine d'état de la gestion du Terminal Windows
  switch (sTerminalState)
    {
      // Etat de départ dans lequel on attend un caractère (n'importe lequel)
      case kTerminalInit:
      
      // Contrôle si un caractère est reçu
      if(mRs232_GetDataFromBuffer(aRs232Buff)==false)
        {
          // Prochain état
          sTerminalState=kPrintMenu;
        }
      break;
      
      // Affichage du menu
      case kPrintMenu:
        
        // Envoi de la commande permettant de remettre le curseur en haut à  gauche
        // de l'écran --> ESC[H --> ESC = 0x1B
        mRs232_WriteChar(0x1B);
        mRs232_WriteChar('[');
        mRs232_WriteChar('H');
        
        // Ecriture du texte de base du terminal
        // Si l'on veut les caractères accentués il faut sélectionner
        // l'émulation TTY dans le terminal windows
        mRs232_WriteString("*** Centrale d'alarme ***\r\n\n");
        mRs232_WriteString("Visualisation de l'état des erreurs, veuillez introduire le\r\n");
        mRs232_WriteString("numéro de l'erreur (0 à 7) :");
      
        // Prochain état
        sTerminalState=kPrintAlarm;
      
      break;
      
      // Attente d'un caractère et affichage de l'état d'une alarme
      case kPrintAlarm:
      
        // Contrôle si un caractère est reçu
        aByteCounter=0;
        while(mRs232_GetDataFromBuffer(&aByte)==false)
          {
            // Enregistrement du byte dans le buffer
            aRs232Buff[aByteCounter]=aByte;
            
            // Compteur de bytes reçus
            aByteCounter++;  
          }
      
      // Contrôle si un caractère entre 0 et 7 a été reçu
      for(i=0;i<aByteCounter;i++)
        {
          if((aRs232Buff[i]>=0x30)&&(aRs232Buff[i]<=0x37))
            {
              mRs232_WriteChar(aRs232Buff[i]);
              mRs232_WriteString("\r\n\n");
              mRs232_WriteString("L'alarme ");
              mRs232_WriteChar(aRs232Buff[i]);
              if(gCompute.ErrTab[aRs232Buff[i]-0x30]==true)
                {
                  mRs232_WriteString(" est ON \r\n\n");
                }
              else
                {
                  mRs232_WriteString(" est OFF\r\n\n"); 
                }
              
              // Prochain état
              sTerminalState=kPrintMenu;
            }
        }

      break;
    }
}
