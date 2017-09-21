/*
 * details.h
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

#ifndef DETAILS_H
#define DETAILS_H

#ifndef USING_PCH
#include <QTabWidget>
#include <QFrame>
#include <QHash>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QLayout>
#endif

#include "proc.h"
#include "lookup.h"
#include "tablefield.h"

class Details : public QWidget
{
    Q_OBJECT
  public:
    Details(Procinfo *p, Proc *proc);
    ~Details();

    void refresh();
    void config_change();
    void process_gone();
    Procinfo *get_procinfo() { return pi; }
    Proc *proc() { return pr; }
    void set_procinfo(Procinfo *p);

signals:
    void closed(Details *);

  protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);

  private:
    QTabWidget *tbar;
    Procinfo *pi;
    Proc *pr;
};

// SimpleTable: a HeadedTable with fixed number of columns
#include "htable.h"
class SimpleTable : public HeadedTable
{
    Q_OBJECT
  public:
    SimpleTable(QWidget *parent, int nfields, TableField *f, int options = 0);
    QSize sizeHint() const;
    virtual void refresh(){};

  protected:
    virtual QString title(int col);
    virtual QString text(int row, int col) = 0;
    virtual int colWidth(int col);
    virtual int alignment(int col);
    virtual int leftGap(int col);
    virtual QString tipText(int col);
    //   Procinfo *procinfo() { return ((Details
    //   *)parentWidget())->procinfo(); }
    Procinfo *procinfo() { return detail->get_procinfo(); }
    Proc *proc() { return detail->proc(); }

  private:
    const TableField *fields;
    Details *detail;
};

class Sockets : public SimpleTable
{
    Q_OBJECT
  public:
    Sockets(QWidget *parent);
    ~Sockets();

    void refresh();
    void refresh_window();
    bool refresh_sockets();
    const char *servname(unsigned port);
    QString ipAddr(unsigned addr);
    QString hostname(unsigned addr);
    void config_change();

  public slots:
    void update_hostname(unsigned addr);

  protected:
    virtual QString text(int row, int col);

  private:
    enum
    {
        FD,
        PROTO,
        RECVQ,
        SENDQ,
        LOCALADDR,
        LOCALPORT,
        REMOTEADDR,
        REMOTEPORT,
        STATE,
        SOCKFIELDS
    };
    static TableField fields[SOCKFIELDS];

    bool doing_lookup; // if table painted with host lookup

    static Lookup *lookup;
    static bool have_services; // true if we have tried reading services
    static QHash<int, char *> servdict;
};

class Maps : public SimpleTable
{
  public:
    Maps(QWidget *parent);
    ~Maps();

    void refresh();
    void refresh_window();
    bool refresh_maps();

  protected:
    virtual QString text(int row, int col);

  private:
    enum
    {
        ADDRESS,
        SIZE,
        PERM,
        OFFSET,
        DEVICE,
        INODE,
        FILENAME,
        MAPSFIELDS
    };
    static TableField fields[MAPSFIELDS];
};

class Files : public SimpleTable
{
  public:
    Files(QWidget *parent);
    ~Files();

    void refresh();
    void refresh_window();
    bool refresh_fds();

  protected:
    virtual QString text(int row, int col);

  private:
    enum
    {
        FILEDESC,
#ifdef LINUX
        FILEMODE,
#endif
        FILENAME,
        FILEFIELDS
    };
    static TableField fields[FILEFIELDS];
};

class Environ : public SimpleTable
{
    Q_OBJECT
  public:
    Environ(QWidget *parent);
    ~Environ();

    void refresh();
    void refresh_window();

  public slots:
    void sort_change(int);

  protected:
    virtual QString text(int row, int col);

    void sort();
    static int compare(const NameValue *a, const NameValue *b);

  private:
    enum
    {
        ENVNAME,
        ENVVALUE,
        ENVFIELDS
    };
    bool rev;                   // sorting reversed
    static Environ *static_env; // for sorting, must have static pointer
    static TableField fields[ENVFIELDS];
};

class AllFields : public SimpleTable
{
  public:
    AllFields(QWidget *parent);
    ~AllFields();

    void refresh();
    void refresh_window();

  protected:
    virtual QString text(int row, int col);

  private:
    enum
    {
        FIELDNAME,
        FIELDDESC,
        FIELDVALUE,
        FIELDSFIELDS
    };
    static TableField fields[FIELDSFIELDS];
};

#endif // DETAILS_H
