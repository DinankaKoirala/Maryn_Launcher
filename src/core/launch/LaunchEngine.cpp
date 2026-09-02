#include "Headers/LaunchEngine.h"
#include "../download/Headers/VersionDetails.h"

LaunchEngine::LaunchEngine(QObject *parent): QObject(parent){
}

LaunchEngine::Platform LaunchEngine::currentPlatform(){
#if defined(Q_OS_WIN)
    return Platform::Windows;
#elif defined (Q_OS_MACOS)
    return Platform::MacOS;
#else
    return Platform::Linux;
#endif
}

QString LaunchEngine::javaExecutableName()
{
    return (currentPlatform() ==Platform::Windows) ? QStringLiteral("java.exe") : QStringLiteral("java");
}

QString LaunchEngine::classPathSeparator(){
    return (currentPlatform()==Platform::Windows) ? QStringLiteral(";") : QStringLiteral(":");
}

QString LaunchEngine::buildClassPath(const QStringList &libraryPath , const QString &clientJarPath) const{
    QStringList allPaths = libraryPath;
    allPaths << clientJarPath;

    return allPaths.join(classPathSeparator());
}

 QString LaunchEngine::resolvePlaceholders(const QString &args , const QMap<QString, QString> &vars) const{
    QString result = args;
    for (auto it = vars.constBegin() ; it != vars.constEnd() ;  ++it){
        QString placeholder = QStringLiteral("${") + it.key() + QStringLiteral("}");
        result.replace(placeholder, it.value());
    }

    return result;
}


QStringList LaunchEngine::resolveArgumentList(const QStringList &args , const QMap<QString, QString> &vars) const{
    QStringList resolved;

    for (const QString &arg : args){
        resolved<<resolvePlaceholders(arg,vars);
    }
    return resolved;
}

QStringList LaunchEngine::platformExtraJvmFlags() const {
    QStringList extra;

#if defined (Q_OS_MACOS)
    extra << QStringLiteral("-XstartOnFirstThread");
#endif
    return extra;
}


void LaunchEngine::launch(const VersionDetails &details, const QString &instanceName, const QString &playerName, const QString &playerUUID, const QString &accessToken, const QString &gameDir){

    QString javaPath = QDir::cleanPath(gameDir + "/cache/runtime/" + QString::number(details.javaVersion) + "/bin/" + javaExecutableName());
    qDebug() << "[LaunchEngine] Java:"<<javaPath;

    QString clientJarPath = QDir::cleanPath(gameDir + "/instances/" + instanceName + "/client.jar");

    QString classPath = buildClassPath(details.libraryPaths , clientJarPath);

    QMap<QString , QString> vars;

    vars["auth_player_name"] = playerName;
    vars["auth_uuid"] = playerUUID;
    vars["auth_access_token"] = accessToken;
    vars["user_type"] = "mojang";
    vars["version_name"] = details.versionId;
    vars["version_type"] = "release";
    vars["game_directory"]    = QDir::toNativeSeparators(gameDir + "/instances/" + instanceName);
    vars["assets_root"]       = QDir::toNativeSeparators(gameDir + "/cache/assets");
    vars["assets_index_name"] = details.assetIndexId;
    vars["classpath"] = classPath;
    vars["launcher_name"] = "MarynLauncher";
    vars["launcher_version"] = "0.1.0";

    QStringList jvmArgs = resolveArgumentList(details.jvmArgs, vars);
    jvmArgs << platformExtraJvmFlags();

    QStringList gameArgs = resolveArgumentList(details.gameArgs, vars);

    QStringList fullArgs;
    fullArgs << jvmArgs;
    fullArgs << "-cp" << classPath;
    fullArgs << details.mainClass;
    fullArgs << gameArgs;

    qDebug() << "[LaunchEngine] fullArgs:" << fullArgs;



    //launch

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    connect(process, &QProcess::readyReadStandardOutput, this , [this, process](){
       emit logOutput(QString::fromUtf8(process->readAllStandardOutput()));
    });

    connect(process, &QProcess::finished, this, [this](int exitCode){
            qDebug() << "[LaunchEngine] Game exited:" <<exitCode;
            emit gameFinished(exitCode);
    });

    connect(process, &QProcess::errorOccurred, this, [this,process](QProcess::ProcessError){
       emit launchError(process->errorString());
    });

    process ->start(javaPath, fullArgs);
    if(!process->waitForStarted(5000)){
        emit launchError("Failed to start Java: " + process->errorString());
        return;
    }

    emit gameStarted();

}























