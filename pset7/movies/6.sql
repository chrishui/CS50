/*  SQL query to determine the average rating of all movies released in 2012 */
Select AVG(rating) FROM ratings where movie_id IN (SELECT id FROM movies where year = "2012");