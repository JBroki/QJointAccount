#include "qjointaccount.h"
#include "ui_qjointaccount.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>

QJointAccount::QJointAccount(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QJointAccount)
{
    ui->setupUi(this);

    connect(ui->refresh, SIGNAL(clicked()), this, SLOT(loadDatabase()));

    loadDatabase();
}

QJointAccount::~QJointAccount()
{
    delete ui;
}

void QJointAccount::loadDatabase()
{
    // Link zur Datenbank (Dropbox-File) lesen



    ui->info->setText("Test");
}
