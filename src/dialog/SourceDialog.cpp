// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
// Copyright 2012-2013 Nikolai Marchenko.
// Copyright 2012-2013 Leonardo Guilherme.
//
// This file is part of the DOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License Version 3.0 as published by
// the Free Software Foundation and appearing in the file LICENSE.GPL included in the
//  packaging of this file.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License Version 3.0 for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : doumleditor@gmail.com
// home   : http://sourceforge.net/projects/douml
//
// *************************************************************************





#include <stdio.h>

#include <qlayout.h>
#include <qpushbutton.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "SourceDialog.h"
#include "UmlDesktop.h"
#include "strutil.h"
#include "translate.h"

bool NumberedMultiLineEdit::event(QEvent * e)
{
    static int old_l = 0;
    static int old_c = 0;

    bool r = MultiLineEdit::event(e);
    int l;
    int c;
    l = textCursor().blockNumber();
    c = textCursor().columnNumber();

    if ((c != old_c) || (l != old_l)) {
        old_c = c;
        old_l = l;
        emit cursorMoved(l, c);
    }

    return r;
}

QSize SourceDialog::previous_size;

SourceDialog::SourceDialog(QString p, BooL & flg, unsigned & edn)
    : QDialog(0),
      path(p), edited(flg), edition_number(edn)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QFileInfo fi(p);

    setWindowTitle(fi.fileName());

    QVBoxLayout * vbox = new QVBoxLayout(this);

    e = new NumberedMultiLineEdit(this);
    QFont font = e->font();

    if (! hasCodec())
        font.setFamily("Courier");

    font.setFixedPitch(TRUE);
    e->setFont(font);
    vbox->addWidget(e);

    QHBoxLayout * hbox = new QHBoxLayout();
    vbox->addLayout(hbox);
    hbox->setMargin(5);

    lineColumn = new QLabel(this);
    lineColumn->setFont(font);
    hbox->addWidget(lineColumn);
    connect(e, SIGNAL(cursorMoved(int, int)),
            this, SLOT(updateCursorPos(int, int)));
    updateCursorPos(0, 0);

    QFile f(p);
    unsigned size = fi.size();

    if (f.open(QIODevice::ReadOnly)) {
        char * s = new char[size + 1];

        if (f.read(s, size) != -1) {
            edited = TRUE;
            edition_number += 1;

            s[size] = 0;
            e->setText(s);

#if 0

            if (fi.isWritable()) {
                QPushButton * edit = new QPushButton(TR("&Edit"), this);
                QPushButton * roundtrip = new QPushButton(TR("&Roundtrip"), this);

                hbox->addWidget(new QLabel(this));
                hbox->addWidget(edit);
                connect(edit, SIGNAL(clicked()), this, SLOT(edit()));

                hbox->addWidget(new QLabel(this));
                hbox->addWidget(roundtrip);
                connect(roundtrip, SIGNAL(clicked()), this, SLOT(accept()));
            }

#endif
        }
        else
            e->setText(TR("cannot read %1"/*, p*/));

        delete [] s;
        f.close();
    }
    else
        e->setText(TR("cannot read %1"/*, p*/));

#if 0
    QPushButton * cancel = new QPushButton(TR("&Cancel"), this);

    hbox->addWidget(new QLabel(this));
    hbox->addWidget(cancel);
    hbox->addWidget(new QLabel(this));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
#endif

    // not done in polish else the initial size is too small
    UmlDesktop::setsize_center(this, previous_size, 0.5, 0.5);

    open_dialog(this);
}

SourceDialog::~SourceDialog()
{
    edited = FALSE;
    edition_number -= 1;
    previous_size = size();

    close_dialog(this);
}

void SourceDialog::updateCursorPos(int l, int c)
{
    QString s;

    s.sprintf("line %-7d column %-6d", l + 1, c + 1);
    lineColumn->setText(s);
}

void SourceDialog::edit()
{
    e->setEnabled(FALSE);
    //..........
}
