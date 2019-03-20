#include <Ag.h>
inherit Ag_Object_Base;

object setup(object repo, mixed data) {
    ::setup(repo, data);
    set_format("commit");
    return this_object();
}

string array ag_object_commit_key_order() {
    // "" for commit message
    return ({ "tree", "parent", "author", "commiter", "gpgsig", Ag_Object_Commit_Key_Message });
}

string serialize() {
    return kvlm_serialize(query_data(), ag_object_commit_key_order());
}

void deserialize(string blobdata) {
    set_data(kvlm_parse(blobdata));
}

