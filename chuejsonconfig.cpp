#include "chuejsonconfig.h"
#include "QJsonDocument"
#include "QJsonArray"
#include "QFile"
#include <QDebug>

CHueJsonConfig::CHueJsonConfig(QObject *parent)
    : QObject(parent)
{

}

void CHueJsonConfig::setConfigName(QString name)
{
    mFilename = name;

    QFile file;
    file.setFileName(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8(), &error);
    mConfig = doc.object();

    QString ip = mConfig.value("bridge").toString();
    QString user = mConfig.value("user").toString();
    mStep = 0;
    emit configReady(ip, user);
}

QJsonObject CHueJsonConfig::nextAction()
{
    QJsonObject retVal;
    QJsonArray actions = mConfig.value("actions").toArray();
    if (mStep >= actions.size()) {
        mStep = 0;
    }

    qDebug() << mStep;
    retVal = actions.at(mStep).toObject();
    qDebug() << retVal;
    mStep++;


    return retVal;
}

