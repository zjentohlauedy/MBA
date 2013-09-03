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

CREATE TABLE Pitcher_Stats_T
(
   Player_Id           INTEGER,
   Season              INTEGER,
   Season_Phase        INTEGER,
   Wins                INTEGER,
   Losses              INTEGER,
   Games               INTEGER,
   Saves               INTEGER,
   Innings             REAL,
   Hits                INTEGER,
   Earned_Runs         INTEGER,
   Home_Runs           INTEGER,
   Walks               INTEGER,
   Strike_Outs         INTEGER,
--
   PRIMARY KEY ( Player_Id, Season, Season_Phase )
);

CREATE TABLE Pitcher_Accolades_T
(
   Player_Id           INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( Player_Id, Season, Accolade )
);

CREATE TABLE Batters_T
(
   Player_Id           INTEGER PRIMARY KEY,
   Power               INTEGER,
   Hit_N_Run           INTEGER,
   Bunt                INTEGER,
   Running             INTEGER,
   Range               INTEGER,
   Arm                 INTEGER,
   Primary_Position    INTEGER,
   Secondary_Position  INTEGER
);

CREATE TABLE Batter_Stats_T
(
   Player_Id      INTEGER,
   Season         INTEGER,
   Season_Phase   INTEGER,
   Games          INTEGER,
   At_Bats        INTEGER,
   Runs           INTEGER,
   Hits           INTEGER,
   Doubles        INTEGER,
   Triples        INTEGER,
   Home_Runs      INTEGER,
   Runs_Batted_In INTEGER,
   Walks          INTEGER,
   Strike_Outs    INTEGER,
   Steals         INTEGER,
   Errors         INTEGER,
--
   PRIMARY KEY ( Player_Id, Season, Season_Phase )
);

CREATE TABLE Batter_Accolades_T
(
   Player_Id           INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( Player_Id, Season, Accolade )
);

CREATE TABLE Teams_T
(
   Team_Id         INTEGER PRIMARY KEY,
   Name            TEXT,
   Location        TEXT,
   Primary_Color   INTEGER,
   Secondary_Color INTEGER
);

CREATE TABLE Team_Players_T
(
   Team_Id   INTEGER,
   Season    INTEGER,
   Player_Id INTEGER,
--
   UNIQUE ( Team_Id, Season, Player_Id )
);

CREATE TABLE Team_Stats_T
(
   Team_Id         INTEGER,
   Season          INTEGER,
   Season_Phase    INTEGER,
   Wins            INTEGER,
   Losses          INTEGER,
   Home_Wins       INTEGER,
   Home_Losses     INTEGER,
   Road_Wins       INTEGER,
   Road_Losses     INTEGER,
   Division_Wins   INTEGER,
   Division_Losses INTEGER,
   League_Wins     INTEGER,
   League_Losses   INTEGER,
   Runs_Scored     INTEGER,
   Runs_Allowed    INTEGER,
--
   PRIMARY KEY ( Team_Id, Season, Season_Phase )
);

CREATE TABLE Team_Pitching_Stats_T
(
   Team_Id             INTEGER,
   Season              INTEGER,
   Season_Phase        INTEGER,
   Wins                INTEGER,
   Losses              INTEGER,
   Games               INTEGER,
   Saves               INTEGER,
   Innings             REAL,
   Hits                INTEGER,
   Earned_Runs         INTEGER,
   Home_Runs           INTEGER,
   Walks               INTEGER,
   Strike_Outs         INTEGER,
--
   PRIMARY KEY ( Team_Id, Season, Season_Phase )
);

CREATE TABLE Team_Batting_Stats_T
(
   Team_Id        INTEGER,
   Season         INTEGER,
   Season_Phase   INTEGER,
   Games          INTEGER,
   At_Bats        INTEGER,
   Runs           INTEGER,
   Hits           INTEGER,
   Doubles        INTEGER,
   Triples        INTEGER,
   Home_Runs      INTEGER,
   Runs_Batted_In INTEGER,
   Walks          INTEGER,
   Strike_Outs    INTEGER,
   Steals         INTEGER,
   Errors         INTEGER,
--
   PRIMARY KEY ( Team_Id, Season, Season_Phase )
);

CREATE TABLE Team_Accolades_T
(
   Team_Id             INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( Team_Id, Season, Accolade )
);

CREATE TABLE Divisions_T
(
   Division_Id INTEGER PRIMARY KEY,
   Name        TEXT
);

CREATE TABLE Division_Teams_T
(
   Division_Id INTEGER,
   Team_Id     INTEGER,
--
   UNIQUE ( Division_Id, Team_Id )
);

CREATE TABLE Division_Stats_T
(
   Division_Id     INTEGER,
   Season          INTEGER,
   Season_Phase    INTEGER,
   Wins            INTEGER,
   Losses          INTEGER,
   Home_Wins       INTEGER,
   Home_Losses     INTEGER,
   Road_Wins       INTEGER,
   Road_Losses     INTEGER,
   League_Wins     INTEGER,
   League_Losses   INTEGER,
   Runs_Scored     INTEGER,
   Runs_Allowed    INTEGER,
--
   PRIMARY KEY ( Division_Id, Season, Season_Phase )
);

CREATE TABLE Division_Accolades_T
(
   Division_Id         INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( Division_Id, Season, Accolade )
);
