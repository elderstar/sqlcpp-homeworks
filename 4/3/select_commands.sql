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