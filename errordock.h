#ifndef ERRORDOCK_H
#define ERRORDOCK_H

#include "becreative.h"
#include "becreative_global.h"
#include <QTableWidget>

using namespace beCreative::Utils;

class ErrorDock : public QObject
{
    Q_OBJECT
public:
    explicit ErrorDock(QTableWidget *tableWidget, QObject *parent = 0);
    ~ErrorDock();

    int addError(QString type, QString description);
    bool removeError(int id);

    ErrorHandler *errorHandler() const;
    void setErrorHandler(ErrorHandler *errorHandler);

    void updateView();

    QTableWidget *tableWidget() const;
    void setTableWidget(QTableWidget *tableWidget);

private:
    ErrorHandler *_errorHandler;
    QTableWidget *_tableWidget;

signals:

public slots:
    void removeGlow();
};

class ErrorItem : public QTableWidgetItem
{
    public:
    ErrorItem(QString text);
    bool operator <(const QTableWidgetItem &other) const;
};

#endif // ERRORDOCK_H
