#include "stdafx.h"
#include "Core.h"
#include <ctime>
#include <iostream>
#include <exception>

void StartHeapControl( );
void DumpMemoryLeaks( );

int main( int argc, char *argv[] )
{
	srand(int(time(nullptr)));
	{ // Make sure stack objects go out of scope before memory leaks are dumped
		StartHeapControl();

		try 
		{
			Core core{ Window{ "Fins Adventure - Daemen, Ienne - 1DAE18", 360.0f, 360.0f } };
			core.Run();
		}
		catch (std::exception ex)
		{
			std::cout << "An exception was caught of std::exception\n";
		}
		catch (...)
		{
			std::cout << "An exception of unknown type was caught!\n";
		}
	}
	DumpMemoryLeaks( );
	return 0;
}

void StartHeapControl( )
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, 0 );

	// Report detected leaks when the program exits
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 156 );
#endif
}

void DumpMemoryLeaks( )
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks( );
#endif
}

