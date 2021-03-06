#include "snakewindow.h"
#include "ui_snakewindow.h"
#include <snakehead.h>
#include <snakebody.h>
#include <QGraphicsSimpleTextItem>
#include <random>
Snakewindow::Snakewindow(QWidget *parent): QMainWindow(parent), ui(new Ui::Snakewindow)
{

	//create a objects
	bodies.prepend(new Snakebody());
	timerf = new QTimer();
	fruit = new Fruit(this);
	timer = new QTimer();
	scene = new QGraphicsScene(0,0,800,800);
	head = new Snakehead(this);
	fruit->setPos(scene->width() / 2, scene->height() / 2);

	//setup the graphics view
	ui->setupUi(this);
	ui->Score->setPalette(Qt::red);
	ui->Snakeview->setScene(scene);
	ui->Snakeview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->Snakeview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//put them to the scene
	scene->addItem(fruit);
	scene->addItem(head);
	scene->addItem(bodies.front());

	//connect
	connect(timer,SIGNAL(timeout()),this,SLOT(timeevent()));
	connect(this,SIGNAL(eaten()),this,SLOT(hasbeeneaten()));
	connect(timerf,SIGNAL(timeout()),this,SLOT(collisioncheck()));
}


Snakewindow::~Snakewindow()
{
	delete ui;
}


void Snakewindow::movebodies(const QPointF prevpos)
{
	for (size_t t = 0;t < bodies.size() - 1;t++)
		{
			bodies[t]->setPos(bodies[t+1]->pos());
		}
	bodies.last()->setPos(prevpos.x(), prevpos.y());

}

void Snakewindow::makebody()
{
	//create a body and add one to the scene
	Snakebody* body = new Snakebody(this);
	body->setPos(bodies.last()->x(),bodies.last()->y());
	bodies.push_front(body);
	scene->addItem(body);
}




//push button event
void Snakewindow::on_Startgame_clicked()
{
	score = 0;
	timerf->start(1);
	scene->setFocusItem(head);
	timer->start(100);
	ui->Startgame->close();
}





//every 100 milliseconds it's checking a head's direction
void Snakewindow::timeevent()
{
		QPointF prevpos = head->pos();
	if(head->getdirection() == Snakehead::DOWN)
		{
			head->setPos(head->pos().x(),head->pos().y() + head->boundingRect().width());
		}
	if(head->getdirection() == Snakehead::UP)
		{
			head->setPos(head->pos().x(),head->pos().y() - head->boundingRect().width());
		}
	if(head->getdirection() == Snakehead::LEFT)
		{
			head->setPos(head->pos().x() - head->boundingRect().width(),head->pos().y());
		}
	if(head->getdirection() == Snakehead::RIGHT)
		{
			head->setPos(head->pos().x() + head->boundingRect().width(),head->pos().y());
		}
		movebodies(prevpos);



}
//if fruit eaten
void Snakewindow::hasbeeneaten()
{
	fruit->setPos(random() % (int)(scene->width()),random() % (int)scene->height());
	makebody();
}

// every 1 milisecond it's checkin a collision condition
void Snakewindow::collisioncheck()
{

	if(head->collidesWithItem(fruit))
		{
			score++;
			ui->Score->display(score);
			emit hasbeeneaten();
		//	qDebug()<<"COLISION";
		}

	for(size_t t = 0;t < bodies.size() - 1;t++)
		{
			if (head->pos() == bodies[t]->pos())
			emit Gameover(t);
		}

}
//if you lose
void Snakewindow::Gameover(int t)
{
	timer->stop();

}






