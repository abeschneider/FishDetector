#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include "ui_mainwindow.h"

namespace Ui {
	class MainWidget;
}

class MainWindow : public QWidget
{
	Q_OBJECT

 public:
 	explicit MainWindow(QWidget *parent = 0);
 	~MainWindow();

 private slots:
  	void updatePlayerUI(QImage img);
  	void on_Play_clicked();
  	void on_LoadVideo_clicked();
	void on_SpeedUp_clicked();
	void on_SlowDown_clicked();
 private:
 	Ui::MainWidget *ui;
 	Player* myPlayer;
};
