CREATE TABLE Players_T
(
   Player_Id           INTEGER PRIMARY KEY,
   First_Name          TEXT,
   Last_Name           TEXT,
   First_Phonetic      TEXT,
   Last_Phonetic       TEXT,
   Skin_Tone           INTEGER,
   Handedness          INTEGER,
   Player_Type         INTEGER,
   Rookie_Season       INTEGER,
   Longevity           INTEGER
);

CREATE TABLE Player_Accolades_T
(
   Player_Id           INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( Player_Id, Season, Accolade )
);

CREATE TABLE Pitchers_T
(
   Player_Id           INTEGER PRIMARY KEY,
   Speed               INTEGER,
   Control             INTEGER,
   Bunt                INTEGER,
   Fatigue             INTEGER
);
