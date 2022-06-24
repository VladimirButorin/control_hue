#include "chueconfigprocessor.h"
#include <QTimer>
#include <QDebug>


CHueConfigProcessor::CHueConfigProcessor(QNetworkAccessManager *nam, CHueJsonConfig *parent) :
    CHueJsonConfig(parent)
  , mNetAccessManager(nam)
{
    connect(this, &CHueJsonConfig::configReady, this, &CHueConfigProcessor::doConnect);
    connect(this, &CHueConfigProcessor::connected, this, &CHueConfigProcessor::doProcess);
    connect(&mTimer, &QTimer::timeout, this, &CHueConfigProcessor::doProcess);
    mTimer.setSingleShot(true);
}

void CHueConfigProcessor::doProcess()
{
    if (mBridge) {
        HueBridge::ConnectionStatus status;
        mBridge->testConnection(status);
        if (status == HueBridge::ConnectionStatus::Success) {
            doStep(nextAction());
        }
    }
}

void CHueConfigProcessor::stopProcess()
{
    mTimer.stop();
}

void CHueConfigProcessor::doConnect(QString ip, QString name)
{
    if (mBridge) {
        mBridge->deleteLater();
        mBridge = nullptr;
    }
    mBridge = new HueBridge(ip, name, mNetAccessManager);
    HueBridge::ConnectionStatus status;
    mBridge->testConnection(status);

    if (status == HueBridge::ConnectionStatus::Success) {
        qDebug() << "Connected to bridge.";
        mLights = HueLight::discoverLights(mBridge);
        mGroups = HueGroup::discoverGroups(mBridge);
    }
    else {
        qDebug() << "No connection to bridge.";
    }
    emit connected(status == HueBridge::ConnectionStatus::Success);
}

void CHueConfigProcessor::doStep(QJsonObject action)
{
    int interval = action.value("sleep").toInt() * 1000;
    if (interval < 1000) {
        interval = 500;
    }
    QString actionName = action.value("action").toString();
    QJsonArray params = action.value("params").toArray();

    foreach (const QJsonValue & value, action.value("names").toArray()) {
        QString name = value.toString();

        if (mBridge) {
            HueAbstractObject *obj = nullptr;

            for (auto group : mGroups) {
//                qDebug() << "group" << group->name().getName();
                if (group->name().getName() == name) {
                    obj = mGroups.fetchRaw(name);
                    break;
                }
            }

            if (!obj) {
                for (auto lamp : mLights) {
//                    qDebug() << "lamp" << lamp->name().getName();
                    if (lamp->name().getName() == name) {
                        obj = mLights.fetchRaw(name);
                        break;
                    }
                }
            }
            if (obj && obj->isValid()) {
                processHueAction(obj, actionName, params);
            }
            mTimer.start(interval);
        }
    }
}

void CHueConfigProcessor::processHueAction(HueAbstractObject *obj, QString actionName, QJsonArray params)
{
    if (obj && obj->isValid()) {
        if (actionName == "turnOn") {
            obj->turnOn();
        } else if (actionName == "turnOff") {
            obj->turnOff();
        } else if (actionName == "xy") {
            double x = params.at(0).toDouble();
            double y = params.at(1).toDouble();
            obj->setXY(x, y);
        } else if (actionName == "saturation") {
            int s = params.at(0).toInt();
            obj->setSaturation(s);
        } else if (actionName == "brightness") {
            int b = params.at(0).toInt();
            obj->setBrightness(b);
        } else if (actionName == "colorTemp") {
            int ct = params.at(0).toInt();
            obj->setColorTemp(ct);
        }
    }
}
