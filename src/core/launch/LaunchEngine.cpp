#include "Headers/LaunchEngine.h"
#include "../download/Headers/VersionDetails.h"

LaunchEngine::LaunchEngine(QObject *parent): QObject(parent){
}

LaunchEngine::Platform LaunchEngine::currentPlatform(){
#if defined(Q_OS_WIN)
    return Platform::Windows;
#elif defined(Q_OS_MACOS)
    return Platform::MacOS;
#else
    return Platform::Linux;
#endif
}

QString LaunchEngine::javaExecutableName()
{
    return (currentPlatform() == Platform::Windows) ? QStringLiteral("java.exe") : QStringLiteral("java");
}

QString LaunchEngine::classPathSeparator(){
    return (currentPlatform() == Platform::Windows) ? QStringLiteral(";") : QStringLiteral(":");
}

QString LaunchEngine::javaExecutablePath(const QString &baseDir, const QString &runtimeName) const {
#if defined(Q_OS_MACOS)
    return QDir::cleanPath(baseDir + "/runtime/" + runtimeName + "/jre.bundle/Contents/Home/bin/java");
#else
    return QDir::cleanPath(baseDir + "/runtime/" + runtimeName + "/bin/" + javaExecutableName());
#endif
}

QString LaunchEngine::buildClassPath(const QStringList &libraryPath, const QString &clientJarPath) const{
    QStringList allPaths = libraryPath;
    allPaths << clientJarPath;
    return allPaths.join(classPathSeparator());
}

QString LaunchEngine::resolvePlaceholders(const QString &args, const QMap<QString, QString> &vars) const{
    QString result = args;
    for (auto it = vars.constBegin(); it != vars.constEnd(); ++it){
        QString placeholder = QStringLiteral("${") + it.key() + QStringLiteral("}");
        result.replace(placeholder, it.value());
    }
    return result;
}

QStringList LaunchEngine::resolveArgumentList(const QStringList &args, const QMap<QString, QString> &vars) const{
    QStringList resolved;
    for (const QString &arg : args){
        resolved << resolvePlaceholders(arg, vars);
    }
    return resolved;
}

QStringList LaunchEngine::platformExtraJvmFlags() const {
    QStringList extra;
#if defined(Q_OS_MACOS)
    extra << QStringLiteral("-XstartOnFirstThread");
#endif
    return extra;
}

bool LaunchEngine::loadLoaderProfile(const QString &instanceDir, const QString &baseDir, QString &mainClassOut, QStringList &extraLibsOut) const {
    // Check for fabric or quilt profile
    QString profilePath;
    if (QFile::exists(instanceDir + "/fabric-profile.json"))
        profilePath = instanceDir + "/fabric-profile.json";
    else if (QFile::exists(instanceDir + "/quilt-profile.json"))
        profilePath = instanceDir + "/quilt-profile.json";
    else
        return false;

    QFile file(profilePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject())
        return false;

    QJsonObject root = doc.object();
    mainClassOut = root["mainClass"].toString();

    // Parse loader libraries and convert to paths
    QJsonArray libraries = root["libraries"].toArray();
    for (const QJsonValue &val : libraries) {
        QJsonObject lib = val.toObject();
        QString name = lib["name"].toString();

        QStringList parts = name.split(':');
        if (parts.size() < 3) continue;

        QString group    = parts[0].replace('.', '/');
        QString artifact = parts[1];
        QString version  = parts[2];
        QString path     = QString("%1/%2/%3/%2-%3.jar").arg(group, artifact, version);
        QString fullPath = QDir::cleanPath(baseDir + "/libraries/" + path);

        if (QFile::exists(fullPath))
            extraLibsOut << fullPath;
    }

    return true;
}

void LaunchEngine::launch(const VersionDetails &details, const QString &instanceName, const QString &playerName, const QString &playerUUID, const QString &accessToken, const QString &baseDir){

    QString javaPath = javaExecutablePath(baseDir, details.javaRuntimeName);
    qDebug() << "[LaunchEngine] Java:" << javaPath;

    QString instanceDir = QDir::cleanPath(baseDir + "/instances/" + instanceName);
    QString clientJarPath = instanceDir + "/client.jar";

    // Check for loader profile and override mainClass + prepend loader libs
    QString mainClass = details.mainClass;
    QStringList loaderLibs;
    bool hasLoader = loadLoaderProfile(instanceDir, baseDir, mainClass, loaderLibs);

    if (hasLoader)
        qDebug() << "[LaunchEngine] Loader profile found, mainClass:" << mainClass;

    // Loader libs go first in classpath so they take priority
    QStringList allLibPaths = loaderLibs + details.libraryPaths;
    QString classPath = buildClassPath(allLibPaths, clientJarPath);

    QMap<QString, QString> vars;
    vars["auth_player_name"]  = playerName;
    vars["auth_uuid"]         = playerUUID;
    vars["auth_access_token"] = accessToken;
    vars["user_type"]         = "mojang";
    vars["version_name"]      = details.versionId;
    vars["version_type"]      = "release";
    vars["game_directory"]    = QDir::toNativeSeparators(instanceDir);
    vars["assets_root"]       = QDir::toNativeSeparators(baseDir + "/assets");
    vars["assets_index_name"] = details.assetIndexId;
    vars["classpath"]         = classPath;
    vars["launcher_name"]     = "MarynLauncher";
    vars["launcher_version"]  = "0.1.0";
    vars["natives_directory"] = QDir::toNativeSeparators(instanceDir + "/natives");

    QDir().mkpath(instanceDir + "/natives");

    QStringList jvmArgs = resolveArgumentList(details.jvmArgs, vars);
    jvmArgs << platformExtraJvmFlags();

    QStringList gameArgs = resolveArgumentList(details.gameArgs, vars);

    QStringList fullArgs;
    fullArgs << jvmArgs;
    fullArgs << "-cp" << classPath;
    fullArgs << mainClass;
    fullArgs << gameArgs;

    qDebug() << "[LaunchEngine] fullArgs:" << fullArgs;

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    connect(process, &QProcess::readyReadStandardOutput, this, [this, process](){
        emit logOutput(QString::fromUtf8(process->readAllStandardOutput()));
    });

    connect(process, &QProcess::finished, this, [this](int exitCode){
        qDebug() << "[LaunchEngine] Game exited:" << exitCode;
        emit gameFinished(exitCode);
    });

    connect(process, &QProcess::errorOccurred, this, [this, process](QProcess::ProcessError){
        emit launchError(process->errorString());
    });

    process->start(javaPath, fullArgs);
    if(!process->waitForStarted(5000)){
        emit launchError("Failed to start Java: " + process->errorString());
        return;
    }

    emit gameStarted();
}