/*
 * pstable2.h
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 2014 dae hyun, yang <daehyun.yang@gmail.com>
 * Copyright 2015 Paulo Lieuthier <paulolieuthier@gmail.com>
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

#include "proc.h"
#include "htable2.h"
#include <QToolTip>

class PstableModel : public HtableModel
{
    Q_OBJECT
  public:
    PstableModel(QObject *parent, Procview *procview);
    ~PstableModel(){};
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const; // pure virtual
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    // int columnCount(const QModelIndex &parent) const;
    //	HeadedTable2 *htable;
    Procview *procview;
};

class Pstable2 : public HeadedTable2
{
    Q_OBJECT
  public:
    Pstable2(QWidget *parent, Procview *pv);

    void set_sortcol();
    void setProcview(Procview *pv);
    void setTreeMode(bool treemode);
    virtual void moveCol(int col, int place);
    void refresh();

    // called by super
    bool hasSelection() { return selectionModel()->hasSelection(); };

    virtual bool isSelected(int row); // hmm
    virtual void setSelected(int row, bool sel);
    virtual int totalRow(); // DEL?

  public slots:
    void selection_update(const QModelIndex &); // hmm
    void setSortColumn(int col);

signals:
    void selection_changed();

  protected:
    // implementation of the interface to HeadedTable
    virtual QString title(int col);         // ok
    virtual QString text(int row, int col); // ok
    virtual int colWidth(int col);
    virtual int alignment(int col);   // ok
    virtual QString tipText(int col); // ok

    virtual int rowDepth(int row);
    virtual NodeState folded(int row);
    virtual int parentRow(int row);
    virtual bool lastChild(int row);
    virtual char *total_selectedRow(int col);
    virtual int sizeHintForColumn(int col) const;
    virtual bool columnMovable(int col);

    // virtual void drawCellContents(int row, int col, int w, int h,
    // QPainter *p);

  private:
    Procview *procview;
};
