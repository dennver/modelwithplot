#include "customplot.h"

CustomPlot::CustomPlot(QWidget* parent) : QCustomPlot(parent)
{
}

void CustomPlot::setDragableGraph(int graph)
{
    m_dragableGraphNumber = graph;
}

void CustomPlot::mousePressEvent(QMouseEvent *event)
{
    // graph is dragable only by left mouse click
    if (event->button() == Qt::MouseButton::LeftButton) {
        double x,y;

        double bestDist = std::numeric_limits<double>::max();
        int bestIndex = 0;
        if (graphCount() > 0 && m_dragableGraphNumber >= 0 && m_dragableGraphNumber < graphCount()) {
            QCPGraph* graphPointer = graph(m_dragableGraphNumber);
            graphPointer->pixelsToCoords(event->localPos(), x, y);
            // search for closest to x,y point in graph
            if (graphPointer->data()->size() >= 1) {
                for (int n = 0; n < (graphPointer->data()->size()); n++)
                {
                    double dist = fabs((graphPointer->data()->begin() + n)->value - y );
                    dist += fabs((graphPointer->data()->begin() + n)->key - x );
                    if (dist < bestDist) {
                        // best dist and index appropriatly
                        bestDist = dist;
                        bestIndex = n;
                    }
                }

                // https://www.qcustomplot.com/documentation/dataselection.html
                // doesn't work correctly
//                QVariant details;
//                if (graph(m_dragableGraphNumber)->selectTest(event->localPos(), false, &details)) {
//                    QCPDataSelection dataPoints = details.value<QCPDataSelection>();
//                    if (dataPoints.dataPointCount() > 0)
//                        best_index = dataPoints.dataRange().begin();
//                }

                m_dragablePoint = bestIndex;
            }
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void CustomPlot::mouseReleaseEvent(QMouseEvent *event)
{
    // graph is dragable only by left mouse click
    if (event->button() == Qt::MouseButton::LeftButton) {
        m_dragablePoint = -1;
        replot();
        emit editingFinished();
    }
    QCustomPlot::mouseReleaseEvent(event);
}

void CustomPlot::mouseMoveEvent(QMouseEvent *event)
{
    double x,y;

    if (graphCount() > 0 && m_dragableGraphNumber >= 0 && m_dragableGraphNumber < graphCount()) {
        QCPGraph* pq_graph = this->graph(m_dragableGraphNumber);
        pq_graph->pixelsToCoords(event->localPos(),x,y);

        y = round(y*16) / 16; //snap to grid
        x = round(x*16) / 16; //snap to grid

        if (m_dragablePoint >= 0) {
            (pq_graph->data()->begin() + m_dragablePoint)->value = y;
            (pq_graph->data()->begin() + m_dragablePoint)->key = x;
            emit dataChanged();
            replot();
        }
    }
}
