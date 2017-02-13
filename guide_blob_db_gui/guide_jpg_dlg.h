#ifndef GUIDE_JPG_DLG_H
#define GUIDE_JPG_DLG_H

#include <QDialog>

namespace Ui {
class guide_jpg_dlg;
}

class guide_jpg_dlg : public QDialog
{
	Q_OBJECT
	
public:
	explicit guide_jpg_dlg(QWidget *parent = 0);
	~guide_jpg_dlg();
	
private:
	Ui::guide_jpg_dlg *ui;
	QPalette * m_pPalette ;
	QPixmap * m_pPixmap ;

private slots:
	void on_pushButton_load_momory_clicked();

protected:
	void paintEvent(QPaintEvent *);
};

#endif // GUIDE_JPG_DLG_H
