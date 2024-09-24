-- Вставка пользователей
INSERT INTO users (username, email) VALUES
('Alice', 'alice@example.com'),
('Bob', 'bob@example.com'),
('Charlie', 'charlie@example.com'),
('David', 'david@example.com'),
('Eva', 'eva@example.com'),
('Frank', 'frank@example.com'),
('Grace', 'grace@example.com'),
('Heidi', 'heidi@example.com'),
('Ivan', 'ivan@example.com'),
('Judy', 'judy@example.com'),
('Karl', 'karl@example.com'),
('Leo', 'leo@example.com'),
('Mallory', 'mallory@example.com'),
('Nancy', 'nancy@example.com'),
('Olivia', 'olivia@example.com'),
('Peggy', 'peggy@example.com'),
('Quentin', 'quentin@example.com'),
('Ruth', 'ruth@example.com'),
('Sybil', 'sybil@example.com'),
('Trent', 'trent@example.com');

-- Вставка сегментов
INSERT INTO segments (segment_name, percentage) VALUES
('MAIL_VOICE_MESSAGES', 50),
('CLOUD_DISCOUNT_30', 20),
('MAIL_GPT', 30);

-- Вставка пользовательских сегментов 
INSERT INTO user_segments (user_id, segment_id) VALUES
(1, 1), -- Alice -> MAIL_VOICE_MESSAGES
(2, 1), -- Bob -> MAIL_VOICE_MESSAGES
(3, 2), -- Charlie -> CLOUD_DISCOUNT_30
(4, 3), -- David -> MAIL_GPT
(5, 3), -- Eva -> MAIL_GPT
(6, 2), -- Frank -> CLOUD_DISCOUNT_30
(7, 1), -- Grace -> MAIL_VOICE_MESSAGES
(8, 3), -- Heidi -> MAIL_GPT
(9, 2), -- Ivan -> CLOUD_DISCOUNT_30
(10, 1), -- Judy -> MAIL_VOICE_MESSAGES
(11, 1), -- Karl -> MAIL_VOICE_MESSAGES
(12, 3), -- Leo -> MAIL_GPT
(13, 2), -- Mallory -> CLOUD_DISCOUNT_30
(14, 1), -- Nancy -> MAIL_VOICE_MESSAGES
(15, 3), -- Olivia -> MAIL_GPT
(16, 1), -- Peggy -> MAIL_VOICE_MESSAGES
(17, 2), -- Quentin -> CLOUD_DISCOUNT_30
(18, 3), -- Ruth -> MAIL_GPT
(19, 1), -- Sybil -> MAIL_VOICE_MESSAGES
(20, 3); -- Trent -> MAIL_GPT