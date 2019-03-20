#include <Ag.h>
inherit Ag_Object_Base;

object setup(object repo, mixed data) {
    ::setup(repo, data);
    set_format("blob");
    return this_object();
}

string serialize() {
    return query_data();
}

void deserialize(string data) {
    set_data(data);
}

