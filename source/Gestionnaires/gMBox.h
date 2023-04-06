/*
 * gMBox.h
 *
 *  Created on: Apr 10, 2014
 *      Author: serge.monnerat
 */

#ifndef __GMBOX__
#define __GMBOX__

#include "def.h"

//-----------------------------------------------------------------------------
// Structure du gestionnaire Input
//-----------------------------------------------------------------------------

#define kNbOfSw 8

typedef struct
{
  bool SwitchTab[kNbOfSw];  // Tableau de l'�tat des interrupteurs
}InputStruct;

extern InputStruct gInput;

//-----------------------------------------------------------------------------
// Structure du gestionnaire Compute
//-----------------------------------------------------------------------------

#define kNbOfErr 8
 
typedef struct
{
  bool ErrTab[kNbOfErr];  // Tableau de l'�tat des erreurs
}ComputeStruct;

extern ComputeStruct gCompute;


#endif /* __GMBOX__ */
