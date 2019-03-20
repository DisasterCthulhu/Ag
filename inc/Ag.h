#ifndef _Ag_Included
#define _Ag_Included

#include <Misc.h>

#define Ag                                 "/dev/Ag/" // DEPLOY: extract
// #define Ag                                 Misc "Ag/" // DEPLOY: uncomment

#define Ag_Armour(x)                       (Ag + "arm/" + x)
#define Ag_Comestible(x)                   (Ag + "f&d/" + x)
#define Ag_Daemon(x)                       (Ag + "dmn/" + x)
#define Ag_Data(x)                         (Ag + "dat/" + x)
#define Ag_Definition(x)                   (Ag + "def/" + x)
#define Ag_Extension(x)                    (Ag + "ext/" + x)
#define Ag_Help(x)                         (Ag + "hlp/" + x)
#define Ag_Include(x)                      (Ag + "inc/" + x)
#define Ag_Misc(x)                         (Ag + "etc/" + x)
#define Ag_Module(x)                       (Ag + "mod/" + x)
#define Ag_Monster(x)                      (Ag + "mon/" + x)
#define Ag_NPC(x)                          (Ag + "npc/" + x)
#define Ag_Room(x)                         (Ag + "rms/" + x)
#define Ag_Text(x)                         (Ag + "txt/" + x)
#define Ag_Weapon(x)                       (Ag + "wpn/" + x)

#endif
