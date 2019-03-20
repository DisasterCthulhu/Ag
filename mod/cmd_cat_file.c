#include <Ag.h>
#include <display.h>

inherit Ag_Module("utils");

string ag_cmd_cat_file_help() {
    return "ag cat-file <type> <object>\n\ttype can be one of: blob, tree, commit, tag";
}

string ag_cat_file(object repo, string obj_name, string format) {
    object obj = Ag_Object_Read(repo, Ag_Object_Find(repo, obj_name, format, True));
    return obj->serialize();
}

status ag_mod_cmd_cat_file(string array args) {
    object who = this_player();
    unless(sizeof(args) == 2) {
        who->display(ag_cmd_cat_file_help());
        return True;
    }
    unless(Ag_Object_Valid_Type(args[0])) {
        who->display(ag_cmd_cat_file_help());
        return True;
    }
    object repo = Ag_Repo_Find(who, ".", True);
    who->display(ag_cat_file(repo, args[1], args[0]), Display_Preformatted);
    return True;
}

