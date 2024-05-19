#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_file_path_cursorPositionChanged(int arg1, int arg2);

    void on_file_path_complete_clicked();

    void on_pushButton_mac_clicked();

    void on_pushButton_mac_check_clicked();

    void on_pushButton_ok_ecp_clicked();

    void on_pushButton_key_gen_clicked();

    void on_pushButton_clear_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
