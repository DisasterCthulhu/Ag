#include <Ag.h>
inherit Ag_Object_Base;

#define Ag_TreeLeaf_Mode (0)
#define Ag_TreeLeaf_Path (1)
#define Ag_TreeLeaf_Hash (2)
#define Ag_TreeLeaf_Size (3)

record TreeLeaf(string mode, string path, string hash) {
    record leaf = allocate(Ag_TreeLeaf_Size);
    leaf[Ag_TreeLeaf_Mode] = mode;
    leaf[Ag_TreeLeaf_Path] = path;
    leaf[Ag_TreeLeaf_Hash] = hash;
    return leaf;
}

// The normal format for this is <file-mode, up to 6 bytes>(space)<path (file or dir name)>\0x00<sha.bytes(not-hex)>
// we can't use nulls, so the modified format here is:
// <object-type>(space)<path>/<hash.hexdigest>
// object-type: we don't have file modes to identify permissions, but blob/tree are sufficient.
// path, the only non-permitted characters are \0x00, and '/', hence the trailing sep
// hexdigest up to a newline (or end of file?)
record tree_parse_leaf(string line) {
    int spc = strstr(line, " ");
    int sep = strstr(line, "/", spc+1);
    return TreeLeaf(line[0..spc-1], line[spc+1..sep-1], line[sep+1..<1]);
}

record array tree_parse(string raw) {
    return map(explode(raw, "\n"), #'tree_parse_leaf);
}

object setup(object repo, mixed data) {
    ::setup(repo, data);
    set_format("tree");
    return this_object();
}

string serialize() {
    return implode(map(query_data(), (:
        //TODO: rework parsing to not allocate the extra leaf for this
        all($1, (: $1 == "" :)) ? "" : // serialize trailing newlines when present
            $1[Ag_TreeLeaf_Mode] + " " + $1[Ag_TreeLeaf_Path] + "/" + $1[Ag_TreeLeaf_Hash]
    :)), "\n");
}

void deserialize(string blobdata) {
    set_data(tree_parse(blobdata));
}
