#include "guide_jpg_dlg.h"
#include "ui_guide_jpg_dlg.h"
#include <QGridLayout>
#include <QBuffer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlError>
#include <QPainter>
#include <QPoint>
#include <QPen>

guide_jpg_dlg::guide_jpg_dlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::guide_jpg_dlg)
{
	ui->setupUi(this);

	m_pPixmap		= new QPixmap(":/TEST/JPG/jpg_test3.jpg");

	setWindowTitle("Stretch Background Image on Resize");
}

guide_jpg_dlg::~guide_jpg_dlg()
{
	delete ui;
}

void guide_jpg_dlg::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setPen(QPen(QColor(255,255,255)));
	p.setBrush(QBrush(QColor(0,0,255)));
	p.drawEllipse(QPoint(10,10) , 8 , 8);

	p.setBrush(QBrush(QColor(0,117,255)));
	p.drawEllipse(QPoint(40,40) , 25 , 25);

	p.setBrush(QBrush(QColor(0,117,255)));
	p.drawEllipse(QPoint(150,150) , 95 , 95);

	p.setBrush(QBrush(QColor(255,127,0)));
	p.drawEllipse(QPoint(650,350) , 200 , 200);
}

void guide_jpg_dlg::on_pushButton_load_momory_clicked()
{
	ui->label_jpg->setPixmap(*m_pPixmap);
	QString dbName( "image_test.db" );
	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
	db.setDatabaseName( dbName );
	db.open();
	QSqlQuery query = QSqlQuery( db );
	query.exec( "CREATE TABLE IF NOT EXISTS imgTable ( filename TEXT, imagedata BLOB )" );

	// Generate an image (in this case a screenshot) and store it in a QByteArray

	QByteArray inByteArray;
	QBuffer inBuffer( &inByteArray );
	inBuffer.open( QIODevice::WriteOnly );
//	m_pPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format
	m_pPixmap->save( &inBuffer, "JPG" ); // write inPixmap into inByteArray in JPG format

	QPixmap memoryPixmap = QPixmap();
	memoryPixmap.loadFromData( inByteArray );
	ui->label_memory->setPixmap(memoryPixmap);

	query.prepare( "INSERT INTO imgTable (filename, imagedata) VALUES ('jpg_test3.jpg', :imageData)" );
	query.bindValue( ":imageData", inByteArray );
	if( !query.exec() ) {
		DEBUG_ERROR("query error : %s" , query.lastError().text().toStdString().c_str());
	}

	// Get image data back from database
	if( !query.exec( "SELECT imagedata from imgTable" )) {
		DEBUG_ERROR("query error : %s" , query.lastError().text().toStdString().c_str());
	}
	query.first();
	QByteArray outByteArray = query.value( 0 ).toByteArray();
	QPixmap outPixmap = QPixmap();
	outPixmap.loadFromData( outByteArray );
	ui->label_db->setPixmap(outPixmap.scaled(ui->label_db->width(), ui->label_db->height()));
	db.close();

	m_pPixmap->save("test.jpg" , "JPG");
}
