SET search_path TO terpelding, berkeley_meters;

DROP VIEW IF EXISTS q11;
DROP VIEW IF EXISTS q10;
DROP VIEW IF EXISTS q8;
DROP VIEW IF EXISTS q7;
DROP VIEW IF EXISTS q6;
DROP VIEW IF EXISTS q5;
DROP VIEW IF EXISTS q4;
DROP VIEW IF EXISTS q3;
DROP VIEW IF EXISTS q2;
DROP VIEW IF EXISTS q1;

CREATE VIEW q1 AS (
    select *
    from berkeley_meters.meters
    where 
        lower(label) like '%demand%' 
        and lower(label) like '%main%' 
        and lower(units) = 'kw'
);
CALL check_view('q1', ARRAY['id', 'label', 'site', 'units'], 23);

CREATE VIEW q2 AS (
    select 
        id,
        label,
        regexp_replace(site, '\s*(New.*|\(.*\))', '', 'g') AS clean_site,
        units
    from Q1
);
CALL check_view('q2', ARRAY['id', 'label', 'clean_site', 'units'], 23);

CREATE VIEW q3 AS (
    select distinct clean_site
    from q2
);
CALL check_view('q3', ARRAY[ 'clean_site'], 14);

CREATE VIEW q4 AS (
    select q3.clean_site,
    (
        select count(q2.id) 
        from q2
        where q3.clean_site = q2.clean_site
    ) as num_meters,
    count(data.id) as num_data_points
    from q3
    join q2 using (clean_site)
    join data using (id)
    group by q3.clean_site
);
CALL check_view('q4', ARRAY['clean_site', 'num_meters', 'num_data_points'], 13);

CREATE VIEW q5 AS (
    select clean_site
    from q3
    left join q4 using (clean_site)
    where num_data_points is null
);
CALL check_view('q5', ARRAY['clean_site'], 1);

CREATE VIEW q6 AS (
    with gaps as (
        select extract(minute from (time - lag(time) over (partition by id order by time))) as time_diff,
        id
        from data
    )
    select time_diff, id, clean_site as site
    from gaps
    join q2 using (id)
    where time_diff > 16
);
CALL check_view('q6', ARRAY['time_diff', 'id', 'site'], 1);

CREATE VIEW q7 AS (
    SELECT 
        q2.clean_site AS site,
        data.time,
        SUM(data.value) AS value
    FROM data
    JOIN q2 USING (id)
    WHERE q2.clean_site NOT IN (SELECT site FROM q6
        UNION
        SELECT clean_site FROM q5)
    GROUP BY q2.clean_site, data.time
);
CALL check_view('q7', ARRAY['site', 'time', 'value'], 7737);

CREATE VIEW q8 AS (
    WITH site_counts AS (
        SELECT 
            q2.clean_site AS site,
            COUNT(*) FILTER (WHERE data.value <> 0) AS non_zero_count
        FROM data
        JOIN q2 USING (id)
        WHERE q2.clean_site NOT IN (
            SELECT clean_site FROM q5
            UNION
            SELECT site FROM q6
        )
        GROUP BY q2.clean_site
    ),
    stats AS (
        SELECT 
            PERCENTILE_CONT(0.5) WITHIN GROUP (ORDER BY non_zero_count) AS median_count,
            PERCENTILE_CONT(0.25) WITHIN GROUP (ORDER BY non_zero_count) AS q1,
            PERCENTILE_CONT(0.75) WITHIN GROUP (ORDER BY non_zero_count) AS q3
        FROM site_counts
    )
    SELECT site
    FROM site_counts, stats
    WHERE non_zero_count < (q1 - 1.5 * (q3 - q1))
       OR non_zero_count > (q3 + 1.5 * (q3 - q1))
);
CALL check_view('q8', ARRAY['site'], 5);

-- --- put q9 answer in README.txt

-- CREATE VIEW q10 AS (
--     SELECT 'YOUR CODE HERE'
-- );
-- CALL check_view('q10', ARRAY['site', 'weekly_eui'], 7);

-- CREATE VIEW q11 AS (
--     SELECT 'YOUR CODE HERE'
-- );
-- CALL check_view('q11', ARRAY['slope', 'intercept', 'corr'], 1);

--- put q12 answer in README.txt
