#include <Ag.h>
inherit Ag_Module("utils");

string agdir;
string worktree;
object conf;

string query_agdir() {
    return agdir;
}

void set_agdir(string path) {
    agdir = path_join(path, Ag_Dir);
}

string query_worktree() {
    return worktree;
}

void set_worktree(string path) {
    worktree = path;
}

object setup(string path, status force) {
    set_worktree(path);
    set_agdir(path);
    unless(force || is_dir(agdir)) {
        error("Not an Ag Reposity");
    }
    conf = this_object(); //TODO: Config Parser
    string cf = repo_file(this_object(), "config");
    if (cf && path_exists(cf)) {
        // conf->parse(cf);
    } else unless(force) {
        notify_fail("Configuration file is missing");
        return 0;
    }
    unless(force) {
        int vers = conf->get("core", "repositoryformatversion");
        if (vers != 0) {
            notify_fail("Unsupported repositoryformatversion " + printable(vers));
            return 0;
        }
    }
    return this_object();
}


