#ifndef _UMLONSIGNALACTION_H
#define _UMLONSIGNALACTION_H


#include "UmlBaseOnSignalAction.h"


class FileIn;
class Token;

class UmlOnSignalAction : public UmlBaseOnSignalAction
{
protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
    UmlOnSignalAction(void * id, const WrapperStr & s) : UmlBaseOnSignalAction(id, s) {
    }


public:
    //  call at end of import : try to solve signal

    virtual void solve(WrapperStr idref);


protected:
    //import the activity signal action starting by 'tk'

    virtual void import_it(FileIn & in, Token & token);

};

#endif
