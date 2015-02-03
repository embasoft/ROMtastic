#ifndef ERRORDOCK_H
#define ERRORDOCK_H

#include "becreative.h"
#include "becreative_global.h"
#include <QTreeWidget>

using namespace beCreative::Utils;

class ErrorDock : public QObject
{
    Q_OBJECT
public:
    explicit ErrorDock(QTreeWidget *treeWidget, QObject *parent = 0);
    ~ErrorDock();

    int addError(QString type, QString description);
    bool removeError(int id);

    ErrorHandler *errorHandler() const;
    void setErrorHandler(ErrorHandler *errorHandler);

    void updateView();

    QTreeWidget *treeWidget() const;
    void setTreeWidget(QTreeWidget *treeWidget);

    void glow(Qt::GlobalColor color, int msec);

private:
    ErrorHandler *_errorHandler;
    QTreeWidget *_treeWidget;

signals:

public slots:
    void removeGlow();
};

class ErrorItem : public QTreeWidgetItem
{
    public:
    ErrorItem(QTreeWidget *parent);
    bool operator <(const QTreeWidgetItem &other) const;
};

#endif // ERRORDOCK_H
