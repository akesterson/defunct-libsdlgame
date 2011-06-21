#ifdef __WIN32__
#include <windows.h>
#endif

#ifndef __WIN32__
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* Backtrace.cpp
 * 
 * This file is compiled for OS X and Linux targets. mingw32 targets
 * use backtrace pulled in the dependencies. 
 */

/* Obtain a backtrace and print it to stdout. */
void __libsdlgame_show_stacktrace (void)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  // The heap may be corrupt at this point, so this may be an 
  // invitation to disaster, but we don't have much choice.
  strings = backtrace_symbols (array, size);

  fprintf(stderr, "Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
    fprintf(stderr, "%s\n", strings[i]);

  free (strings);
}

void __libsdlgame_sigsegvhandler(int z)
{
  fprintf(stderr, "Caught segmentation fault; stacktrace follows.\n");
  __libsdlgame_show_stacktrace();
#ifdef DEBUG_LOOPONSEGFAULT
  fprintf(stderr, "Entering infinite loop; please connect a debugger to me and then kill me.\n");
  fflush(stderr);
  while ( 1 ) { };
#endif
  exit(1);
}

#endif

#include <stdlib.h>
#include <iostream>

void RegisterStacktrace()
{
#ifdef __WIN32__
  if ( LoadLibraryA("backtrace.dll") == NULL ) {
    std::cerr << "Unable to locate backtrace.dll.\n";
    exit(1);
  }
#else
  signal(SIGSEGV, &__libsdlgame_sigsegvhandler);
#endif
}
