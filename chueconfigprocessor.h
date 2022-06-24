#pragma once

#include <QObject>
#include "QNetworkAccessManager"
#include <QTimer>
#include "chuejsonconfig.h"
#include <QJsonArray>

#include <HueLib/source/huelib.h>


class CHueConfigProcessor : public CHueJsonConfig
{
    Q_OBJECT
public:
    explicit CHueConfigProcessor(QNetworkAccessManager *nam, CHueJsonConfig *parent = nullptr);

public slots:
    void doProcess();
    void stopProcess();

private slots:
    void doConnect(QString ip, QString name);
    void doStep(QJsonObject action);
    void processHueAction(HueAbstractObject *obj, QString actionName, QJsonArray params);

signals:
    void connected(bool isConnected);

private:
    QNetworkAccessManager *mNetAccessManager = nullptr;
    HueBridge* mBridge = nullptr;
    HueLightList mLights;
    HueGroupList mGroups;

    QTimer mTimer;
};
