/*
 * gInput.c
 *
 *  Created on: Apr 10, 2014
 *      Author: serge.monnerat
 */


#include "gInput.h"
#include "gMBox.h"
#include "mSwitch.h"

//-----------------------------------------------------------------------------
// Configuration du gestionnaire
//-----------------------------------------------------------------------------
void gInput_Setup(void)
{
}

//-----------------------------------------------------------------------------
// Exécution du gestionnaire
//-----------------------------------------------------------------------------
void gInput_Execute(void)
{
  SwitchEnum i;
  
  // Lecture des interrupteurs
  for(i=kSw1;i<=kSw8;i++)
    {
      gInput.SwitchTab[i]= mSwitch_ReadSwitch(i);
    }
}
