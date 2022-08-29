/*
//  MBRMgr.c
//
//    Master Boot Record Manager (MBR) Manager
//	Deletes the partition table from a hard drive
//
//
//      usage:
//
//    MBRMgr <Options> HD#
//
//       /NOWARN  Does not display a warning before deleting partition
//       /QUIET   Does not display info messages
//
//
//       HD#   0 = Hard Disk 0, 1 = Hard Disk 1, etc.
//
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <bios.h>

#define FALSE 0
#define TRUE 1

int main(int argc, char *argv[])
{
   int bNoWarn = FALSE;
   int bQuiet = FALSE;
   int iDrive = -1;
   char bBuffer[512];
   int count = 0;
   struct diskinfo_t diskinfo;

   memset(bBuffer, 0, sizeof(bBuffer));

   /*
   // check argument count
   */
   if (argc < 2)
   {
      printf("\n\nMaster Boot Record (MBR Manager)\n"
             "\tDeletes the partition table from a hard drive.\n\n"
             "\tUsage:\n\n"
             "\tMBRMgr <Options> HD#\n\n"
             "\t\t/NOWARN  Does not display a warning before deleting partition.\n"
             "\t\t/QUIET   Does not display info messages.\n\n"
             "\t\tHD#   0 = Hard Disk 0, 1 = Hard Disk 1, etc.\n\n");

      return -1;
   }

   /*
   // loop thru the commandline
   */
   for (count = 1; count < argc; count++)
   {

      /*
      //      cout << "argv[" << count << "]   " << argv[count] << "\n";
      */
      /*
      // check for options
      */
      if ('/' == argv[count][0])
      {
         /*
         // No warning
         */
         if (!strcmp("/NOWARN", strupr(argv[count])))
         {
            bNoWarn = TRUE;
         }

         // Quiet
         else if (!strcmp("/QUIET", strupr(argv[count])))
         {
            bQuiet = TRUE;
         }
      }

      /*
      // not an option, must be physical drive #
      */
      else
      {
         /*
         // make sure arg is a digit
         */
         if (isdigit(argv[count][0]))
         {
            iDrive = atoi(argv[count]);
         }
      }
   }

   /*
   // make sure that we have a valid drive #
   */
   if (iDrive < 0)
   {
      if (!bQuiet)
      {
         printf("\nInvalid Drive Number\n");
      }
      return -1;
   }

   if (!bNoWarn)
   {
      printf("Delete Partition Table from Physical Drive %d? <N>", iDrive);
      count = getch();
      printf("\n");

      // only proceed if y or Y was entered
      if ((121 != count) && (89 != count))
      {
         return 0;
      }
   }

   diskinfo.drive = iDrive + 0x80;
   diskinfo.head = 0;
   diskinfo.track = 0;
   diskinfo.sector = 1;
   diskinfo.nsectors = 1;
   diskinfo.buffer = bBuffer;

   count = _bios_disk(_DISK_WRITE, &diskinfo);

   if (!bQuiet)
   {
      if (0 == count)
      {
         printf("SUCCESS - Deleting Partition Table from Physical Drive %d\n", iDrive);
      }
      else
      {
         printf("ERROR - Deleting Partition Table from Physical Drive %d\n", iDrive);
      }
   }

   return 0;
}
