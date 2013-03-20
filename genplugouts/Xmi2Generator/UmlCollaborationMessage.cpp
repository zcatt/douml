
#include "UmlCollaborationMessage.h"
#include "FileOut.h"
#include "UmlItem.h"
#include "../src/misc/mystr.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "UmlClassInstanceReference.h"
//Added by qt3to4:
#include "misc/mystr.h"
void UmlCollaborationMessage::write(FileOut & out, UmlItem * diagram, const Q3PtrVector< UmlCollaborationMessage > & msgs, unsigned & index)
{
    unsigned sup = msgs.size();
    UmlPackage * prj = UmlPackage::getProject();

    while (index != sup) {
        const UmlCollaborationMessage * msg = msgs[index++];
        WrapperStr pfix = msg->hrank() + ".";
        unsigned pfixlen = pfix.length();

#define MSG  "MSG", msg->itsrank
#define SEND "MSGOCCSPECSEND", msg->itsrank
#define REC  "MSGOCCSPECREC", msg->itsrank
#define BEH  "BEHEXECSPEC", msg->itsrank
#define EXEC "EXECOCCSPEC", msg->itsrank

        out.indent();
        out << "<fragment xmi:type=\"uml:MessageOccurrenceSpecification\"";
        out.id_prefix(diagram, SEND);
        out.ref(diagram, "covered", msg->from()->lifeline());
        out.ref(prj, "event",
                (msg->operation() != 0) ? msg->operation()->event(FALSE)
                : UmlOperation::event("SEND", msg->form()));
        out.ref(diagram, "message", MSG);
        out << "/>\n";

        out.indent();
        out << "<message xmi:type=\"uml:Message\"";
        out.id_prefix(diagram, MSG);
        out << " name=\"";
        out.quote((const char *)((msg->operation() != 0) ? msg->operation()->name()
                                 : msg->form()));//[jasa] ambiguous call
        out << '"';
        out.ref(diagram, "sendEvent", SEND);
        out.ref(diagram, "receiveEvent", REC);
        out.ref(diagram, "connector", msg->from()->connector(msg->to()));
        out << "/>\n";

        out.indent();
        out << "<fragment xmi:type=\"uml:MessageOccurrenceSpecification\"";
        out.id_prefix(diagram, REC);
        out.ref(diagram, "covered", msg->to()->lifeline());
        out.ref(prj, "event",
                (msg->operation() != 0) ? msg->operation()->event(TRUE)
                : UmlOperation::event("REC", msg->form()));
        out.ref(diagram, "message", MSG);
        out << "/>\n";

        out.indent();
        out << "<fragment xmi:type=\"uml:BehaviorExecutionSpecification\"";
        out.id_prefix(diagram, BEH);
        out.ref(diagram, "covered", msg->to()->lifeline());
        out.ref(diagram, "start", REC);
        out.ref(diagram, "finish", EXEC);
        out << "/>\n";

        if (index != sup) {
            WrapperStr pfix2 = msgs[index]->hrank() + ".";

            if ((pfix2.length() > pfixlen) && !strncmp(pfix, pfix2, pfixlen))
                write(out, diagram, msgs, index);
        }

        out.indent();
        out << "<fragment xmi:type=\"uml:ExecutionOccurrenceSpecification\"";
        out.id_prefix(diagram, EXEC);
        out.ref(diagram, "covered", msg->to()->lifeline());
        out.ref(prj, "event",
                UmlOperation::event("EXEC",
                                    (msg->operation() != 0) ? msg->operation()->name()
                                    : msg->form()));
        out.ref(diagram, "execution", BEH);
        out << "/>\n";
    }
}

