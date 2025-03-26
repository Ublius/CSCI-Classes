DROP TABLE IF EXISTS Sales CASCADE;
DROP TABLE IF EXISTS Publisher_Info CASCADE;
DROP TABLE IF EXISTS Reviews CASCADE;
DROP TABLE IF EXISTS Game_Info CASCADE;
DROP TABLE IF EXISTS VideoGames CASCADE;

-- (Q2) Initial Schema (CREATE TABLE ...)
CREATE TABLE VideoGames (
    game_id SERIAL PRIMARY KEY,
    Name TEXT,
    Platform TEXT,
    Year_of_Release TEXT NULL,  
    Genre TEXT NULL,                
    Publisher TEXT NULL,            
    NA_Sales FLOAT NULL,           
    EU_Sales FLOAT NULL,           
    JP_Sales FLOAT NULL,       
    Other_Sales FLOAT NULL,       
    Global_Sales FLOAT NULL,       
    Critic_Score TEXT NULL,       
    Critic_Count TEXT NULL,       
    User_Score TEXT NULL,        
    User_Count TEXT NULL,       
    Developer TEXT NULL,        
    Rating TEXT NULL         
);

-- (Q2) Load data into the table (\copy ...)
\copy VideoGames(Name, Platform, Year_of_Release, Genre, Publisher, NA_Sales, EU_Sales, JP_Sales, Other_Sales, Global_Sales, Critic_Score, Critic_Count, User_Score, User_Count, Developer, Rating) FROM 'Video_Games_Sales_as_at_22_Dec_2016.csv' DELIMITER ',' CSV HEADER;

-- (Q4) Normalized schemas (CREATE TABLE ...)
CREATE TABLE Game_Info (
    game_id SERIAL PRIMARY KEY,
    Name TEXT,
    Platform TEXT,
    Year_of_Release TEXT,
    Genre TEXT,
    Developer TEXT,
    Publisher TEXT
);

CREATE TABLE Sales (
    game_id INTEGER NOT NULL,
    Name TEXT,
    Platform TEXT,
    NA_Sales FLOAT,
    EU_Sales FLOAT,
    JP_Sales FLOAT,
    Other_Sales FLOAT,
    Global_Sales FLOAT
    -- PRIMARY KEY (game_id),
    -- FOREIGN KEY (game_id) REFERENCES Game_Info(game_id)
);

CREATE TABLE Publisher_Info (
    Publisher TEXT NOT NULL,
    Genre TEXT NOT NULL,
    Developer TEXT,
    Rating TEXT
    -- PRIMARY KEY (Publisher, Genre)
    -- FOREIGN KEY (Publisher) REFERENCES Game_Info(Publisher)
);

CREATE TABLE Reviews (
    game_id INTEGER NOT NULL,
    Name TEXT,
    Platform TEXT,
    Critic_Score FLOAT,
    Critic_Count FLOAT,
    User_Score TEXT,
    User_Count FLOAT
    -- PRIMARY KEY (game_id),
    -- FOREIGN KEY (game_id) REFERENCES Game_Info(game_id)
);

-- (Q5) Loading data from the initial table into the normalized tables  (INSERT INTO ... SELECT ...)
INSERT INTO Game_Info (Name, Platform, Year_of_Release, Genre, Developer, Publisher)
SELECT Name, Platform, Year_of_Release, Genre, Developer, Publisher
FROM VideoGames
WHERE Name IS NOT NULL AND Platform IS NOT NULL;

INSERT INTO Sales (game_id, Name, Platform, NA_Sales, EU_Sales, JP_Sales, Other_Sales, Global_Sales)
SELECT DISTINCT gi.game_id, gi.name, gi.Platform, vg.NA_Sales, vg.EU_Sales, vg.JP_Sales, vg.Other_Sales, vg.Global_Sales
FROM VideoGames vg
JOIN Game_Info gi ON vg.Name = gi.Name AND vg.Platform = gi.Platform AND vg.Year_of_Release = gi.Year_of_Release
WHERE gi.Publisher IS NOT NULL 
  AND gi.Publisher <> '' 
  AND vg.NA_Sales > 0;
-- ON CONFLICT (game_id) DO NOTHING;

INSERT INTO Publisher_Info (Publisher, Genre, Rating, Developer)
SELECT DISTINCT Publisher, Genre, Rating, Developer
FROM VideoGames
WHERE Publisher IS NOT NULL 
  AND Publisher <> ''
  AND Genre IS NOT NULL 
  AND Genre <> ''
  AND Developer IS NOT NULL
  AND Developer <> ''
  AND Rating IS NOT NULL
  AND Rating <> '';
