#include "projecthistorymanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

ProjectHistoryManager* ProjectHistoryManager::m_instance = nullptr;

ProjectHistoryManager* ProjectHistoryManager::instance()
{
    if (!m_instance) {
        m_instance = new ProjectHistoryManager();
    }
    return m_instance;
}

ProjectHistoryManager::ProjectHistoryManager(QObject *parent)
    : QObject(parent), m_db(nullptr)
{
    initialize();
}

ProjectHistoryManager::~ProjectHistoryManager()
{
    if (m_db && m_db->isOpen()) {
        m_db->close();
        delete m_db;
    }
}

bool ProjectHistoryManager::initialize()
{
    m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "project_history"));

    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(dataPath);
    }

    QString dbPath = dataPath + "/project_history.db";
    m_db->setDatabaseName(dbPath);

    if (!m_db->open()) {
        qWarning() << "Cannot open database:" << m_db->lastError().text();
        return false;
    }

    return createTables();
}

bool ProjectHistoryManager::createTables()
{
    QSqlQuery query(*m_db);

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS project_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            path TEXT UNIQUE NOT NULL,
            last_opened DATETIME,
            created DATETIME DEFAULT CURRENT_TIMESTAMP,
            is_active BOOLEAN DEFAULT 0,
            open_count INTEGER DEFAULT 1
        )
    )";

    if (!query.exec(sql)) {
        qWarning() << "Create table failed:" << query.lastError().text();
        return false;
    }

    // 创建索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_path ON project_history(path)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_last_opened ON project_history(last_opened DESC)");

    return true;
}

bool ProjectHistoryManager::addProject(const QString &name, const QString &path)
{
    if (!m_db || !m_db->isOpen()) return false;

    // 先检查是否已存在
    QSqlQuery checkQuery(*m_db);
    checkQuery.prepare("SELECT id FROM project_history WHERE path = :path");
    checkQuery.bindValue(":path", path);

    if (!checkQuery.exec()) {
        qWarning() << "Check query failed:" << checkQuery.lastError();
        return false;
    }

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (checkQuery.next()) {
        // 更新现有记录
        QSqlQuery updateQuery(*m_db);
        updateQuery.prepare(R"(
            UPDATE project_history
            SET name = :name,
                last_opened = :time,
                open_count = open_count + 1
            WHERE path = :path
        )");
        updateQuery.bindValue(":name", name);
        updateQuery.bindValue(":time", currentTime);
        updateQuery.bindValue(":path", path);

        return updateQuery.exec();
    } else {
        // 插入新记录
        QSqlQuery insertQuery(*m_db);
        insertQuery.prepare(R"(
            INSERT INTO project_history (name, path, last_opened, created)
            VALUES (:name, :path, :time, :time)
        )");
        insertQuery.bindValue(":name", name);
        insertQuery.bindValue(":path", path);
        insertQuery.bindValue(":time", currentTime);

        return insertQuery.exec();
    }
}

bool ProjectHistoryManager::updateProjectTime(const QString &path)
{
    if (!m_db || !m_db->isOpen()) return false;

    QSqlQuery query(*m_db);
    query.prepare(R"(
        UPDATE project_history
        SET last_opened = :time,
            open_count = open_count + 1
        WHERE path = :path
    )");
    query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":path", path);

    return query.exec();
}

bool ProjectHistoryManager::setActiveProject(const QString &path)
{
    if (!m_db || !m_db->isOpen()) return false;

    // 清除所有活动的
    QSqlQuery clearQuery(*m_db);
    clearQuery.exec("UPDATE project_history SET is_active = 0");

    // 设置新的活动项目
    QSqlQuery setQuery(*m_db);
    setQuery.prepare("UPDATE project_history SET is_active = 1 WHERE path = :path");
    setQuery.bindValue(":path", path);

    return setQuery.exec();
}

bool ProjectHistoryManager::removeProject(const QString &path)
{
    if (!m_db || !m_db->isOpen()) return false;

    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM project_history WHERE path = :path");
    query.bindValue(":path", path);

    return query.exec();
}

bool ProjectHistoryManager::hasProject(const QString &path)
{
    if (!m_db || !m_db->isOpen()) return false;

    QSqlQuery query(*m_db);
    query.prepare("SELECT COUNT(*) FROM project_history WHERE path = :path");
    query.bindValue(":path", path);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

QList<ProjectHistory> ProjectHistoryManager::getRecentProjects(int limit)
{
    QList<ProjectHistory> projects;

    if (!m_db || !m_db->isOpen()) return projects;

    QSqlQuery query(*m_db);
    query.prepare("SELECT * FROM project_history ORDER BY last_opened DESC LIMIT :limit");
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        qWarning() << "Query failed:" << query.lastError();
        return projects;
    }

    while (query.next()) {
        ProjectHistory project;
        project.id = query.value("id").toInt();
        project.name = query.value("name").toString();
        project.path = query.value("path").toString();
        project.lastOpened = query.value("last_opened").toDateTime();
        project.created = query.value("created").toDateTime();
        project.isActive = query.value("is_active").toBool();
        project.openCount = query.value("open_count").toInt();

        projects.append(project);
    }

    return projects;
}

ProjectHistory ProjectHistoryManager::getActiveProject()
{
    ProjectHistory project;

    if (!m_db || !m_db->isOpen()) return project;

    QSqlQuery query(*m_db);
    query.exec("SELECT * FROM project_history WHERE is_active = 1 LIMIT 1");

    if (query.next()) {
        project.id = query.value("id").toInt();
        project.name = query.value("name").toString();
        project.path = query.value("path").toString();
        project.lastOpened = query.value("last_opened").toDateTime();
        project.created = query.value("created").toDateTime();
        project.isActive = query.value("is_active").toBool();
        project.openCount = query.value("open_count").toInt();
    }

    return project;
}

void ProjectHistoryManager::cleanupOldRecords(int keepDays)
{
    if (!m_db || !m_db->isOpen()) return;

    QDateTime cutoff = QDateTime::currentDateTime().addDays(-keepDays);

    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM project_history WHERE last_opened < :cutoff AND is_active = 0");
    query.bindValue(":cutoff", cutoff.toString("yyyy-MM-dd HH:mm:ss"));
    query.exec();
}
