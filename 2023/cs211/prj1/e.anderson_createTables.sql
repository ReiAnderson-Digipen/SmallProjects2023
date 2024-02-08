
DROP DATABASE IF EXISTS Imstagram;
CREATE DATABASE IF NOT EXISTS Imstagram;

use Imstagram;

create table Users(
	userID INT primary key NOT NULL,
    firstName varchar(80) NOT NULL,
    lastName varchar(80),
    dob DATE,
    gender enum('Male', 'Female', 'Other')

);
create table ProfileInformation(
	userID INT UNIQUE NOT NULL,
    profileID INT UNIQUE NOT NULL primary key,
    hometown varchar(80),
    currentLocation varchar(80),
    EducationHistory varchar(80)
);
create table Friendship(
	friendshipID INT unique not null primary key,
    friend1 INT not NULL,
    friend2 INT not null
);

create table Photo(
	photoID INT unique NOT NULL primary key,
    ownerID INT NOT NULL,
    albumID INT NOT NULL,
	caption varchar(80),
    timeCreated timestamp,
    timeModified timestamp,
    link varchar(80)
);
create table Tags(
	tagID INT unique not null primary key,
    taggedUser int not null,
    photoID int not null
);
create table Album(
	albumID INT unique not null primary key,
    ownerID INT not null,
    albumName varchar(80),
    coverPhoto INT not null, 
    timeCreated TIMESTAMP,
    link varchar(80),
    albumVisibility enum('Public', 'Private')

);

create table Message(
	messageID INT unique not NULL primary key,
	senderID INT not NULL,
    recieverID INT not NULL,
    messageBody varchar(1000)
);

create table Event(
	eventID INT unique not null primary key,
    creatorID INT not NULL,
    hostID INT not NULL,
    eventName varchar(80),
    eventDescription varchar(256),
    eventState varchar(80),
    eventCountry varchar(80),
    startTime datetime,
    endTime datetime
);
create table EventParticipant(
	userID INT not null,
    eventID INT not null,
    primary key(userID, eventID)
);

