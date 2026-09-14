#include "Headers/InstanceManager.h"


void InstanceManager::createInstance(QString &instanceName, const QString &baseDir, const QString &instanceVersion, const QString &loader, const QString &loaderVersion){

    QDir dir;
    dir.mkpath(baseDir + "/instances/" + instanceName + "/");

    QString InstanceJsonPath = baseDir + "/instances/" + instanceName + "/instance.json";
    QFileInfo checkFile(InstanceJsonPath);
    if (checkFile.exists() && checkFile.isFile()){
        qDebug() << "Instance already exists, skipping.";
    }
    else{

        QJsonObject rootObject;
        rootObject["name"] = instanceName;
        rootObject["version"] = instanceVersion;
        rootObject["loader"] = loader;
        rootObject["loaderVersion"] = loaderVersion;

        QJsonDocument jsonDoc(rootObject);

        QFile file(InstanceJsonPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            return;
        }

        file.write(jsonDoc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "JSON file successfully created!";
    }
}

QList<Instance> InstanceManager::fetchInstancesList(const QString &baseDir){
    QDir instanceDir(baseDir + "/instances/");
    QList<Instance> instances;

    if (!instanceDir.exists()){
        qWarning() << "Parent directory doesnt exists:" << baseDir + "/instances/";
        return{};
    }

    QDirIterator it(instanceDir.absolutePath(), QDir::Dirs | QDir::NoDotAndDotDot);

    while (it.hasNext()) {
        it.next();
        QString subfolderPath = it.filePath();
        QString jsonFilePath = QDir(subfolderPath).filePath("instance.json");
        QFile jsonFile(jsonFilePath);

        if (jsonFile.exists()) {
            if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
               qWarning() << "Could not open file:" << jsonFilePath;
                continue; 
            }

            QByteArray fileData = jsonFile.readAll();
            jsonFile.close();

            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);
 
            if(parseError.error == QJsonParseError::NoError) {
                if(jsonDoc.isObject()) {
                    QJsonObject jsonObj = jsonDoc.object();
                    Instance tempInstance;
                    tempInstance.name = jsonObj["name"].toString();
                    tempInstance.version = jsonObj["version"].toString();
                    tempInstance.loader = jsonObj["loader"].toString("vanilla");
                    tempInstance.loaderVersion = jsonObj["loaderVersion"].toString("");

                    instances.append(tempInstance);;
                }else {
                    qWarning() << "JSON parse error in" << jsonFilePath << ":";
                }
            }
        }

    }
    return instances;
}


Instance InstanceManager::getInstanceData(QString &instanceName , const QString &baseDir){
    QDir instanceDir(baseDir + "/instances/" + instanceName + "/");
    Instance tempInstance;
    QString jsonFilePath = QDir(instanceDir).filePath("instance.json");
        QFile jsonFile(jsonFilePath);

        if (jsonFile.exists()) {
            if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
               qWarning() << "Could not open file:" << jsonFilePath;
                return {}; 
            }

            QByteArray fileData = jsonFile.readAll();
            jsonFile.close();

            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);
 
            if(parseError.error == QJsonParseError::NoError) {
                if(jsonDoc.isObject()) {
                    QJsonObject jsonObj = jsonDoc.object();
                    tempInstance.name = jsonObj["name"].toString();
                    tempInstance.version = jsonObj["version"].toString();
                    tempInstance.loader = jsonObj["loader"].toString("vanilla");
                    tempInstance.loaderVersion = jsonObj["loaderVersion"].toString("");
                }else {
                    qWarning() << "JSON parse error in" << jsonFilePath << ":";
                }
            }
        }
        return tempInstance;
}


void InstanceManager::updateInstance(const QString &instanceName, const QString &baseDir, const QString &newVersion) {
    QString jsonPath = baseDir + "/instances/" + instanceName + "/instance.json";

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QJsonObject rootObject = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    rootObject["version"] = newVersion;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to open instance.json for writing:" << file.errorString();
    return;
}
    file.write(QJsonDocument(rootObject).toJson(QJsonDocument::Indented));
    file.close();
}