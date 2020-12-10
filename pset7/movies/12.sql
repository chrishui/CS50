/*  SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred */

SELECT movies.title FROM movies WHERE movies.id IN

(SELECT stars.movie_id FROM stars WHERE stars.person_id IN

(SELECT people.id FROM people WHERE people.name IN ("Johnny Depp", "Helena Bonham Carter"))

GROUP BY stars.movie_id

HAVING COUNT(stars.movie_id)>1

);

/* Alternate solution:

SELECT movies.title FROM movies WHERE movies.id IN

(SELECT stars.movie_id FROM stars WHERE stars.person_id IN (SELECT people.id FROM people WHERE people.name = "Johnny Depp" ))

INTERSECT

SELECT movies.title FROM movies WHERE movies.id IN

(SELECT stars.movie_id FROM stars WHERE stars.person_id IN (SELECT people.id FROM people WHERE people.name = "Helena Bonham Carter" ));

___

SELECT movies.title FROM movies WHERE movies.id IN

(SELECT stars.movie_id FROM stars WHERE stars.person_id IN

(SELECT people.id FROM people WHERE people.name IN ("Johnny Depp", "Helena Bonham Carter"))

GROUP BY stars.movie_id

HAVING COUNT(*)>1

);

*/