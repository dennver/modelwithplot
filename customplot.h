#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include "qcustomplot.h"

class CustomPlot : public QCustomPlot
{
    Q_OBJECT
public:
    CustomPlot(QWidget* parent);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void setDragableGraph(int graph = -1);

signals:
    void dataChanged();
    void editingFinished();

private:
    int m_dragablePoint = -1;
    int m_dragableGraphNumber = -1;

};


#endif // CUSTOMPLOT_H
