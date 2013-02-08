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

#ifndef _UMLNCRELATION_H
#define _UMLNCRELATION_H


#include "UmlBaseNcRelation.h"
//Added by qt3to4:
#include "misc/mystr.h"
#include <QTextStream>

//This class manages 'relations' between non class objects
//
// You can modify it as you want (except the constructor)
class UmlNcRelation : public UmlBaseNcRelation
{
public:
    UmlNcRelation(void * id, const WrapperStr & n) : UmlBaseNcRelation(id, n) {};

    virtual void generate_import(QTextStream & f, const WrapperStr & indent);
};

#endif
