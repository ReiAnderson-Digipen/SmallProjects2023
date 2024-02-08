create database HW1;
use HW1;
create table forumUser(
	userID INT primary key,
    userAlias varchar(80),
    password char(15) ,
    gender ENUM('Male', 'Female', 'Other'),
    userLevel INT default 0,
    registerDate DATE,
    birthday DATE,
    email varchar(80)
);

create table forumSection(
	sectionID INT primary key,
    sectionName varchar(80),
    postAmount INT,
    clickCount INT
);

create table forumPost(
	sectionID INT,
    postID INT,
    poster varchar(80),
    status ENUM('unlocked', 'locked'),
    clickCount INT,
    postDate DATE,
    lastReply DATE,
    lastReplyUser varchar(80),
    replyCount INT,
    hitRate FLOAT,
    primary key (postID, sectionID)
);

create table postReply(
	sectionID INT,
    postID INT,
    replyCount INT,
    poster varchar(80),
    clickCount INT,
    lastReply DATE,
    lastReplyUser varchar(80)
);

create table forumVisitor(
	ipAdress varchar(15),
    name varchar(80),
    gender ENUM('Male', 'Female', 'Other'),
    favoriteSection varchar(80)
);

create table administrator(
	adminID INT primary key,
    name varchar(80),
    password varchar(15)
);

create table operate(
	adminID INT,
    sectionID INT,
    sectionDeletion tinyint,
    sectionPublish tinyint,
	sectionModification tinyint
);

create table visit(
	userID INT,
    sectionID INT,
    visitTime TIMESTAMP
);

create table browse(
	ipAddress varchar(15),
    sectionID INT,
    postID INT,
    browseTime TIMESTAMP
);







