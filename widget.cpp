#include "widget.h"
#include "ui_widget.h"
#include "customtablemodel.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->plot->addGraph();
    ui->plot->setDragableGraph(0); // data of graph(0) is dragable
    connect(ui->plot, &CustomPlot::dataChanged, this, &Widget::rebuildResultGraph);
    QCPGraph *graph = ui->plot->graph();

    CustomTableModel *model = new CustomTableModel(ui->tableView);
    model->setInternalData(graph->data().get());

    //old connect syntax due to new syntax 'pros'
    connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), ui->plot, SLOT(replot()));
    connect(model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), ui->plot, SLOT(replot()));
    connect(model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), ui->plot, SLOT(replot()));

    connect(ui->plot, &CustomPlot::dataChanged, ui->tableView, &QTableView::reset);

    ui->tableView->setModel(model);

    graph->addData(1, 1);
    graph->addData(2, 1);
    graph->addData(2, 2);
    graph->addData(3, 2);
    graph->addData(3, 5);

    rebuildResultGraph();
    ui->plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    ui->plot->setInteractions(QCP::iSelectLegend | QCP::iSelectPlottables | QCP::iSelectItems);

//    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Widget::customMenuRequested);

//    ui->tableView->addAction(new QAction(tr("Add row"), ui->tableView));
//    connect(ui->tableView->actions().last(), &QAction::triggered, this, &Widget::insertRow);
//    ui->tableView->addAction(new QAction(tr("Remove row"), ui->tableView));
//    connect(ui->tableView->actions().last(), &QAction::triggered, this, &Widget::removeRow);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::rebuildResultGraph()
{
    ui->plot->update();
    ui->plot->replot();
}

void Widget::customMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    for (auto action : ui->tableView->actions()) {
        menu->addAction(action);
    }
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void Widget::insertRow()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->tableView->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;
}

void Widget::removeRow()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->tableView->model();
    model->removeRow(index.row(), index.parent());
}


