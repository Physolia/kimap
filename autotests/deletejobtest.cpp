/*
   SPDX-FileCopyrightText: 2009 Andras Mantia <amantia@kde.org>

   SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
   SPDX-FileContributor: Kevin Ottens <kevin@kdab.com>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QTest>

#include "kimaptest/fakeserver.h"
#include "kimap/session.h"
#include "kimap/deletejob.h"

#include <QTest>

class DeleteJobTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testDelete_data()
    {
        QTest::addColumn<QString>("mailbox");
        QTest::addColumn<QList<QByteArray> >("scenario");

        QList<QByteArray> scenario;
        scenario << FakeServer::preauth()
                 << "C: A000001 DELETE \"foo\""
                 << "S: A000001 OK DELETE completed";
        QTest::newRow("good") << "foo" << scenario;

        scenario.clear();
        scenario << FakeServer::preauth()
                 << "C: A000001 DELETE \"foo-BAD\""
                 << "S: A000001 BAD command unknown or arguments invalid";
        QTest::newRow("bad") << "foo-BAD" << scenario;

        scenario.clear();
        scenario << FakeServer::preauth()
                 << "C: A000001 DELETE \"foo\""
                 << "S: A000001 Name \"foo\" has inferior hierarchical names";
        QTest::newRow("no") << "foo" << scenario;

        scenario.clear();
        scenario << FakeServer::preauth()
                 << "C: A000001 DELETE \"foo/bar\""
                 << "S: A000001 OK DELETE completed";
        QTest::newRow("hierarchical") << "foo/bar" << scenario;
    }

    void testDelete()
    {
        QFETCH(QString, mailbox);
        QFETCH(QList<QByteArray>, scenario);

        FakeServer fakeServer;
        fakeServer.setScenario(scenario);
        fakeServer.startAndWait();

        KIMAP::Session session(QStringLiteral("127.0.0.1"), 5989);

        KIMAP::DeleteJob *job = new KIMAP::DeleteJob(&session);
        job->setMailBox(mailbox);
        bool result = job->exec();
        QEXPECT_FAIL("bad" , "Expected failure on BAD response", Continue);
        QEXPECT_FAIL("no" , "Expected failure on NO response", Continue);
        QVERIFY(result);
        if (result) {
            QCOMPARE(job->mailBox(), mailbox);
        }

        fakeServer.quit();
    }

};

QTEST_GUILESS_MAIN(DeleteJobTest)

#include "deletejobtest.moc"
