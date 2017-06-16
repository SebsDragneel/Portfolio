use day1;

select firstname, lastname from users;

select username, password from users;

select * from users where gender = "F";


 
select * from useremailtype;

insert into users
(userId, firstname, lastname, middlename, username, password, DOB, gender, occupationId, userStatusId, userTypeId)
values ('A', 'Sebastian', 'DiazPortillo', '---', 'SebsDragneel', 'Ulqui0rra', '1997-01-10',  'M', 455, 1, 1);

select * from users where username = "SebsDragneel";

insert into useremail
(userId, userEmail, userEmailTypeId)
values ('A', 'sebs_dragneel@hotmail.com', 1);
insert into useremail
(userId, userEmail, userEmailTypeId)
values ('A', 'sebs.dragneel@gmail.com', 2);
insert into userphone
(userId, userPhone, userPhoneTypeId)
values ('A', '+14079694735', 1);
insert into userphone
(userId, userPhone, userPhoneTypeId)
values ( 'A', '+5215532152535', 2);


select users.userId, lastname, firstname, userEmail from users
join userEmail
 on users.userId = userEmail.userId
 where users.userid = 'A';
 
 select users.userId, lastname, firstname, userPhone, userPhoneType from users
join userPhone
 on users.userId = userPhone.userId
 join userPhoneType
 on userPhone.userPhoneTypeId = userPhoneType.userPhoneTypeId
 where users.userId = 'A';
 
 update users set
lastname = 'Diaz'
where userId = 'A';

select * from users
where userId = 'A';

delete from userPhone
where userID = 'A' and userPhone = '+14079694735';

update occupation set
occupation = 'Computer Software Engineers'
where occupationId = 455;

insert into userType
(userTypeId, userType)
values (50000000, 'Totally Awesome');

 update users set
userTypeId = 50000000
where userId = 'A';

-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


insert into users
(userId, firstname, lastname, middlename, username, password, DOB, gender, occupationId, userStatusId, userTypeId)
values ('B', 'Sasuke', 'Uchiha', '---', 'DarkHokage', 'Il0veNarut0', '1996-01-10',  'M', 456, 1, 1);

insert into orders
(userId, orderDate , shippingDate)
values ('B', '2016-09-29 13:57:18', '2016-09-30 16:00:00');
insert into orders
(userId, orderDate , shippingDate)
values ('B', '2016-09-29 14:50:18', '2016-10-01 16:00:00');
insert into orderitem
(orderId, itemId, quantity)
values (21278, 1, 1);
insert into orderitem
(orderId, itemId, quantity)
values ( 21279,2, 1);

insert into userdvd
(userId, dvdId)
values ('B',7);
insert into userdvd
(userId, dvdId)
values ('B',13);

insert into uservehicle
(userId, vehicleId)
values ('B',10000);
insert into uservehicle
(userId, vehicleId)
values ('B',32071);



select users.userId, firstname, lastname, orders.orderId from users
join orders
 on users.userId = orders.userId;
 
 select * from users
left join orders
 on users.userId = orders.userId
 limit 200;
 
 
 select users.userId, firstname, lastname from users
left join orders
 on  users.userId = orders.userId
 where orders.orderId is null
 limit 200;
 
  select orders.orderId, orders.orderDate, users.userId, firstname, lastname, itemName, quantity from users
 join orders
 on  users.userId = orders.userId
 join orderitem
 on orders.orderId = orderitem.orderId
 join item
 on orderitem.itemId = item.itemId
 limit 200;

ALTER TABLE orders
  ADD destinationState VARCHAR(2);
  update orders
set destinationState =
(
select state
    from state
    order by state asc
    limit 1
);
  
  ALTER TABLE orders
  ADD  constraint fk_destinationState
  foreign key(destinationState)
  references state(state);
  
  insert into activitylog(acivityDate, activityText, userId, orderId)
  values (2016-09-29, 'order placed', 'B', 19);
  
  drop view if exists destinationState;
  create view destinationState as 
  select userid.username from users limit 10;
  
Alter table users
add index (lastname);

select concat(firstname, ' ', lastname) as fullname from users
where lastname = 'Steady';

select count(*) from users
where lastname like '%ing';


select count(*) as Employed from users
where firstname like 'J%' and occupationId is not null;


-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

select avg(mpgHighway) from vehicle;

select make, model, avg(mpgCity) as Av from vehicle where mpgCity > 0 group by model order by  Av limit 10;

