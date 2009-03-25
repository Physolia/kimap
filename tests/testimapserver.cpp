#include <kcomponentdata.h>
#include <kaboutdata.h>
#include <kdebug.h>
#include <qtcpsocket.h>
#include <qcoreapplication.h>
#include <qsignalspy.h>

#include "kimap/session.h"
#include "kimap/capabilitiesjob.h"
#include "kimap/listjob.h"
#include "kimap/loginjob.h"
#include "kimap/logoutjob.h"

using namespace KIMAP;

int main( int argc, char **argv )
{
  KAboutData about("TestImapServer", 0, ki18n("TestImapServer"), "version");
  KComponentData cData(&about);

  if (argc < 4) {
    kError() << "Not enough parameters, expecting: <server> <user> <password>";
  }

  QString server = QString::fromLocal8Bit(argv[1]);
  QString user = QString::fromLocal8Bit(argv[2]);
  QString password = QString::fromLocal8Bit(argv[3]);

  kDebug() << "Querying:" << server << user << password;

  QCoreApplication app(argc, argv);
  Session session(server, 143);

  kDebug() << "Logging in...";
  LoginJob *login = new LoginJob(&session);
  login->setUserName(user);
  login->setPassword(password);
  login->exec();
  Q_ASSERT_X(login->error()==0, "LoginJob", login->errorString().toLocal8Bit());
  Q_ASSERT(session.state()==Session::Authenticated);

  kDebug() << "Asking for capabilities:";
  CapabilitiesJob *capabilities = new CapabilitiesJob(&session);
  capabilities->exec();
  Q_ASSERT_X(capabilities->error()==0, "CapabilitiesJob", capabilities->errorString().toLocal8Bit());
  Q_ASSERT(session.state()==Session::Authenticated);
  kDebug() << capabilities->capabilities();

  kDebug() << "Listing mailboxes:";
  ListJob *list = new ListJob(&session);
  list->setIncludeUnsubscribed(true);
  list->exec();
  Q_ASSERT_X(list->error()==0, "ListJob", list->errorString().toLocal8Bit());
  Q_ASSERT(session.state()==Session::Authenticated);

  int count = list->mailBoxes().size();
  for (int i=0; i<count; ++i) {
    QList<QByteArray> descriptor = list->mailBoxes()[i];
    QByteArray mailBox;
    for (int j=1; j<descriptor.size(); ++j) {
      if (j!=1) mailBox+=descriptor[0];
      mailBox+=descriptor[j];
    }
    kDebug() << mailBox;
  }

  kDebug() << "Logging out...";
  LogoutJob *logout = new LogoutJob(&session);
  logout->exec();
  Q_ASSERT_X(logout->error()==0, "LogoutJob", logout->errorString().toLocal8Bit());
  Q_ASSERT(session.state()==Session::Disconnected);

  return 0;
}