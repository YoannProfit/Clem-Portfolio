//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Defines.h
// Date de création : Octobre 2015
//
// Description : Un fichier d'en-tête qui définit toutes 
// les définitions et macros utilisées par le programme
//----------------------------------------------------

/***** Video Memory *****/

/* Adresse de la mémoire centrale où
   l'on doit stocker les données texte du Shell
   Voir VideoMemory.cpp */
#define GEM_VIDEO_MEMORY_ADRESS 0xB8000
									
/* 4000 octets de données texte */
#define GEM_VIDEO_MEMORY_SCREEN_SIZE 0xFA0 

/* Adresse limite où l'intervalle des données texte prennent fin */
#define GEM_VIDEO_MEMORY_LIMIT 0xB8FA0

// Taille en hauteur et largeur du buffer textuel
#define GEM_TEXT_VIDEO_MEMORY_WIDTH 25
#define GEM_TEXT_VIDEO_MEMORY_HEIGHT 80

/***** Singletons *****/

#define GEM_VIDEO_MEMORY VideoMemory::GetSingletonPtr()
#define GEM_IO_PORTS IOPorts::GetSingletonPtr()
#define GEM_SYSTEM System::GetSingletonPtr()
#define GEM_ISR_MANAGER ISRManager::GetSingletonPtr()
#define GEM_CLOCK Clock::GetSingletonPtr()
#define GEM_SHELL_MANAGER ShellManager::GetSingletonPtr()
#define GEM_MEMORY_MANAGER MemoryManager::GetSingletonPtr()

/***** Interrupts *****/

/* Nombre d'entrées dans la table "Interrupt Descriptor Table" */
#define GEM_IDT_ENTRIES 256

/***** ELF *****/

#define GEM_ELF_HEADER_START_MAGIC_NUMBER 0x7f
#define GEM_ELF_IDENT_NUMBER 6
#define GEM_ELF_HEADER_START_MAGIC_NUMBER_ELF_1 'E'
#define GEM_ELF_HEADER_START_MAGIC_NUMBER_ELF_2 'L'
#define GEM_ELF_HEADER_START_MAGIC_NUMBER_ELF_3 'F'


/***** Système mémoire de pages *****/

/*

 x << SOS_PAGE_SIZE_SHIFT est équivalent à
 x * SOS_PAGE_SIZE

 --------------

 x >> SOS_PAGE_SIZE_SHIFT est équivalent à
 x / SOS_PAGE_SIZE (arrondi entier inférieur)

 --------------

 x & SOS_PAGE_SIZE_MASK est équivalent à
 x modulo SOS_PAGE_SIZE

*/
#define	GEM_PAGE_SIZE 4 * 1024
#define GEM_PAGE_SHIFT 12
#define GEM_KERNEL_HEAP_LIMIT

/**** CPUID ****/
#define	GEM_MAX_EXT_VALUE_EAX 0x80000000

/**** Définitions de Port ****/
#define GEM_REBOOT_VALUE 0xFE
#define GEM_REAL_TIME_CLOCK_OUT_PORT 0x70
#define GEM_REAL_TIME_CLOCK_IN_PORT 0x71

/**** Configuration de la GDT ****/

// Pour le segment de code
#define GEM_CODE_BASE 0
#define GEM_CODE_LIMIT 0xFFFFFFFF
#define GEM_CODE_GRANULARIY 0xCF
#define GEM_CODE_ACCESS 0x9A

// Pour le segment des données
#define GEM_DATA_BASE 0
#define GEM_DATA_LIMIT 0xFFFFFFFF
#define GEM_DATA_GRANULARIY 0xCF
#define GEM_DATA_ACCESS 0x92

// Pour le segment de la pile
#define GEM_STACK_BASE 0
#define GEM_STACK_LIMIT 0xFFFFFFFF
#define GEM_STACK_GRANULARIY 0xCF
#define GEM_STACK_ACCESS 0x92


/**** ISR *****/
#define GEM_IDT_ENTRIES_COUNT 256

/**** ASCII simple ****/

#define GEM_KEYBOARD_ASCII_SPACE ' '
#define GEM_KEYBOARD_ASCII_TAB 0x09
#define GEM_KEYBOARD_ASCII_CARRIAGE_RETURN '\r'
#define GEM_KEYBOARD_ASCII_NEW_LINE '\n'
#define GEM_KEYBOARD_ASCII_BACKSPACE 0x08

/**** Entrées clavier ****/


/**** Shell ****/

#define GEM_SHELL_PROMPT_TEXT L"[Bash]"
#define GEM_SHELL_INPUT_LEGAL_CHARS L"$&éèçà^ABCDEFGHIJKLMNOPQRSTUVWYXUZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[none]\\*-_.:,;ù' "
#define GEM_SHELL_MAX_COMMAND_HISTORY 25


/**** IDE Ports ****/


/**** Divers ****/

#define GEM_TTYS_COUNT 6
#define GEM_ERROR -1
#define GEM_OK 0

/***** Macros *****/

#define DEBUG_BREAK_IF(p) if (p) { __asm { int 3 } }

#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }

#define SAFE_DELETE_ARRAY if (p) { delete[] (p); (p) = nullptr; }



/* Annonce au compilateur de ne pas altérer la structure 
   déclarée */
#define GEM_PACK __attribute__ ((packed))

#define debugprint(msg) std::wcout << #msg " = " << msg << std::endl;

#ifndef NDEBUG
 
#define AssertMsg(expr, msg) if (!(expr)) { std::wout << (#expr << " " << __FILE__ << " " << __LINE__ \
<< " " << __FUNCTION__ << " " << msg << std::endl); __asm { int 3 } }

// Ajouter variadics arguments
#define AssertNullPointer(pointer) if (pointer == nullptr) { AssertMsg(pointer, #pointer " == nullptr !!"); \
	__asm { int 3 } }
                     
#else
 
#define AssertMsg(expr, msg)

#endif

#define GEM_UNKNOWN L"Unknown"
#define GEM_EMPTY L"Empty"    

#define BIT(x) 1 << x
