#include <QCoreApplication>
#include <QTime>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QtDebug>
#include <QDirIterator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);
 QSqlDatabase db;
 QSqlTableModel* modelDevices;
 QString DateFileConf[5];

 qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));

 if(argc != 2)
   {
    printf("%s","wrong argc!");
    return 0;
    }

 QDirIterator iterator ("/home", QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
 while(iterator.hasNext())
 {
 iterator.next();
 if(iterator.fileName() == argv[1])
   {
    qDebug() << iterator.fileInfo().absoluteFilePath();
    break;
   }
 }


 QString field[] = {"ip_bd:","bd_port:","bd:","log_bd:","pass_bd:"};
 QByteArray data;
 QFile AccessBase(iterator.fileInfo().absoluteFilePath()); //argv[1]
 unsigned i = 0;
 if(AccessBase.open(QIODevice::ReadOnly))
   {
     while(!(AccessBase.atEnd()))
     {
      data = AccessBase.readLine();
      int temp = data.indexOf(field[i],0);
      if(temp>0)
        {
         temp +=field[i].size();
         data.remove(0,temp);

         if(i > 4)
            break;
           else
              {
               DateFileConf[i].append(data);
               qDebug() << data;
               i++;
              }
        }
     }
   }
  else
      {
       qDebug() << "Error open:" <<AccessBase.errorString ();
       return 0;
      }
 AccessBase.close();

 db = QSqlDatabase::addDatabase("QPSQL");
 db.setHostName(DateFileConf[0]);
 db.setPort(DateFileConf[1].toInt());
 db.setDatabaseName(DateFileConf[2].trimmed());
 db.setUserName(DateFileConf[3].trimmed());
 db.setPassword(DateFileConf[4].trimmed());

 if(!db.open())
   {
   qDebug() <<"DataBaseConnectError";
   }
  else
     {
      qDebug() <<"Successful...";
      modelDevices = new QSqlTableModel(0,db);
      modelDevices->setTable("test_light_soft");
      modelDevices->select();

      QSqlQuery query = QSqlQuery(db);
      query.prepare("INSERT INTO test_light_soft (number_1, number_2) "
                    "VALUES (:number_1, :number_2)");
      query.bindValue(":number_1",qrand()%5000);
      query.bindValue(":number_2",qrand()%5000);

      if(!query.exec())
        {
         qDebug() <<db.lastError().text();
        }
     }

 return a.exec();
}
