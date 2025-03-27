## Q1 Dataset location and citation

https://www.kaggle.com/datasets/rush4ratio/video-game-sales-with-ratings
3/20/25

## Q3 Functional Dependencies

1. (Name, Platform) → (Year_of_Release, Genre, Publisher, Developer, Rating)

This is a non trivial functional dependecy because the right-hand attributes are not a subset of the left-hand attributes. This applies to the dataset because every game on a specific platform has a fixed release year, genre, publisher, developer, and rating. This functional dependecy violates BCNF because (Name, Platform) is not a super key, other attributes exist like the sales data which is independent.

2. (Publisher, Genre) → Rating

This is a non trivial functional dependecy because rating is not a subset of (Publisher, Genre). This applies to the dataset because most publishers tend to release games of specific genres with a typical rating. This violates BCNF because (Publisher, Genre) is not a super key, since multiple games exist under a publisher-genre combination. 

## Q4 Normalized Schema

Game_Info (game_id, Name, Platform, Year_of_Release, Genre, Developer, Publisher)

Sales (game_id, NA_Sales, EU_Sales, JP_Sales, Other_Sales, Global_Sales)

Publisher_Info (Publisher, Genre, Rating, Developer)

Reviews (game_id, Critic_Score, Critic_Count, User_Score, User_Count)

This does not violate BCNF because (game_id) is now a superkey in Game_Info, Sales, and Reviews. As well as (Publisher, Genre, Rating, Developer) is now handled sperately in Publisher_Info, which ensures all relations are in BCNF. This table seperation helps reduce the reduancy of every repeated publisher, genre, and rating. Now Game_Info only references Publisher.

## Q5 Data Cleaning

The data cleaning that I did was I removed any null values that where in any of the columns besides sales, I did this because I wanted to look at data for every game that had a valid publisher, developer, and rating. By adding this constraint it made it easiser to compare the data as well as join the data all together. Another constraint that I added was to ensuret that NA_Sales were greater than 0. I added this constraint because I wanted to remove any games that were not sold in the Americas.

## Q6 Keys and Constraints

In the sales table I added a primary key as game_id because it uniquely identifies each record in the table. This is because each game has a unique game_id and every row of sales should associated to single game. The foreign key of game_id ensure referential integrity between the Sales table and Game_Info table, that way  Each game’s sales data in the Sales table must correspond to an existing game_id in the Game_Info table. This makes it so that we cannot insert sales data for a game that does not exist in the Game_Info table, preventing orphan records. The Publisher_Info table has a primary composite key of Publisher, Genre, Developer, Rating because the same publisher may release multiple games in the same genre with different developers and ratings. As well as the combinations of these fields are unique for each game. The Reviews table is very similar to the Sales table however has User_Score constraint to ensure that values are within the proper range of user scoring. 

## Q7 Interesting Queries

The first query calculates the total North American sales for each publisher and developer, the sales are in the millions. It calculates it by joining together three different tables the Game_Info, Sales, and Publisher_Info table. The results are then grouped together by both publisher and developer, ensuring the sales are distinguished for each developer under a publisher. The total sales are then rounded to two decimal places for better readability. This is interesting because it provides insights into how well publishers perform in North America based on the different developers they have. It helps to identify succesful publisher-developer collaborations and has the potential to broaded to different regions.

The second query finds all games that have a user score greater than or equal to the average user of all games in the dataset. This uses a subquery to calculate the average user score from the revies table. This is interesting because it identifies highly rated games based on the average users/players which can be useful for finding recommended games or games that players themsevles enjoyed.bec

The third query calculate the average critic score and user score for every genre. This is interesting because it helps compare critic opinions vs user opinions for the different game genres. It can show the gaps between critic and player perceptions wehere they differ and are the same. 

## Q8 Performance Tuning

To improve performance I implemented two key optimizations the first is an index on User_Score in the Reviews table and the second is a composite index on game_id and Global_Sales in the Sales table. 

The first changed was implemented because the subquery uses an aggregatie function to filters games that have scores above the average. Below are the results from before and after the index:
| Metric              | Before Index | After Index |
|---------------------|-------------|------------|
| Query Execution Time | 1.623 sec   | 0.973 sec  |
| Rows Scanned       | 7017        | 4103       |
| Pages Read         | 151.17      | 98.32      |
| Memory Usage       | 230 kB      | 190 kB     |

The query results are 40% faster, reducing the number of scanned rows and memory footprint.

The second change was implemented because the query aggregates the data and groups by the Publisher and Developer. The join operation was reading a large number of rows due to missing index optimizations. The composite index allows the query planner to quickly retrive games sales information without scanning unnecessary data. Below are the results from before and after the change:  

| Metric              | Before Index | After Index |
|---------------------|-------------|------------|
| Query Execution Time | 621.89 ms   | 320.44 ms  |
| Rows Processed     | 1,760,410    | 853,102    |
| Memory Usage       | 77 kB        | 55 kB      |
| Hash Join Cost     | 4837.84      | 3289.42    |

The query results are 48% faster, reducing the number of scanned rows processed and memory usage.