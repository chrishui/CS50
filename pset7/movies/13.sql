/* SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred. */
/* Kevin Bacon born in 1958. */
/* Kevin Bacon not included in resulting list. */

SELECT people.name FROM people WHERE people.id IN(

SELECT stars.person_id FROM stars WHERE stars.movie_id IN (

SELECT stars.movie_id FROM stars WHERE stars.person_id IN

(SELECT people.id FROM people WHERE name = "Kevin Bacon" AND birth="1958")

)

EXCEPT

SELECT people.id FROM people WHERE name = "Kevin Bacon" AND birth="1958"

)
;

