// watchdialog.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdeg?rd, 1997-1999

#include "watchdogdialog.h"

#include "listmodel.h"
#include "checkboxdelegate.h"
#include "misc.h"   // TBloon class
#include "qps.h"
#include "watchcond.h"

#include <QShowEvent>

extern QList<watchCond *> watchlist;
extern Qps *qps;

WatchdogDialog::WatchdogDialog()
{
    setupUi(this);
    listmodel = new ListModel();

    tableView->setModel(listmodel);
    checkBoxDelegate delegate;
    tableView->setEditTriggers(QAbstractItemView::SelectedClicked);
    ///	tableView->setItemDelegate(&delegate);

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    QHeaderView *h = tableView->verticalHeader();
    h->setVisible(false);

    QHeaderView *v = tableView->horizontalHeader();
#if QT_VERSION >= 0x050000
    v->setSectionResizeMode(0, QHeaderView::Stretch);
    v->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
    //	v->setClickable (false);
    connect(newButton, SIGNAL(clicked()), this, SLOT(_new()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(del()));
    connect(comboBox, SIGNAL(activated(int)), SLOT(comboChanged(int)));
    connect(comboBox, SIGNAL(highlighted(const QString &)),
            SLOT(condChanged(const QString &)));

    connect(tableView, SIGNAL(clicked(const QModelIndex &)),
            SLOT(eventcat_slected(const QModelIndex &)));
    connect(message, SIGNAL(textEdited(const QString &)),
            SLOT(Changed(const QString &)));
    connect(command, SIGNAL(textEdited(const QString &)),
            SLOT(Changed(const QString &)));
    connect(proc_name, SIGNAL(textEdited(const QString &)),
            SLOT(Changed(const QString &)));
    connect(comboBox, SIGNAL(activated(const QString &)),
            SLOT(Changed(const QString &)));

    checkBox_alreadyrun->hide();
    listView->hide();
    spinBox->hide();
    label_cpu->hide();
    ///	printf("close ...\n");
    //	tableView->update();
    //	listmodel->update(); // meaningless..

    TBloon *bloon = new TBloon(this);
    return;
}

void WatchdogDialog::showEvent(QShowEvent *event)
{
    //  Qt 4.4.0 bug?
    //  printf("show!!!!!!!!!\n");
    listmodel->update();
}
void WatchdogDialog::comboChanged(int idx)
{

    // itemText(idx);
    QString str = comboBox->currentText();

    if (str.contains("cpu"))
    {
        label_cpu->show();
        spinBox->show();
    }
    else
    {
        spinBox->hide();
        label_cpu->hide();
    }

    if (str.contains("process"))
    {
        label_procname->show();
        proc_name->show();
    }
    else
    {
        label_procname->hide();
        proc_name->hide();
    }

    if (message->text().isEmpty())
    {
        //	if(str.contains("start")) message->setText("%CMD start
        // with pid %PID");
        //	if(str.contains("finish"))	message->setText("%CMD
        // finish with pid
        //%PID");
    }
}

void WatchdogDialog::eventcat_slected(const QModelIndex &idx)
{

    watchCond *w = watchlist[idx.row()];
    //	printf("row=%d\n",at=idx.row());

    if (idx.column() == 1)
    {
        w->enable = !(w->enable);
        listmodel->update(idx.row());
        return;
    }

    QString str = idx.data().toString(); // Qt::DisplayRol

    if (str.contains("process"))
        proc_name->setText(w->procname);
    else
        proc_name->setText("");
    if (str.contains("cpu"))
        spinBox->setSingleStep(w->cpu);
    else
        spinBox->setSingleStep(50);
    if (str.contains("exec"))
        command->setText(w->command);
    else
        command->setText("");
    if (str.contains("showmsg"))
        message->setText(w->message);
    else
        message->setText("");

    checkCombo();
    comboBox->setCurrentIndex(w->cond);
}

void WatchdogDialog::Changed(const QString &str)
{
    QModelIndex idx = tableView->currentIndex();
    //	QModelIndexList list=tableView->selectedIndexes ();
    bool flag = tableView->selectionModel()->hasSelection();
    // if(list.count() and idx.isValid())
    if (flag and idx.isValid())
    {
        int at = idx.row();
        watchCond *w = watchlist[at];
        w->message = message->text();
        w->command = command->text();
        w->procname = proc_name->text();
        w->cond = comboBox->currentIndex();
        listmodel->update(at);
        // watchlist.removeAt(at);
    }
    // listmodel->update();
}

void WatchdogDialog::checkCombo()
{
    if (comboBox->count() == 1)
    {
        comboBox->clear();
        comboBox->addItem("if process start");
        comboBox->addItem("if process finish");
        return;
    }
}

// comboChanged() -> checkCombo()
void WatchdogDialog::condChanged(const QString &str)
{
    checkCombo();
    // what is this?
    // printf("chagend\n");
    // comboBox->currentText();
    // command->text();
    // message->text();
}

void WatchdogDialog::_new()
{
    tableView->clearSelection();
    proc_name->clear();
    command->clear();
    message->clear();
    comboBox->clear();
    comboBox->addItem("select condition");
}

void WatchdogDialog::add()
{
    watchCond *w = new watchCond;
    w->enable = true;
    w->cond = comboBox->currentIndex();
    w->command = command->text();
    w->message = message->text();
    w->procname = proc_name->text();
    watchlist.append(w);
    //	listView->update(QModelIndex());
    //	listView->reset();
    //	tableView->reset();
    //	listmodel->insertRow(listmodel->rowCount(QModelIndex()));
    // tableView->update(QModelIndex());
    // tableView->dataChanged(QModelIndex(),QModelIndex()); //protected
    listmodel->update();
}

void WatchdogDialog::del()
{
    // QModelIndex idx=listView->currentIndex();
    QModelIndex idx = tableView->currentIndex();
    if (idx.isValid())
    {
        int at = idx.row();
        watchlist.removeAt(at);
    }
    listmodel->update();
    tableView->setCurrentIndex(idx);
}

void WatchdogDialog::apply()
{
    qps->write_settings();
    close();
}

