#include <Ag.h>
#include <display.h>

inherit Ag_Module("utils");

string ag_cmd_hash_object_help() {
    return "ag hash-object [-w] [-t TYPE] FILE\n\ttype can be one of: blob*, tree, commit, tag. *default";
}

string object_hash(string data, string format, object repo) {
    object obj = new(Ag_Object(format))->setup(repo, data);
    string hash = Ag_Object_Write(obj, repo);
    //TODO: dest obj, don't cache on far side?
    return hash;
}

// passing a repo object will persist the object to a file
string ag_hash_object(object repo, string path, string format) {
    return object_hash(read_file(path), format, repo);
}

status ag_mod_cmd_hash_object(string array args) {
    object who = this_player();
    unless(sizeof(args)) {
        who->display(ag_cmd_hash_object_help());
        return True;
    }
    status write = (member(args, "-w") != Null);
    int type_idx = member(args, "-t");
    string format = "blob";
    if(type_idx > Null) {
        format = args[type_idx + 1];
        unless(Ag_Object_Valid_Type(format)) {
            who->display("Invalid object type " + printable(format) + "\n" +
                        ag_cmd_hash_object_help());
            return True;
        }
    }
    string path = who->resolve_path(args[<1]);
    unless(path_exists(path)) {
        who->display("Unable to locate file: " + printable(path) + "\n" + ag_cmd_hash_object_help());
        return 1;
    }
    object repo = (write ? Ag_Repo_Find(who, ".", True) : 0);
    who->display(ag_hash_object(repo, path, format));
    return True;
}

