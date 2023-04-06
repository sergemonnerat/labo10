/*
 * gCompute.c
 *
 *  Created on: Apr 10, 2014
 *      Author: serge.monnerat
 */


#include "gCompute.h"
#include "gMBox.h"

//-----------------------------------------------------------------------------
// Configuration du gestionnaire
//-----------------------------------------------------------------------------
void gCompute_Setup(void)
{
}

//-----------------------------------------------------------------------------
// Exécution du gestionnaire
//-----------------------------------------------------------------------------
void gCompute_Execute(void)
{
  unsigned int i;
  
  // Lecture des interrupteurs et affectation des erreurs
  for(i=0;i<kNbOfErr;i++)
    {
      gCompute.ErrTab[i]= gInput.SwitchTab[i];
    }
}
