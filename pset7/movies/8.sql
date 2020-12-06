/* SQL query to list the names of all people who starred in Toy Story */

SELECT name FROM people WHERE id IN (Select person_id from stars WHERE movie_id IN (SELECT id from movies WHERE title = "Toy Story"));

