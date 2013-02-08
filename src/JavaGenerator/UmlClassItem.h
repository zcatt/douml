// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
// Copyright 2012-2013 Nikolai Marchenko.
// Copyright 2012-2013 Leonardo Guilherme.
//
// This file is part of the DOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : enmarantispam@gmail.com
// home   : http://sourceforge.net/projects/douml
//
// *************************************************************************

#ifndef UMLCLASSITEM_H
#define UMLCLASSITEM_H

#include <q3ptrlist.h>
//Added by qt3to4:
#include "misc/mystr.h"
#include <QTextStream>

#include "UmlBaseClassItem.h"

class QTextStream;
class UmlTypeSpec;

class UmlClassItem : public UmlBaseClassItem
{
public:
    UmlClassItem(void * id, const WrapperStr & n)
        : UmlBaseClassItem(id, n) {
    };

    virtual void generate(QTextStream & f, const WrapperStr & cl_stereotype,
                          WrapperStr indent) = 0;
    virtual void generate_enum_pattern_item(QTextStream & f, int & current_value,
                                            const WrapperStr & class_name, WrapperStr indent) = 0;
    virtual void generate_enum_pattern_case(QTextStream & f, WrapperStr indent) = 0;
    virtual void generate_enum_item(QTextStream & f, WrapperStr indent, BooL & first);
    virtual void generate_enum_member(QTextStream & f, WrapperStr indent) = 0;
};

#endif
