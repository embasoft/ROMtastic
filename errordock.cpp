#include "errordock.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>

ErrorDock::ErrorDock(QTableWidget *tableWidget, QObject *parent)
{
    setErrorHandler(new ErrorHandler(this));
    setTableWidget(tableWidget);
    tableWidget->sortItems(0, Qt::DescendingOrder);
}

ErrorDock::~ErrorDock()
{

}

int ErrorDock::addError(QString type, QString description)
{
    int result = errorHandler()->addError(errorHandler()->errorCount(), type, description);
    updateView();
    return result;
}

bool ErrorDock::removeError(int id)
{
    int result = errorHandler()->removeError(id);
    updateView();
    return result;
}

ErrorHandler *ErrorDock::errorHandler() const
{
    return _errorHandler;
}

void ErrorDock::setErrorHandler(ErrorHandler *errorHandler)
{
    _errorHandler = errorHandler;
}

void ErrorDock::updateView()
{
    if (errorHandler() && tableWidget())
    {
        tableWidget()->clear();

        // MUTABLE ITERATOR
        foreach (ErrorHandler::Error error, errorHandler()->errors())
        {
            int row = tableWidget()->rowCount();
            tableWidget()->setRowCount(row + 1);

            QList<QTableWidgetItem *> item;
            item.append(new ErrorItem(QString::number(error.id)));
            item.append(new QTableWidgetItem(error.type));
            item.append(new QTableWidgetItem(error.description));

            for (int i = 0; i < item.size(); i++)
            {
                tableWidget()->setItem(row, i, item[i]);

                QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
                effect->setOffset(.0);
                effect->setBlurRadius(20.0);
                effect->setColor(Qt::red);

                tableWidget()->setGraphicsEffect(effect);
                QTimer::singleShot(1000, this, SLOT(removeGlow()));
            }
        }
    }
}
QTableWidget *ErrorDock::tableWidget() const
{
    return _tableWidget;
}

void ErrorDock::setTableWidget(QTableWidget *tableWidget)
{
    _tableWidget = tableWidget;
}

void ErrorDock::removeGlow()
{
    tableWidget()->setGraphicsEffect(NULL);
}

ErrorItem::ErrorItem(QString text) : QTableWidgetItem(text)
{

}

bool ErrorItem::operator <(const QTableWidgetItem &other) const
{
    return text().toInt() < other.text().toInt();
}

