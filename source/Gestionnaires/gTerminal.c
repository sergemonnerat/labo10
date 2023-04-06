/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�nierie, Switzerland.
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
  // Initialisation de la machine d'�tat
  sTerminalState=kTerminalInit;
}


//-----------------------------------------------------------------------------
// Exécution du gestionnaire
//-----------------------------------------------------------------------------
void gTerminal_Execute(void)
{
  UInt8 aRs232Buff[200];
  UInt8 aByte;
  UInt8 aByteCounter;
  UInt8 i;
  
  // Machine d'�tat de la gestion du Terminal Windows
  switch (sTerminalState)
    {
      // Etat de d�part dans lequel on attend un caract�re (n'importe lequel)
      case kTerminalInit:
      
      // Contr�le si un caract�re est re�u
      if(mRs232_GetDataFromBuffer(aRs232Buff)==false)
        {
          // Prochain �tat
          sTerminalState=kPrintMenu;
        }
      break;
      
      // Affichage du menu
      case kPrintMenu:
        
        // Envoi de la commande permettant de remettre le curseur en haut � gauche
        // de l'�cran --> ESC[H --> ESC = 0x1B
        mRs232_WriteChar(0x1B);
        mRs232_WriteChar('[');
        mRs232_WriteChar('H');
        
        // Ecriture du texte de base du terminal
        // Si l'on veut les caract�res accentu�s il faut s�lectionner
        // l'�mulation TTY dans le terminal windows
        mRs232_WriteString("*** Centrale d'alarme ***\r\n\n");
        mRs232_WriteString("Visualisation de l'�tat des erreurs, veuillez introduire le\r\n");
        mRs232_WriteString("num�ro de l'erreur (0 � 7) :");
      
        // Prochain �tat
        sTerminalState=kPrintAlarm;
      
      break;
      
      // Attente d'un caract�re et affichage de l'�tat d'une alarme
      case kPrintAlarm:
      
        // Contr�le si un caract�re est re�u
        aByteCounter=0;
        while(mRs232_GetDataFromBuffer(&aByte)==false)
          {
            // Enregistrement du byte dans le buffer
            aRs232Buff[aByteCounter]=aByte;
            
            // Compteur de bytes re�us
            aByteCounter++;  
          }
      
      // Contr�le si un caract�re entre 0 et 7 a �t� re�u
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
              
              // Prochain �tat
              sTerminalState=kPrintMenu;
            }
        }

      break;
    }
}
