## Q1 Dataset location and citation

https://www.kaggle.com/datasets/rush4ratio/video-game-sales-with-ratings
3/20/25

## Q3 Functional Dependencies

1. (Name, Platform) → (Year_of_Release, Genre, Publisher, Developer, Rating)

This is a non trivial functional dependecy because the right-hand attributes are not a subset of the left-hand attributes. This applies to the dataset because every game on a specific platform has a fixed release year, genre, publisher, developer, and rating. This functional dependecy violates BCNF because (Name, Platform) is not a super key, other attributes exist like the sales data which is independent.

2. (Publisher, Genre) → Rating

This is a non trivial functional dependecy because rating is not a subset of (Publisher, Genre). This applies to the dataset because most publishers tend to release games of specific genres with a typical rating. This violates BCNF becuase (Publisher, Genre) is not a super key, since multiple games exist under a publisher-genre combination. 

## Q4 Normalized Schema

Game_Info (game_id, Name, Platform, Year_of_Release, Genre, Developer, Publisher)

Sales (game_id, NA_Sales, EU_Sales, JP_Sales, Other_Sales, Global_Sales)

Publisher_Info (Publisher, Genre, Rating)

Reviews (game_id, Critic_Score, Critic_Count, User_Score, User_Count)

This does not violate BCNF because (game_id) is now a superkey in Game_Info, Sales, and Reviews. As well as (Publisher, Genre) is now handled sperately in Publisher_Info, which ensures all relations are in BCNF. This table seperation helps reduce the reduancy of every repeated publisher, genre, and rating. Now Game_Info only references Publisher.

## Q5 Data Cleaning

answer here...

## Q6 Keys and Constraints

answer here...

## Q7 Interesting Queries

answer here...

## Q8 Performance Tuning

answer here...
