#ifndef QJOINTACCOUNT_H
#define QJOINTACCOUNT_H

#include <QMainWindow>

namespace Ui {
class QJointAccount;
}

class QJointAccount : public QMainWindow
{
    Q_OBJECT

public:
    explicit QJointAccount(QWidget *parent = 0);
    ~QJointAccount();

private:
    Ui::QJointAccount *ui;

private slots:
    void loadDatabase();
};

#endif // QJOINTACCOUNT_H
