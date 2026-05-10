#ifndef MAPSELECT_H
#define MAPSELECT_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <string>

class MapSelect : public QDialog
{
    Q_OBJECT

public:
    explicit MapSelect(QWidget *parent = nullptr);
    ~MapSelect();

    std::string selectedMap() const;
    void setMode(bool isPlayMode);
    void refreshMapList();  // 公开刷新方法

signals:
    void newMapRequested();  // 新建地图信号

private slots:
    void onItemClicked(QListWidgetItem *item);
    void onNewMapClicked();
    void onStartClicked();
    void onRenameClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onCancelClicked();

private:
    bool deleteMapFile(const QString &mapName);
    bool renameMapFile(const QString &oldName, const QString &newName);
    void showDefaultUI();
    void showStartButton();
    void showActionButtons();

    QListWidget *mapList;
    QPushButton *newMapButton;
    QPushButton *startButton;
    QPushButton *renameButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *cancelButton;
    QLabel *infoLabel;
    std::string selectedMapName;
    bool isPlayMode;
};

#endif // MAPSELECT_H