#include "Headers/LaunchEngine.h"

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
    return (currentPlatform() ==Platform::Windows?) ? QStringLiteral("java.exe") : QStringLiteral("java");
}

QString LaunchEngine::classPathSeparator(){
    return (currentPlatform()==Platform::Windows) ? QStringLiteral(";") : QStringLiteral(":");
}

QString LaunchEngine::buildClassPath(const QStringList &libraryPath , const QString &clientJarPath) const{
    QStringList allPaths = libraryPaths;
    allPaths << ClientJarPath;

    return allPaths.join(classPathSeparator());
}

 QString LaunchEngine::ressolvePlaceholders(const QString &args , const QMAP<QString, QString> &vars) const{
    QString result = arg;
    for (auto it = vars.constBegin() ; it != vars.consEnd() ;  ++it){
        QString placeholder = QStringLiteral("${") + it.key() + QStringLiteral("}");
        result.replace(placeholder, it.value());
    }

    return result;
}


QString LaunchEngine::ressolveArgumentList(const QStringList &args , const QMAP<QString, QString> &vars) const{
    QString resolved;

    for (const QString &arg : arg){
        resolved<<resolvedPlaceholders(arg,vars)
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
