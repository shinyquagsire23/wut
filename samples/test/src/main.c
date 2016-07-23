#include <coreinit/core.h>
#include <coreinit/debug.h>
#include <coreinit/thread.h>
#include <coreinit/internal.h>
#include <coreinit/heap.h>
#include <stdlib.h>
#include <stdio.h>

const char *asdf_str = "asdf";
int something = 1;

int
CoreEntryPoint(int argc, const char **argv)
{
   OSReport("Hello world from %s\n", argv[0]);
   something = 2;
   return argc;
}

int
main(int argc, char **argv)
{
   OSReport("Main thread running on core %d\n", OSGetCoreId());

   // Run thread on core 0
   OSThread *threadCore0 = OSGetDefaultThread(0);

   const char *core0Args[] = {
      "Core 0"
   };

   OSRunThread(threadCore0, CoreEntryPoint, 0, core0Args);

   // Run thread on core 2
   OSThread *threadCore2 = OSGetDefaultThread(2);

   const char *core2Args[] = {
      "Core 2"
   };

   OSRunThread(threadCore2, CoreEntryPoint, 2, core2Args);

   // Wait for threads to return
   int resultCore0 = -1, resultCore2 = -1;
   OSJoinThread(threadCore0, &resultCore0);
   OSJoinThread(threadCore2, &resultCore2);

   OSReport("Core 0 thread returned %d %x\n", resultCore0, something);
   OSReport("Core 2 thread returned %d\n", resultCore2);
   char *asdf = malloc(0x123);
   
   //Test global vars, which use .rela.dyn relocations for .sdata
   void *addr = asdf_str;
   unsigned int val = *(unsigned int*)addr;
   __os_snprintf(asdf, 0x123, "%x its: %x %x\n", addr, val);
   OSReport(asdf);
   
   //Test sprintf
   snprintf(asdf, 0x123, "%s %x", "hello sprintf!", 0x12345);
   OSFatal(asdf);
   return 0;
}
