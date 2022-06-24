#pragma once

#include <QObject>
#include <QJsonObject>


class CHueJsonConfig : public QObject
{
    Q_OBJECT
public:
    explicit CHueJsonConfig(QObject *parent = nullptr);

public slots:
    void setConfigName(QString name);
    QJsonObject nextAction();

signals:
    void configReady(QString ip, QString user);
private:
    QString mFilename;
    QJsonObject mConfig;
    int mStep = 0;
};
