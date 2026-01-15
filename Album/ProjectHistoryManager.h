#ifndef PROJECTHISTORYMANAGER_H
#define PROJECTHISTORYMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>

class QSqlDatabase;

struct ProjectHistory {
    int id;
    QString name;
    QString path;
    QDateTime lastOpened;
    QDateTime created;
    bool isActive;
    int openCount;

    ProjectHistory() : id(-1), isActive(false), openCount(0) {}
};

class ProjectHistoryManager : public QObject
{
    Q_OBJECT
public:
    static ProjectHistoryManager* instance();

    bool initialize();

    // 项目操作
    bool addProject(const QString &name, const QString &path);
    bool updateProjectTime(const QString &path);
    bool setActiveProject(const QString &path);
    bool removeProject(const QString &path);
    bool hasProject(const QString &path);

    // 查询
    QList<ProjectHistory> getRecentProjects(int limit = 10);
    ProjectHistory getActiveProject();

    void cleanupOldRecords(int keepDays = 30);

private:
    ProjectHistoryManager(QObject *parent = nullptr);
    ~ProjectHistoryManager();

    bool createTables();
    bool openDatabase();

    QSqlDatabase* m_db;
    static ProjectHistoryManager* m_instance;
};

#endif // PROJECTHISTORYMANAGER_H
