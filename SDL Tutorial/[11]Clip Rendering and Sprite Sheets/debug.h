/*************************************************************************
File name:		debug.h
Description:	helper function(s)
**************************************************************************/

//#include guard.  Prevents double definitions
#ifndef DEBUG_H
#define DEBUG_H

//includes
#include <windows.h>	//for OutputDebugString
#include <sstream>

//declarations
//void OutputMessage()
//Outputs the stringstream ss to the Output tab
//mainly for debugging
//Side effect: clears ss contents after outputting.
void OutputMessage(std::stringstream& ss)
{
	OutputDebugString(ss.str().c_str());
	ss.str("");
	ss.clear();
}

#endif	//#ifndef DEBUG_H