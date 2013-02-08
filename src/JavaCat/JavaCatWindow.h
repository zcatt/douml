//Added by qt3to4:
#include "misc/mystr.h"
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

#ifndef JAVACATWINDOW_H
#define JAVACATWINDOW_H

#ifdef REVERSE
#include "UmlCom.h"

#define JavaCatWindow UmlCom

#else

#include <q3mainwindow.h>

class QSplitter;
class BrowserView;
class BrowserNode;
class CommentView;

// singleton

class JavaCatWindow : public Q3MainWindow
{
    Q_OBJECT

public:
    JavaCatWindow();

    static void trace(QString);
    static void trace(WrapperStr);
    static void clear_trace();

protected:
    static JavaCatWindow * the;

    QSplitter * spl;
    BrowserView * browser;
    CommentView * comment;
    BrowserNode * commented;    // the commented object

private slots:
    void load();
    void save();
    void quit();
    void scan();
    void browser_search();
    void historic_back();
    void historic_forward();

    void motif_style();
    void motifplus_style();
    void windows_style();

    void about();
    void aboutQt();
};

#endif

#endif
