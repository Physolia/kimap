/*
   SPDX-FileCopyrightText: 2015 Christian Mollekopf <mollekopf@kolabsys.com>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kimaptest/fakeserver.h"
#include "kimap/session.h"
#include "kimap/idjob.h"

#include <QTest>

typedef QMap<QByteArray, QByteArray> ArrayMap;
Q_DECLARE_METATYPE(ArrayMap)

class IdJobTest: public QObject {
  Q_OBJECT

private Q_SLOTS:

void testId_data() {
    QTest::addColumn<QList<QByteArray> >("scenario");
    QTest::addColumn<ArrayMap>("values");
    QList<QByteArray> scenario;
    scenario << "S: * PREAUTH"
             << "C: A000001 ID (\"name\" \"clientid\")"
             << "S: * ID NIL"
             << "S: A000001 OK ID completed";
  
    ArrayMap values;
    values.insert("name", "clientid");
    QTest::newRow("good") << scenario << values;
}

void testId()
{
    QFETCH(QList<QByteArray>, scenario);
    QFETCH(ArrayMap, values);

    FakeServer fakeServer;
    fakeServer.setScenario(scenario);
    fakeServer.startAndWait();
    KIMAP::Session session(QStringLiteral("127.0.0.1"), 5989);

    KIMAP::IdJob *job = new KIMAP::IdJob(&session);
    foreach (const QByteArray &key, values.keys()) {
        job->setField(key, values.value(key));
    }
    bool result = job->exec();
    QVERIFY(result);
    fakeServer.quit();
}

};

QTEST_GUILESS_MAIN(IdJobTest)

#include "idjobtest.moc"
