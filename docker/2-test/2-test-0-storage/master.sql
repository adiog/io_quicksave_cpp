CREATE TABLE user (
    "user_hash" text NOT NULL PRIMARY KEY,
    "databaseConnectionString" text NOT NULL,
    "password" text NOT NULL,
    "storageConnectionString" text NOT NULL,
    "username" text NOT NULL
);

INSERT INTO user ("user_hash", "username", "password", "databaseConnectionString", "storageConnectionString")
       VALUES('aac33daf5e2bf8841007a49fde63d900b67c31db', 'testuser', 'testpass', 'sqlite:///storage/users/testuser.db', 'file:///storage/users/testuser');
