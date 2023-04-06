/*
------------------------------------------------------------
Copyright 2003-200x Haute Ecole ARC Ingénierie,
All rights reserved
------------------------------------------------------------
Nom du fichier : iCpu.h
Auteur et Date : Monnerat Serge, 12 mars 20xx

But : configuration du périphérique MCG

Modifications
Date		Faite		Ctrl		Description

----------------------------------------------------------
*/

#ifndef __ICPU__
#define __ICPU__

#include "def.h"

//------------------------------------------------------------
// Core and peripheral clock init
// External clock = 8MHz
// Core clock = 48MHz
// Peripheral bus = 24MHz
// Flexbus = 24MHz
// Flash = 24MHz
//------------------------------------------------------------
void iCpu_CLKInit(void);

#endif
