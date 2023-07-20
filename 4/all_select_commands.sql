--Задание 4.2

SELECT title, MAX(duration) FROM tracks GROUP BY title ORDER BY max DESC LIMIT 1;

SELECT title, duration FROM tracks WHERE duration >= '00:03:50';
SELECT title, year_release FROM collections WHERE year_release >= 2018
AND year_release <=2020;
SELECT title FROM musicians WHERE title NOT LIKE '% %';
SELECT title FROM tracks WHERE title LIKE '%Мой%';

--Задание 4.3

SELECT genre_id, mg.title, COUNT(musiciant_id) FROM musicians_to_genres LEFT JOIN musical_genres mg ON genre_id = mg.id GROUP BY title, genre_id;

SELECT COUNT(*)
FROM tracks WHERE tracks.album_id IN
(SELECT id FROM albums WHERE year_release >=2019 AND year_release <=2020);

SELECT album_id, a.title, AVG(duration) FROM tracks t LEFT JOIN albums a ON t.album_id = a.id GROUP BY album_id, a.title;

SELECT title FROM musicians
WHERE id IN
(SELECT DISTINCT musiciant_id FROM musicians_to_albums WHERE musiciant_id IN 
(SELECT id FROM albums WHERE year_release != 2020));

SELECT title FROM collections WHERE id IN 
(SELECT DISTINCT collection_id FROM tracks_to_collections WHERE track_id IN
(SELECT id FROM tracks WHERE album_id IN
(SELECT album_id FROM musicians_to_albums WHERE musiciant_id = 1)));

--Задание 4.4*

SELECT title FROM albums WHERE id IN
(SELECT distinct album_id FROM musicians_to_albums WHERE musiciant_id IN 
(SELECT musiciant_id FROM musicians_to_genres GROUP BY musiciant_id HAVING COUNT(musiciant_id) > 1 )); 

SELECT title FROM tracks WHERE id NOT IN (SELECT DISTINCT track_id FROM tracks_to_collections);

SELECT title FROM musicians WHERE id = (SELECT musiciant_id FROM musicians_to_albums WHERE album_id = (SELECT album_id FROM tracks WHERE duration = (SELECT MIN(duration) FROM tracks)));

SELECT title FROM albums WHERE id = (SELECT album_id FROM (SELECT album_id, count(album_id) FROM tracks GROUP BY album_id ORDER BY count LIMIT 1) al_id);