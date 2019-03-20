#include <Ag.h>
#include <daemon.h>

inherit "/std/daemon";
inherit "/mod/daemon/control";
inherit Ag_Module("utils");

mapping repos;
mapping objects;

void configure() {
    ::configure();
    set_creator("elronuan");
    // repo / object tracking to hopefully not repeatedly generate the same objects when in use
    // objects may need to be placed under repo paths if there are collisions
    repos = ([]);
    objects = ([]);
}

object ag_repo_for_path(string path) {
    if(member(repos, path)) {
        return repos[path];
    }
    return repos[path] = new(Ag_Repository)->setup(path);
}

object ag_repo_find(object who, string path, status required) {
    // Path resolution appears to be garbage, here's some slight runaway safety
    if (strlen(path) > 80 || caller_stack_depth() > 10) {
        remove();
    }
    who ||= this_player();
    path = who->resolve_path(path);
    if (is_dir(path_join(path, Ag_Dir))) {
        return ag_repo_for_path(path);
    }
    string parent = path_parent(path);
    if (parent == Ag_PathSep || parent == "") { // Chomping the /path off ends up with ""
        // parent == path _should_ work, but ls has /.git and there's priv issues.  so do not look at /.git
        // The above is probably not an issue now with Ag_Dir
        if(required) { // consider error()
            who->display("No " + Ag_Dir + " directory found.");
            return 0;
        } else {
            return 0;
        }
    }
    return ag_repo_find(who, parent, required);
}

object ag_restore_object(object repo, string format, string data) {
    return new(Ag_Object(format))->setup(repo, data);
}

mixed ag_object_read(object repo, string hash) {
    if(member(objects, hash) && objects[hash]) {
        return objects[hash];
    }
    string path = repo_file(repo, path_join("objects", ({ hash[0..1], hash[2..<1] })), False);
    // File should be zlib inflated before looking for the header
    string raw = Ag_Inflate(read_file(path)); // Null terminated header... this hack works for uncompressed objects. still search for the header / payload separator
    int spc = strstr(raw, " ");
    string format = raw[0..spc-1];
    int sep = strstr(raw, Ag_Object_Header_Sep, spc+1); // NUL Separator, or end of string (as lpc strings work)
    int len = to_int(raw[spc+1..sep-1]);
    string data = raw[sep+1..<1];
    if (strlen(data) != len) {
        return "Malformed object " + printable(hash) + ": bad length", printable(({ len, strlen(data) }));
    }
    return objects[hash] = ag_restore_object(repo, format, data);
}

string object_write(object obj, status actually_write) {
    string data = obj->serialize(); // Data cannot contain nulls.
    string repr = obj->query_format() +
        " " + // type / length separator
        to_string(strlen(data)) +
        Ag_Object_Header_Sep + // header / payload separator
        data;
    string hash = Ag_Hash(repr); // hash is actually md5...
    objects[hash] = obj;
    if (actually_write) {
        string path = repo_file(obj->query_repo(),  path_join("objects", ({ hash[0..1], hash[2..<1] })), True);
        write_file(path, Ag_Deflate(repr));
    }
    return hash;
}

//TODO: Extend
string ag_object_find(object repo, string name, string format, status follow) {
    return name;
}