select make, count(model), year from vehicle where year = 1997 group by model order by make;

select make, count(model) as models, year from vehicle where year = 2002  group by model having models > 4 order by make;

drop table if exists vehicleMake;
create table vehicleMake (
	MakeId int(11) NOT NULL auto_increment,
 	make varchar(255) DEFAULT NULL,
  PRIMARY KEY (MakeId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

drop table if exists vehicleModel;
create table vehicleModel (
	ModelId int(11) NOT NULL auto_increment,
 	model varchar(255) DEFAULT NULL,
  PRIMARY KEY (ModelId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

drop table if exists vehicleDrive;
create table vehicleDrive (
	DriveId int(11) NOT NULL auto_increment,
 	drive varchar(255) DEFAULT NULL,
  PRIMARY KEY (DriveId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

drop table if exists vehicleFuelType;
create table vehicleFuelType (
	FuelId int(11) NOT NULL auto_increment,
 	fuelType varchar(255) DEFAULT NULL,
  PRIMARY KEY (FuelId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



CREATE UNIQUE INDEX indexMake
ON vehicleMake (make);

CREATE UNIQUE INDEX indexmodel
ON vehicleModel (model);

CREATE UNIQUE INDEX indexdrive
ON vehicleDrive (drive);

CREATE UNIQUE INDEX indexfuelType
ON vehicleFuelType (fuelType);

insert into vehicleMake(make)
select distinct(make)
from vehicle;

insert into vehicleModel(model)
select distinct(model)
from vehicle;

insert into vehicleDrive(drive)
select distinct(drive)
from vehicle;

insert into vehicleFuelType(fuelType)
select distinct(fuelType)
from vehicle;


drop table if exists vehicleNormal;
create table vehicleNormal (
vehicleId int(11) not null,
 makeId int(11) DEFAULT NUll,
 modelId int(11) DEFAULT NUll,
 Year int(11) DEFAULT NUll,
 cylinders varchar(255) DEFAULT NULL,
 driveId int(11) DEFAULT NUll,
 mpgHighway decimal(11, 4) DEFAULT NUll,
 mpgCity decimal(11, 4) DEFAULT NUll,
 fuelTypeId int(11) DEFAULT NUll,
  PRIMARY KEY (vehicleId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


CREATE  INDEX indexMake
ON vehicle (make);

CREATE  INDEX indexmodel
ON vehicle (model);

CREATE  INDEX indexdrive
ON vehicle (drive);

CREATE INDEX indexfuelType
ON vehicle (fuelType);



insert into vehicleNormal(vehicleId, makeId,  modelId, Year, cylinders, driveId, mpgHighway, mpgCity, fuelTypeId)
select vehicle.vehicleId, vehicleMake.MakeId, vehicleModel.ModelId, vehicle.year, vehicle.cylinders, vehicledrive.DriveId, vehicle.mpgHighway, vehicle.mpgCity, vehiclefueltype.FuelId 
from vehicle
join vehicleMake on vehicle.make = vehiclemake.make
join vehicleModel on vehicle.model = vehicleModel.model
join vehicleDrive on vehicle.drive = vehicleDrive.drive
join vehicleFuelType on vehicle.fuelType = vehicleFuelType.fuelType;

select vehicleId, vehiclenormal.makeId, vehiclenormal.modelId, year, vehiclenormal.driveId, vehiclenormal.fueltypeId from vehiclenormal
join vehicleMake on vehiclenormal.makeId = vehiclemake.makeId
join vehicleModel on vehiclenormal.modelId = vehicleModel.modelId
join vehicleDrive on vehiclenormal.driveId = vehicleDrive.driveId
join vehicleFuelType on vehiclenormal.fuelTypeId = vehicleFuelType.fuelId;


-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


select state, count(state) from useraddress group by state;

select DVD_Title, count(Released), Year from dvd group by Year order by Year desc limit 10;

select avg(publicRating), DVD_Title, Year from dvd  where year = 2002 group by Genre having avg(publicRating) > 2.0;

select avg(datediff(now(), dob)/ 365) as Age, gender from users where occupationId = 164 and gender = 'M';


drop table if exists DVDNormal;
create table DVDNormal (
dvdId int(11) not null,
 dvdTitle varchar(255) DEFAULT NULL,
 Year varchar(255) DEFAULT NUll,
 publicRating decimal(4,2) DEFAULT NULL,
 dvdStudioId int(11) DEFAULT NUll,
 dvdStatusId int(11) DEFAULT NUll,
 dvdGenreId int(11) DEFAULT NUll,
 
  PRIMARY KEY (dvdId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


drop table if exists dvdStudio;
create table dvdStudio (
	dvdStudioId int(11) NOT NULL auto_increment,
 	Studio varchar(255) DEFAULT NULL,
  PRIMARY KEY (dvdStudioId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

drop table if exists dvdStatus;
create table dvdStatus (
	dvdStatusId int(11) NOT NULL auto_increment,
 	Status varchar(255) DEFAULT NULL,
  PRIMARY KEY (dvdStatusId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

drop table if exists dvdGenre;
create table dvdGenre (
	dvdGenreId int(11) NOT NULL auto_increment,
 	Genre varchar(255) DEFAULT NULL,
  PRIMARY KEY ( dvdGenreId)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


ALTER TABLE dvdNormal
  ADD  constraint fk_dvdStudio
  foreign key(dvdStudioId)
  references dvdstudio(dvdStudioId)
  on update cascade
  on delete cascade;
  
  ALTER TABLE dvdNormal
  ADD  constraint fk_dvdStatus
  foreign key(dvdStatusId)
  references dvdstatus(dvdStatusId)
  on update cascade
  on delete cascade;
  
  ALTER TABLE dvdNormal
  ADD  constraint fk_dvdGenre
  foreign key(dvdGenreId)
  references dvdGenre(dvdGenreId)
  on update cascade
  on delete cascade;

CREATE UNIQUE INDEX dvdStudioindex
ON dvdStudio (Studio);

CREATE UNIQUE INDEX dvdStatusindex
ON dvdStatus (Status);

CREATE UNIQUE INDEX dvdGenreindex
ON dvdGenre (Genre);


CREATE  INDEX indexMake
ON dvd (studio);

CREATE  INDEX indexmodel
ON dvd (status);

CREATE  INDEX indexdrive
ON dvd (genre);

insert into dvdstudio(Studio)
select distinct(Studio)
from dvd;

insert into dvdstatus(Status)
select distinct(Status)
from dvd;

insert into dvdgenre(Genre)
select distinct(Genre)
from dvd;


insert into dvdNormal(dvdId, dvdTitle, Year, publicRating, dvdStudioId, dvdStatusId, dvdGenreId)
select dvd.dvdId, dvd.DVD_Title, dvd.Year, dvd.publicRating, dvdstudio.dvdStudioId,  dvdstatus.dvdStatusId, dvdgenre.dvdGenreId 
from dvd
join dvdstudio on dvd.Studio = dvdstudio.Studio
join dvdstatus on dvd.Status = dvdstatus.Status
join dvdgenre on dvd.Genre = dvdgenre.Genre;


CREATE VIEW view_dvds AS
SELECT dvdId, dvdtitle, dvdstudio.Studio, dvdstatus.Status, dvdgenre.Genre
FROM dvdnormal
join dvdstudio on dvdnormal.dvdStudioId = dvdstudio.dvdStudioId
join dvdstatus on dvdnormal.dvdStatusId = dvdstatus.dvdStatusId
join dvdgenre on dvdnormal.dvdGenreId = dvdgenre.dvdGenreId;

select UserId, count(*) as dvdcount from userdvd group by userId order by dvdcount desc;


select    users.userId, firstname, lastname, dvdcount, gender  from users  join ( SELECT userId, count(*) as dvdcount
FROM userdvd group by userId )as OC on  OC.userId = users.userId where gender = 'F' order by dvdcount desc limit 1;


-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


alter table item
add column quantityInStock Int(11);
update item set quantityInStock = 10;

drop trigger if exists ReduceQuantityinStock;
delimiter $$

CREATE TRIGGER ReduceQuantityinStock
AFTER INSERT ON orderitem
FOR EACH ROW 
BEGIN
update item
set item.quantityInStock = item.quantityInStock - new.quantity
where item.itemId = new.itemId;
END$$

delimiter ;

select  itemId, itemName, quantityInStock from item where itemId = 13 or itemId = 18;

insert into orderitem (orderId, itemId, quantity)
values (0000005, 13, 2);
insert into orderitem (orderId, itemId, quantity)
values (0000006, 18, 2);

select  itemId, itemName, quantityInStock from item where itemId = 13 or itemId = 18;


drop trigger if exists UserStatusChange;
delimiter $$

CREATE TRIGGER UserStatusChange
AFTER update ON users
FOR EACH ROW 
BEGIN
if new.userStatusId != old.userStatusId then 
insert into activityLog (activitydate, activitytext, userId)
values (now(), 'User Status Changed', new.userId);
end if;
END$$

delimiter ;

drop table if exists numbers;
CREATE TABLE numbers (
  n int(11) NOT NULL,
  PRIMARY KEY (n)
) ENGINE=INNODB DEFAULT CHARSET=latin1;

INSERT INTO numbers (n)
VALUES
	(1),
	(2),
	(3),
	(4),
	(5),
	(6),
	(7),
	(8),
	(9),
	(10),
	(11),
	(12),
	(13),
	(14),
	(15),
	(16),
	(17),
	(18),
	(19),
	(20),
	(21),
	(22),
	(23),
	(24),
	(25),
	(26),
	(27),
	(28),
	(29),
	(30),
	(31),
	(32),
	(33),
	(34),
	(35),
	(36),
	(37),
	(38),
	(39),
	(40),
	(41),
	(42),
	(43),
	(44),
	(45),
	(46),
	(47),
	(48),
	(49),
	(50),
	(51),
	(52),
	(53),
	(54),
	(55),
	(56),
	(57),
	(58),
	(59),
	(60),
	(61),
	(62),
	(63),
	(64),
	(65),
	(66),
	(67),
	(68),
	(69),
	(70),
	(71),
	(72),
	(73),
	(74),
	(75),
	(76),
	(77),
	(78),
	(79),
	(80),
	(81),
	(82),
	(83),
	(84),
	(85),
	(86),
	(87),
	(88),
	(89),
	(90),
	(91),
	(92),
	(93),
	(94),
	(95),
	(96),
	(97),
	(98),
	(99),
	(100);
    
    
    select n,
    (select count(*) from dvd
    where price > n - 1  and  price <= n
    ) as Total
from numbers;
    
SELECT 	n - 1,  
	CASE 
		when s.cylinders is null THEN 0
        else s.cylinders
	end as helloworld, c
from numbers
left join (SELECT cylinders, count(*) as c
from 	vehicle
where vehicle.year = 1990
group by vehicle.cylinders)00 s on s.cylinders = n-1

SELECT  vehicle.cylinders,	count(*)
from 	vehicle

where vehicle.year = 1990
group by  vehicle.cylinders

) as orderSum on vehicle.cylinders = 
where 	n >= 0 and n <= 16;

select n-1, CASE 
		when s.cylinders is null THEN 0
        else s.cylinders
	end as helloworld, c
from numbers
left join
(select cylinders, count(*) as c
from vehicle
where vehicle.year = 1990
group by vehicle.cylinders) s on s.cylinders = n-1
where n >= 0 and n <= 16;

select n-1, CASE 
		when s.cylinders is null THEN 0
        else s.cylinders
	end as helloworld, c
from numbers
left join
(select cylinders, count(*) as c
from vehicle
where vehicle.year = 2012
group by vehicle.cylinders) s on s.cylinders = n-1
where n >= 0 and n <= 16;

-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

select avg(mpgHighway) as mpgH, avg(mpgCity) as mpgC from vehicle group by fuelType order by mpgC desc; 

select * from vehicle where cylinders = 3;
delete from vehicle where cylinders = 3;
delete from uservehicle where vehicleId in 
(select vehicleId from vehicle where cylinders = 3);
delete from vehicle where cylinders = 3;


delimiter $$
drop function if exists getItemsInStock $$

CREATE FUNCTION getItemsInStock(fId int(11)) 
RETURNS int
READS SQL DATA
BEGIN
	declare r int;
	SELECT quantityInStock into r FROM item WHERE itemId = fId;
	return(r);
END$$

delimiter ; 

SELECT itemId, 	getItemsInStock(itemId) AS InStock
FROM item limit 100;

SET GLOBAL log_bin_trust_function_creators = 1;
delimiter $$
create function getTotal(oId INT(11))
returns int(11) deterministic
begin
  declare total int(11);
  set total = (
    select sum(b.itemPrice * a.quantity) from orderitem a
    join item b on a.itemId = b.itemId
    where orderId = oId
  );
  return total;
end$$
delimiter ;

select getTotal(13) as sum;



select * from users
where userID = 'A';
delete from orders
where userID = 'B';