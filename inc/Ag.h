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

#define Ag_Lib                             (Ag_Module("Lib"))
#define Ag_Lib_Interface                   (Ag_Module("Lib_interface"))
#define Ag_Object_Base                     (Ag_Misc("Object"))
#define Ag_Object(x)                       (Ag_Misc("object_" + (x)))    // object_blob, commit, tree, tag.

#define Ag_Repo_Find(x, y, z)              (Ag_Daemon("control")->ag_repo_find(x, y, z)) // who, path, required => repo
#define Ag_Repository                      (Ag_Misc("repository"))

#define Ag_Object_Find(w, x, y, z)         (Ag_Daemon("control")->ag_object_find(w, x, y, z)) // repo, name, format, follow => hash
#define Ag_Object_Read(x, y)               (Ag_Daemon("control")->ag_object_read(x, y))  // repo, hash => object
#define Ag_Object_Write(x, y)              (Ag_Daemon("control")->ag_object_write(x, y)) // object, actually_write => hash

// Various options that if changed would break compatibility, but should be adjustable.
// Individual definitions should have comments as to what the value ought to be with proper support
#define Ag_Hash(x)                         (md5(x))  // Sha1, or possibly sha256
#define Ag_Inflate(x)                      (x)       // ZLib Inflate (used when reading objects)
#define Ag_Deflate(x)                      (x)       // ZLib Deflate (when writing objects). The hash is generated before deflate
#define Ag_Object_Commit_Key_Message       ("")      // special case as part of a commit object, holds the commit message after key-value pairs
#define Ag_PathSep                         ("/")     // / is unixy
#define Ag_Dir                             (".ag")   // Like .git or something
#define Ag_Object_Header_Sep               ("X")     // Typically '\0x00', but nulls are very problematic in LPC, this follows [0-9]+ and is a space filler
                                                     // using "X" since we can go back to string handling

#endif
