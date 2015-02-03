#include "errordock.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>

ErrorDock::ErrorDock(QTreeWidget *treeWidget, QObject *parent)
{
    setErrorHandler(new ErrorHandler(this));
    setTreeWidget(treeWidget);
    treeWidget->sortItems(0, Qt::DescendingOrder);
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
    if (errorHandler() && treeWidget())
    {
        treeWidget()->clear();

        foreach (ErrorHandler::Error error, errorHandler()->errors())
        {
            ErrorItem *item = new ErrorItem(treeWidget());

            QStringList values;
            values.append(QString::number(error.id));
            values.append(error.type);
            values.append(error.description);

            for (int i = 0; i < values.size(); i++)
            {
                item->setText(i, values[i]);
            }

            treeWidget()->addTopLevelItem(item);

            if (error.type.toLower() == "error")
                glow(Qt::red, 1000);
            else if (error.type.toLower() == "warning")
                glow(Qt::yellow, 1000);
            else if (error.type.toLower() == "hint")
                glow(Qt::cyan, 1000);
            else if (error.type.toLower() == "test")
                glow(Qt::blue, 1000);
            else
                glow(Qt::red, 1000);
        }
    }
}
QTreeWidget *ErrorDock::treeWidget() const
{
    return _treeWidget;
}

void ErrorDock::glow(Qt::GlobalColor color, int msec)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setOffset(.0);
    effect->setBlurRadius(20.0);
    effect->setColor(color);

    treeWidget()->setGraphicsEffect(effect);
    QTimer::singleShot(msec, this, SLOT(removeGlow()));
}

void ErrorDock::setTreeWidget(QTreeWidget *treeWidget)
{
    _treeWidget = treeWidget;
}

void ErrorDock::removeGlow()
{
    treeWidget()->setGraphicsEffect(NULL);
}

ErrorItem::ErrorItem(QTreeWidget *parent) : QTreeWidgetItem(parent)
{

}

bool ErrorItem::operator <(const QTreeWidgetItem &other) const
{
    int column = treeWidget()->sortColumn();
    return text(column).toInt() < other.text(column).toInt();
}

