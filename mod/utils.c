#include <Ag.h>
#include <files.h>

string query_short() {
    return object_name(this_object());
}

// Largely file utilities, and functions that call with repo as the first arg
status is_dir(string path) {
    // FSIZE_DIR (-2) indicates that it is a directory.
    return file_size(path) == FSIZE_DIR;
}

status path_exists(string path) {
    // FSIZE_NOFILE (-1) indicates that the file either does not exist,
    //   or that it is not readable for the calling object/user.
    return file_size(path) != FSIZE_NOFILE;
}

string path_join(string path, mixed paths) {
    if(path == Ag_PathSep && stringp(paths)) {
        path = ""; // implode with an empty string as the first element starts it with a /
    }
    if(stringp(paths)) {
        return implode(({path, paths}), Ag_PathSep);
    } else if(arrayp(paths)) {
        return implode(({ path, implode(paths, Ag_PathSep) }), Ag_PathSep);
    }
}

// recurisve mkdir
status mkdirs(string path) {
    string array parts = explode(path, Ag_PathSep);
    string working = "";
    status failures = False;
    foreach(string part : parts) {
        working = path_join(working, part);
        if(is_dir(working)) {
            continue;
        }
        failures ||= !mkdir(working);
    }
    return !failures;
}

// chomp trailing directory
string path_parent(string path) {
    return implode(explode(path, Ag_PathSep)[0..<2], Ag_PathSep);
}


// Repo Functions
// Ag_Dir relative path
string repo_path(object repo, mixed path) {
    return path_join(repo->query_agdir(), path);
}

// Ag_Dir relative path, return if it exists or is creatable (when requested), errors if path is a file or unable to be created
string repo_dir(object repo, mixed path, status make_dir) {
    path = repo_path(repo, path);
    if(path_exists(path)) {
        if(is_dir(path)) {
            return path;
        } else {
            error("Not a directory " + printable(path));
        }
    }
    if(make_dir) {
        unless(mkdirs(path)) {
            error("Failed to recursively create path " + printable(path));
        }
        return path;
    }
    return "";
}

// Same as the above, except for a file, errors if the parent directory does not exist or cannot be created.
// does not verify access to the target file
string repo_file(object repo, string path, status make_dir) {
    string parent = path_parent(path);
    if(repo_dir(repo, parent, make_dir)) {
        return repo_path(repo, path);
    }
    return "";
}

