use hw1;

DELIMITER //

create FUNCTION contains_digit(number INT, digit INT)
returns tinyint
begin
while number != 0 do
	if MOD(number, 10) = digit then
		return 1;
	end if;
    SET number = number/10;
    
end while;
return 0;
end//

DELIMITER ;

SELECT * from forumPost where postDate > '2003-03-01' into outfile 'first.txt';
select postID from forumPost where clickCount > 10 into outfile 'second.txt';
select userID from visit where DATE(visitTime) >= '2003-09-22' OR contains_digit(sectionID, 1) into outfile 'third.txt';
select postID from forumPost where MOD(sectionID, 10) = 1 into outfile 'fourth.txt';
select userLevel from forumUser where birthday >= '1995-01-13' or registerDate < '2016-10-19' into outfile 'fith.txt';

-- wasnt sure how to save all of these to a file so i combined them all after the fact! thank you
