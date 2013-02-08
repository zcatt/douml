
#include "UmlActivityItem.h"
#include "FileOut.h"
#include "UmlActivity.h"

#include "UmlFlow.h"
//Added by qt3to4:
#include "misc/mystr.h"
void UmlActivityItem::write_selection(FileOut & out, WrapperStr s)
{
    if (! s.isEmpty())
        out.ref(activity()->add_opaque_behavior(s, dynamic_cast<UmlItem *>(this),
                                                "OPAQUE_SELECTION_BEHAVIOR_"),
                "selection", "OPAQUE_SELECTION_BEHAVIOR_");
}

UmlActivity * UmlActivityItem::activity()
{
    UmlItem * r = dynamic_cast<UmlItem *>(this);

    do {
        r = r->parent();
    }
    while (r->kind() != anActivity);

    return (UmlActivity *) r;
}

