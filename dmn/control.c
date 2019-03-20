#include <Ag.h>
#include <daemon.h>

inherit "/std/daemon";
inherit "/mod/daemon/control";

void configure() {
    ::configure();
    set_creator("elronuan");
}
