// Utilities.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Utilities.h"


// This is an example of an exported variable
UTILITIES_API int nUtilities=0;

// This is an example of an exported function.
UTILITIES_API int fnUtilities(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see Utilities.h for the class definition
CUtilities::CUtilities()
{
    return;
}
