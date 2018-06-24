#include "qjointaccount.h"
#include <QApplication>

#include <QSettings>
#include <QDate>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    bool openGui = false;
    bool mode = false; // false=demand, true=confirmation

    // Read settings
    // (name and link to global files)
    QSettings settings( "QJA_Config.ini", QSettings::IniFormat );
    settings.beginGroup("Settings");
    QString name = settings.value("name").toString();
    QString namePartner;
    QString path = settings.value("path").toString();
    settings.endGroup();
    if (name == "c")
        namePartner = "j";
    else
        namePartner = "c";

    // Read Config.ini from path
    QSettings config( path+"Config.ini", QSettings::IniFormat );
    config.beginGroup("Options");
    int payday = config.value("payday").toInt();
    int targetAmount = config.value("targetAmount").toInt();
    config.endGroup();
    config.beginGroup("Flags");
    QDate lastChange = QDate::fromString(config.value("lastChange").toString(),"dd.MM.yyyy");
    QDate nextPayday = QDate::fromString(config.value("nextPayday").toString(),"dd.MM.yyyy");
    bool amount = config.value(name + "Amount").toBool();
    bool amountPartner = config.value(namePartner + "Amount").toBool();
    bool paid = config.value(name + "Paid").toBool();
    bool paidPartner = config.value(namePartner + "Paid").toBool();
    config.endGroup();

    // Payday changed?
    if (nextPayday.day()<payday)
    {
        nextPayday.setDate(nextPayday.year(), nextPayday.month(), payday);
        config.beginGroup("Flags");
        config.setValue("nextPayday", nextPayday);
        config.endGroup();
    }
    else if (nextPayday.day()>payday)
    {
        nextPayday = nextPayday.addMonths(1);
        nextPayday.setDate(nextPayday.year(), nextPayday.month(), payday);
        config.beginGroup("Flags");
        config.setValue("nextPayday", nextPayday);
        config.endGroup();
    }

    // New month? Amount unknown? Both amounts known -> sum known
    // (starting with "payday" in Config.ini)
    QDate date = QDate::currentDate();
    if (nextPayday<=date)
    {
        // increment payday, reset flags
        openGui = true;
        config.beginGroup("Flags");
        config.setValue("nextPayday", (nextPayday.addMonths(1).toString("dd.MM.yyyy")));
        config.setValue("cAmount", 0);
        config.setValue("jAmount", 0);
        config.setValue("cPaid", 0);
        config.setValue("jPaid", 0);
        config.endGroup();

        QFile database(path+"GemeinschaftskontoDatenbank.txt");
        database.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append); // todo: catch error
        QTextStream databaseStream(&database);
        databaseStream << date.year() << "\t" << date.month() << "\t" << targetAmount
                       << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << endl;
    }
    else if (!amount)
    {
        openGui = true;
    }
    else if (amount && amountPartner)
    {
        openGui = true;
        mode = true;
    }


    // In GUI: Wenn Änderung oder neuer Monat: neuer Eintrag in lastChanged

    if (openGui)
    {
        QApplication app(argc, argv);
        QJointAccount w;
        w.show();
        return app.exec();
    }
    else return 0;
}
