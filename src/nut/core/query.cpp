/*!************************************************************************
**
** This file is part of Nut project.
** https://github.com/HamedMasafi/Nut
**
** Nut is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Nut is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Nut.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include "query.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

/*!
  * \class Query
  * \brief This class hold a query. A query can be used for getting database rows, editing or deleting without row fetching.
  * A query can be used for getting data from database.
  * \code
  * auto q = db.posts()->query();
  * q->join(Post::commentsTable());
  * q->orderBy(!Post::saveDateField() & Post::bodyField());
  * q->where(Post::idField() > 5);
  *
  * auto posts = q->toList();
  * \endcode
  * In below code posts is a QList<Post> that contain rows from database from this query:
  * \code
  * SELECT * FROM post WHERE post.id>5 ORDER BY post.saveDate DESC, post.body
  * \endcode
  */

/*!
 * \fn QList<T *> Query::toList(int count = -1)
 * \param count Total rows must be returned
 * \return This function return class itself
 * This function return rows
 */

/*!
 * \fn Query<T> *Query::where(WherePhrase where)
 * Where phrase is a phrase using table's static field methods.
 * \code
 * q->setWhere(Post::idField() == 4 || Post::titleField().isNull());
 * \endcode
 * In sql this is like below code:
 * \code
 * ... WHERE post.id=4 OR post.title IS NULL
 * \endcode
 * \param where Where phrase
 * \return This function return class itself
 */

/*!
 * \fn Query<T> *Query::orderBy(WherePhrase phrase)
 * \param phrase Order phrase
 * \return This function return class itself
 *  orderBy set a new order for this query. Order can be a hrase like that:
 * \code
 * query->orderBy(Post::idField());
 * \endcode
 * If you need more than one order field seprate them by & operator, example:
 * \code
 * query->orderBy(Post::idField() & Post::bodyField());
 * \endcode
 * Order can be also DESC, for that put exclamation mark near field
 * \code
 * query->orderBy(!Post::idField & Post::bodyField());
 * \endcode
 */

NUT_END_NAMESPACE

QT_END_NAMESPACE
