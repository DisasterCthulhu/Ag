#include <item.h>
#include <daemon.h>
#include <Ag.h>

inherit "/std/item";
inherit Ag_Module("utils");
inherit Ag_Module("cmd_init");
inherit Ag_Module("cmd_cat_file");
inherit Ag_Module("cmd_hash_object");

mixed ag_can_move(mapping args) {
    unless(is_developer(args["to"]))
        return ({ Move_Inappropriate });
    return True;
}

void ag_do_move(mapping args) {
	object who = args["to"];
	if(who && is_developer(who)) {
		add_action("ag_cmd", "ag");
	}
}
status ag_cmd_hash_object(string array args) {
    return ag_mod_cmd_hash_object(args);
}

status ag_cmd_cat_file(string array args) {
    return ag_mod_cmd_cat_file(args);
}

status ag_cmd_init(string array args) {
    return ag_mod_cmd_init(args);
}

status ag_cmd_help(string array args) {
    object who = this_player();
    who->display("usage: ag --help <command> [<args>]\n"
            "Ag commands that are implemented\n"
            "\t<subcmd> help  Usage information for the related subcommand\n"
            "\tinit [path]    Create an empty Ag Repository or reinit an existing one\n"
            "\tcat-file       Provide content information for repository objects\n"
            "\thash-object    Compute object ID and optional creates a blob from a file\n");
    return True;
}

status ag_cmd(string args) {
    object who = this_player();
    unless(stringp(args)) {
        return ag_cmd_help(({}));
    }
    string array parts = explode(args, " ");
    string cmd = parts[0];
    parts = parts[1..<1];
    switch(cmd) {
        case "init":
            return ag_cmd_init(parts);
            break;
        case "hash-object":
            return ag_cmd_hash_object(parts);
            break;
        case "cat-file":
            return ag_cmd_cat_file(parts);
            break;
        case "--help": // fallthrough to help
        case "-h":
        case "help":
        default:
            return ag_cmd_help(parts);
    }
    return False;
}

void configure() {
    ::configure();
    set_creator("elronuan");
    set_names(({ "ag" }));
    set_indestructible(True);
    add_description("A tool to work with Ag.");
    set_item_info("ag <subcommand> [<args...>]'");
    set_shape(([
        Shape_Type   : Geometry_Cylinder,
        Shape_Radius : 0.05,
        Shape_Length : 3.00,
    ]));
    add_proportion(({ Material_Melanaster, 1.0 }));
    add_hook(Can_Move, #'ag_can_move);
    add_hook(Do_Move, #'ag_do_move);
}

