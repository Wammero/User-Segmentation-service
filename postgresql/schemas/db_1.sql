CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE segments (
    segment_id SERIAL PRIMARY KEY,
    segment_name VARCHAR(50) NOT NULL,
    percentage INT NOT NULL CHECK (percentage >= 0 AND percentage <= 100),
    UNIQUE(segment_name)
);

CREATE TABLE user_segments (
    user_segment_id SERIAL PRIMARY KEY,
    user_id INT NOT NULL,
    segment_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (segment_id) REFERENCES segments(segment_id),
    UNIQUE(user_id, segment_id) 
);