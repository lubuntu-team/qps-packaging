/*
 * command.h
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 1997-1999 Mattias Engdegård
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef COMMAND_H
#define COMMAND_H

#ifndef USING_PCH
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QToolButton>
#include <QPushButton>
#include <QListView>
#endif

#include "proc.h"
#include "misc.h"

class ListModel;

class Command
{
  Q_DECLARE_TR_FUNCTIONS(Command)
  public:
    Command(){};
    Command(QString n, QString cmd, bool toolbar = false);
    ~Command();
    void call(Procinfo *p);
    bool IsNeedProc();
    QString getString();
    void putString(QString str);

    // CommandButton *toolbutton;

    QString name;    // appears in the menu
    QString cmdline; // may contain variables (%p etc)
    int menu;        // index in menu
    bool toolbar;
    bool popup;
};

#endif // COMMAND_H
