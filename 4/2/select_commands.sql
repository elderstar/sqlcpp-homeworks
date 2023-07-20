SELECT title, MAX(duration) FROM tracks GROUP BY title ORDER BY max DESC LIMIT 1;

SELECT title, duration FROM tracks WHERE duration >= '00:03:50';
SELECT title, year_release FROM collections WHERE year_release >= 2018
AND year_release <=2020;
SELECT title FROM musicians WHERE title NOT LIKE '% %';
SELECT title FROM tracks WHERE title LIKE '%Мой%';