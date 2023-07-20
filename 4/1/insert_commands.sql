INSERT INTO musical_genres (title) VALUES ( 'Панк-рок');
INSERT INTO musical_genres (title) VALUES ( 'Русский рок');
INSERT INTO musical_genres (title) VALUES ( 'Поп-рок');
INSERT INTO musical_genres (title) VALUES ( 'Инди-фолк');
INSERT INTO musical_genres (title) VALUES ( 'Инди-рок');

INSERT INTO musicians(title) VALUES ('Король и Шут');
INSERT INTO musicians(title) VALUES ('ДДТ');
INSERT INTO musicians(title) VALUES ('Мёртвые Дельфины');
INSERT INTO musicians(title) VALUES ('Di Liberta');

INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (1,1);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (1,2);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (2,2);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (2,5);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (3,2);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (3,3);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (3,5);
INSERT INTO musicians_to_genres(musiciant_id, genre_id) VALUES (4,4);

INSERT INTO albums(title, year_release) VALUES ('Истинный убийца','1993');
INSERT INTO albums(title, year_release) VALUES ('Актриса Весна','1992');
INSERT INTO albums(title, year_release) VALUES ('Одуванчики для канареек','2003');
INSERT INTO albums(title, year_release) VALUES ('dictum','2020');
INSERT INTO albums(title, year_release) VALUES ('Жаль, нет ружья','2002');

INSERT INTO musicians_to_albums(musiciant_id, album_id) VALUES (1,1);
INSERT INTO musicians_to_albums(musiciant_id, album_id) VALUES (1,5);
INSERT INTO musicians_to_albums(musiciant_id, album_id) VALUES (2,2);
INSERT INTO musicians_to_albums(musiciant_id, album_id) VALUES (3,3);
INSERT INTO musicians_to_albums(musiciant_id, album_id) VALUES (3,3);
INSERT INTO musicians_to_albums(musiciant_id, album_id) VALUES (4,4);

INSERT INTO tracks(title, album_id, duration) VALUES ('Истинный убийца',1,'00:02:10');
INSERT INTO tracks(title, album_id, duration) VALUES ('Охотник',1,'00:02:41');
INSERT INTO tracks(title, album_id, duration) VALUES ('Сапоги мертвеца',1,'00:02:40');

INSERT INTO tracks(title, album_id, duration) VALUES ('В последнюю осень',2,'00:04:44');
INSERT INTO tracks(title, album_id, duration) VALUES ('Что такое осень',2,'00:04:56');
INSERT INTO tracks(title, album_id, duration) VALUES ('Актриса весна',2,'00:05:41');

INSERT INTO tracks(title, album_id, duration) VALUES ('Мёртвый город',3,'00:02:45');
INSERT INTO tracks(title, album_id, duration) VALUES ('На моей луне',3,'00:03:21');
INSERT INTO tracks(title, album_id, duration) VALUES ('Музыка в огне',3,'00:03:49');

INSERT INTO tracks(title, album_id, duration) VALUES ('Кто здесь',4,'00:03:36');
INSERT INTO tracks(title, album_id, duration) VALUES ('Небожители',4,'00:03:14');
INSERT INTO tracks(title, album_id, duration) VALUES ('Стримы на стоп',4,'00:03:41');

INSERT INTO tracks(title, album_id, duration) VALUES ('Мой характер',5,'00:03:51');

INSERT INTO collections(title, year_release) VALUES ('Собрание','2001');
INSERT INTO collections(title, year_release) VALUES ('Юрий Шевчук И Группа ДДТ','2021');
INSERT INTO collections(title, year_release) VALUES ('Нашествие. Шаг 13','2003');
INSERT INTO collections(title, year_release) VALUES ('Лучшие Песни и Новые Хиты | Русский Рок','2020');

INSERT INTO tracks_to_collections(track_id, collection_id) VALUES (2,4);
INSERT INTO tracks_to_collections(track_id, collection_id) VALUES (8,3);
INSERT INTO tracks_to_collections(track_id, collection_id) VALUES (5,2);
INSERT INTO tracks_to_collections(track_id, collection_id) VALUES (3,1);
INSERT INTO tracks_to_collections(track_id, collection_id) VALUES (2,1);
INSERT INTO tracks_to_collections(track_id, collection_id) VALUES (13,1);