-- ON CONFLICT (Publisher, Genre) DO NOTHING;

INSERT INTO Reviews (game_id, Name, Platform, Critic_Score, Critic_Count, User_Score, User_Count)
SELECT DISTINCT gi.game_id, gi.name, gi.Platform,
       CASE WHEN vg.Critic_Score IS NULL OR vg.Critic_Score = '' THEN NULL ELSE CAST(vg.Critic_Score AS DOUBLE PRECISION) END,
       CASE WHEN vg.Critic_Count IS NULL OR vg.Critic_Count = '' THEN NULL ELSE CAST(vg.Critic_Count AS INTEGER) END,
       CASE WHEN vg.User_Score IS NULL OR vg.User_Score = '' THEN NULL ELSE vg.User_Score END,
       CASE WHEN vg.User_Count IS NULL OR vg.User_Count = '' THEN NULL ELSE CAST(vg.User_Count AS INTEGER) END
FROM VideoGames vg
JOIN Game_Info gi ON vg.Name = gi.Name AND vg.Platform = gi.Platform
WHERE vg.Critic_Score IS NOT NULL AND vg.Critic_Score <> ''
  AND vg.Critic_Count IS NOT NULL AND vg.Critic_Count <> ''
  AND vg.User_Score IS NOT NULL AND vg.User_Score <> ''
  AND vg.User_Count IS NOT NULL AND vg.User_Count <> '';
-- ON CONFLICT (game_id) DO NOTHING;

-- (Q6) Keys and constraints (ALTER TABLE ... ADD CONSTRAINT ...)
ALTER TABLE Sales
ADD CONSTRAINT pk_sales PRIMARY KEY (game_id),
ADD CONSTRAINT fk_sales_game FOREIGN KEY (game_id) REFERENCES Game_Info(game_id);

ALTER TABLE Publisher_Info
ADD CONSTRAINT pk_publisher_info PRIMARY KEY (Publisher, Genre, Developer, Rating);

ALTER TABLE Reviews
ADD CONSTRAINT pk_reviews PRIMARY KEY (game_id),
ADD CONSTRAINT fk_reviews_game FOREIGN KEY (game_id) REFERENCES Game_Info(game_id);

-- (Q7) Interesting Queries (SELECT ...)

-- Query 1: JOIN to find total sales by Publisher
SELECT pi.Publisher, gi.Developer, ROUND(SUM(s.Global_Sales)::numeric, 2) AS Total_Sales
FROM Sales s
JOIN Game_Info gi ON s.game_id = gi.game_id
JOIN Publisher_Info pi ON gi.Publisher = pi.Publisher
GROUP BY pi.Publisher, gi.Developer;

-- Query 2: Subquery to find games with a higher user score than average
SELECT gi.Name, r.User_Score
FROM Reviews r
JOIN Game_Info gi ON r.game_id = gi.game_id
WHERE CAST(r.User_Score AS FLOAT) >= (
    SELECT AVG(CAST(User_Score AS FLOAT))
    FROM Reviews
);

-- Query 3: Aggregate function to find the average critic score by genre
SELECT gi.Genre, 
       ROUND(AVG(CAST(r.Critic_Score AS DOUBLE PRECISION))::numeric, 2) AS Avg_Critic_Score,
       ROUND(AVG(CAST(r.User_Score AS FLOAT))::numeric, 2) AS Avg_User_Score
FROM Reviews r
JOIN Game_Info gi ON r.game_id = gi.game_id
GROUP BY gi.Genre;

-- (Q8) Indexes and performance tuning (CREATE INDEX ...)

-- Index on Publisher to speed up queries by publisher
-- CREATE INDEX idx_publisher ON Game_Info (Publisher);

-- CREATE INDEX idx_user_score ON Reviews (CAST(User_Score AS FLOAT));

EXPLAIN ANALYZE
SELECT gi.Name, r.User_Score
FROM Reviews r
JOIN Game_Info gi ON r.game_id = gi.game_id
WHERE CAST(r.User_Score AS FLOAT) >= (
    SELECT AVG(CAST(User_Score AS FLOAT))
    FROM Reviews
);

-- CREATE INDEX idx_sales_game_global ON Sales (game_id, Global_Sales);

EXPLAIN ANALYZE
SELECT pi.Publisher, ROUND(SUM(s.Global_Sales)::numeric, 2) AS Total_Sales
FROM Sales s
JOIN Game_Info gi ON s.game_id = gi.game_id
JOIN Publisher_Info pi ON gi.Publisher = pi.Publisher
GROUP BY pi.Publisher;