#include <Ag.h>

inherit Ag_Module("utils");

string repo_default_config() {
    //TODO: Use a config parser/writer
    return "[core]\n" +
        "\trepositoryformatversion = 0\n" + 
        "\tfilemode = true\n" +
        "\tbare = false\n";
}

string repo_create(string path) {
    object repo = new(Ag_Repository);
    repo->setup(path, True);
    string res = "";

    if (path_exists(repo->query_worktree())) {
        if (!is_dir(repo->query_worktree())) {
            res = printable(repo->query_worktree()) + " is not a directory";
            safe_destruct(repo);
            return res;
        } else if (sizeof(get_dir(repo->query_worktree() + Ag_PathSep))) {
            res = printable(repo->query_worktree()) + " is not empty";
            safe_destruct(repo);
            return res;
        }
    } else {
        mkdir(repo->query_worktree());
    }
    repo_dir(repo, "branches", True);
    repo_dir(repo, "objects", True);
    repo_dir(repo, "refs", True);
    repo_dir(repo, path_join("refs", "tags"), True);
    repo_dir(repo, path_join("refs", "heads"), True);
    write_file(repo_file(repo, "description"),
        "Unnamed repository; edit this file 'description' to name the repository.\n");
    write_file(repo_file(repo, "HEAD"),
        "ref: refs/heads/master\n");
    write_file(repo_file(repo, "config"),
        repo_default_config());
    res = "repo setup for worktree: " + printable(repo->query_worktree()) +
          " at " + printable(repo->query_agdir());
    return res;
}

string ag_cmd_init_help() {
    return "ag init [path]\tpath resolved through who->resolve_path(arg)\nCreate an empty Ag Repository or reinit an existing one";
}

status ag_mod_cmd_init(string array args) {
    object who = this_player();
    string path = who->query_path();
    if(sizeof(args)) {
        if(member(({"help", "--help", "-h"}), args[0]) != Null) {
            who->display(ag_cmd_init_help());
            return True;
        }
        path = who->resolve_path(args[0]);
    }
    string res = repo_create(path);
    if(res) {
        who->display(res);
    }
    return True;
}

