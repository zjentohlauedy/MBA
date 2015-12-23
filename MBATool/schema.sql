CREATE TABLE Players_T
(
   Player_Id           INTEGER PRIMARY KEY,
   First_Name          TEXT,
   Last_Name           TEXT,
   First_Phoenetic     TEXT,
   Last_Phoenetic      TEXT,
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
   Innings             INTEGER,
   Outs                INTEGER,
   Hits                INTEGER,
   Earned_Runs         INTEGER,
   Home_Runs           INTEGER,
   Walks               INTEGER,
   Strike_Outs         INTEGER,
--
   PRIMARY KEY ( Player_Id, Season, Season_Phase )
);

CREATE VIEW Pitcher_Stats_V AS SELECT *,
  (CAST(Wins AS FLOAT) / (Wins + Losses)) Win_Loss_Ratio,
  (CAST(Wins AS FLOAT) / Games) Win_Pct,
  (CAST(Earned_Runs AS FLOAT) / (((CAST(Outs AS FLOAT) / 3) + Innings) / 9)) Earned_Run_Avg,
  (CAST(Hits AS FLOAT) / ((Innings * 3) + Outs + Hits)) Vs_Batting_Avg,
  (((CAST(Outs AS FLOAT) / 3) + Innings) / Games) Innings_Per_Game,
  ((CAST(Walks AS FLOAT) + Hits) / ((CAST(Outs AS FLOAT) / 3) + Innings)) WHIP,
  (CAST(Strike_Outs AS FLOAT) / (((CAST(Outs AS FLOAT) / 3) + Innings) / 9)) SO_Per_Nine,
  (CAST(Home_Runs AS FLOAT) / (((CAST(Outs AS FLOAT) / 3) + Innings) / 9)) HR_Per_Nine,
  (((((CAST(Outs AS FLOAT) / 3) + Innings) - Hits) + (Strike_Outs - Hits)) / (((CAST(Outs AS FLOAT) / 3) + Innings) / 9)) Efficiency
  FROM Pitcher_Stats_T;

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

CREATE VIEW Batter_Stats_V AS SELECT *,
  (CAST(Hits AS FLOAT) / At_Bats) Batting_Average,
  (((CAST(Hits AS FLOAT) - Doubles - Triples - Home_Runs) + (Doubles * 2) + (Triples * 3) + (Home_Runs * 4)) / At_Bats) Slugging_Average,
  ((CAST(Hits AS FLOAT) + Walks) / (At_Bats + Walks)) On_Base_Pct,
  (CAST(Strike_Outs AS FLOAT) / At_Bats) Strike_Out_Avg,
  ((CAST(Runs AS FLOAT) + Runs_Batted_In - Home_Runs) / Games) Runs_Per_Game
  FROM Batter_Stats_T;

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

CREATE VIEW Team_Stats_V AS SELECT *, (Wins - League_Wins) World_Series_Wins, (League_Wins - Division_Wins) LCS_Wins FROM Team_Stats_T;

CREATE TABLE Team_Versus_Stats_T
(
   Team_Id         INTEGER,
   Season          INTEGER,
   Season_Phase    INTEGER,
   Opponent        INTEGER,
   Wins            INTEGER,
   Losses          INTEGER,
   Runs_Scored     INTEGER,
   Runs_Allowed    INTEGER,
--
   PRIMARY KEY ( Team_Id, Season, Season_Phase, Opponent )
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
   Innings             INTEGER,
   Outs                INTEGER,
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

CREATE VIEW Division_Stats_V AS SELECT *, (Wins - League_Wins) World_Series_Wins FROM Division_Stats_T;

CREATE TABLE Division_Accolades_T
(
   Division_Id         INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( Division_Id, Season, Accolade )
);

CREATE TABLE Leagues_T
(
   League_Id INTEGER PRIMARY KEY,
   Name      TEXT
);

CREATE TABLE League_Divisions_T
(
   League_Id   INTEGER,
   Division_Id INTEGER,
--
   UNIQUE ( League_Id, Division_Id )
);

CREATE TABLE League_Stats_T
(
   League_Id       INTEGER,
   Season          INTEGER,
   Season_Phase    INTEGER,
   Wins            INTEGER,
   Losses          INTEGER,
   Home_Wins       INTEGER,
   Home_Losses     INTEGER,
   Road_Wins       INTEGER,
   Road_Losses     INTEGER,
   Runs_Scored     INTEGER,
   Runs_Allowed    INTEGER,
--
   PRIMARY KEY ( League_Id, Season, Season_Phase )
);

CREATE TABLE League_Accolades_T
(
   League_Id           INTEGER,
   Season              INTEGER,
   Accolade            INTEGER,
--
   UNIQUE ( League_Id, Season, Accolade )
);

CREATE TABLE League_Teams_T
(
   League_Id   INTEGER,
   Team_Id     INTEGER,
--
   UNIQUE ( League_Id, Team_Id )
);

CREATE TABLE Organizations_T
(
   Organization_Id INTEGER PRIMARY KEY,
   Name            TEXT,
   Abbreviation    TEXT,
   Season          INTEGER,
   Stage           INTEGER,
   Draft_Round     INTEGER,
   Pick_Number     INGETER
);

CREATE TABLE Organization_Leagues_T
(
   Organization_Id INTEGER,
   League_Id       INTEGER,
--
   UNIQUE ( Organization_Id, League_Id )
);
