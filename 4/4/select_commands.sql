SELECT title FROM albums WHERE id IN
(SELECT distinct album_id FROM musicians_to_albums WHERE musiciant_id IN 
(SELECT musiciant_id FROM musicians_to_genres GROUP BY musiciant_id HAVING COUNT(musiciant_id) > 1 )); 

SELECT title FROM tracks WHERE id NOT IN (SELECT DISTINCT track_id FROM tracks_to_collections);

SELECT title FROM musicians WHERE id = (SELECT musiciant_id FROM musicians_to_albums WHERE album_id = (SELECT album_id FROM tracks WHERE duration = (SELECT MIN(duration) FROM tracks)));

SELECT title FROM albums WHERE id = (SELECT album_id FROM (SELECT album_id, count(album_id) FROM tracks GROUP BY album_id ORDER BY count LIMIT 1) al_id);