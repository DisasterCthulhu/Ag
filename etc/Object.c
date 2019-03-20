#include <Ag.h>
inherit Ag_Module("utils");

object repo;
string format;
mixed data;

object query_repo() {
    return repo;
}

void set_repo(object obj) {
    repo = obj;
}

mixed query_data() {
    return data;
}

void set_data(mixed contents) {
    data = contents;
}

void set_format(string fmt) {
    format = fmt;
}

string query_format() {
    return format;
}

// To be implemented by subclasses
string serialize() {
    error("unimplemented");
}

void deserialize(string data) {
    error("unimplemented");
}

object setup(object repo, mixed data) {
    set_repo(repo);
    if (data) {
        deserialize(data);
    }
    return this_object();
}

// Parser/Serializer for key-value with message (eg: commit objects)
// LPC Lacks orderd mappings, so the array of keys is used to order when serializing
string kvlm_serialize(mapping m, string array keys) {
    string ret = "";
    foreach(string k : keys) {
        if(k == Ag_Object_Commit_Key_Message) {
            continue;
        }
        mixed val = m[k];
        unless(val) {
            continue;
        }
        unless(arrayp(val)) {
            val = ({ val });
        }
        foreach(string v : val) {
            ret += k + " " + replace(v, "\n", "\n ") + "\n";
        }
    }
    if(member(m, Ag_Object_Commit_Key_Message)) {
        ret += "\n" + m[Ag_Object_Commit_Key_Message];
    }
    return ret;
}

mapping kvlm_parse(string raw, int start, mapping m) {
    m ||= ([]);
    int spc = strstr(raw, " ", start);
    int nl = strstr(raw, "\n", start);
    if((spc < 0) || (nl < spc)) {
        if(nl == start) {
            m[Ag_Object_Commit_Key_Message] = raw[start+1..<1];
            return m;
        } else {
            return m;
        }
    }
    string k = raw[start..spc-1];
    int end = start;
    while(True) {
        end = strstr(raw, "\n", end+1);
        unless(raw[end+1] == ' ') {
            break;
        }
    }
    string value = replace(raw[spc+1..end-1], "\n ", "\n");
    if(member(m, k)) {
        if(arrayp(m[k])) {
            m[k] += ({ value });
        } else {
            m[k] = ({ m[k], value });
        }
    } else { 
        m[k] = value;
    }
    return kvlm_parse(raw, end+1, m);
}

