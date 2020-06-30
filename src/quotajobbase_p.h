/*
    SPDX-FileCopyrightText: 2009 Andras Mantia <amantia@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef KIMAP_QUOTAJOBBASE_P_H
#define KIMAP_QUOTAJOBBASE_P_H

#include "job_p.h"
#include "response_p.h"
#include "session.h"

#include <QMap>

namespace KIMAP
{
class QuotaJobBasePrivate : public JobPrivate
{
public:
    QuotaJobBasePrivate(Session *session, const QString &name) : JobPrivate(session, name)
    {}

    ~QuotaJobBasePrivate() { }
    static QMap<QByteArray, QPair<qint64, qint64> > readQuota(const Response::Part &content);

    QMap<QByteArray, QPair<qint64, qint64> > quota;
};
}

#endif
