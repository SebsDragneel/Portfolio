# ************************************************************
# Sequel Pro SQL dump
# Version 4096
#
# http://www.sequelpro.com/
# http://code.google.com/p/sequel-pro/
#
# Host: 127.0.0.1 (MySQL 5.1.44)
# Database: cableListing
# Generation Time: 2014-02-05 18:24:03 +0000
# ************************************************************


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


# Dump of table cableRawData
# ------------------------------------------------------------

DROP TABLE IF EXISTS `cableRawData`;

CREATE TABLE `cableRawData` (
  `show` varchar(50) DEFAULT NULL,
  `station` varchar(50) DEFAULT NULL,
  `genre` varchar(50) DEFAULT NULL,
  `premierDate` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

LOCK TABLES `cableRawData` WRITE;
/*!40000 ALTER TABLE `cableRawData` DISABLE KEYS */;

INSERT INTO `cableRawData` (`show`, `station`, `genre`, `premierDate`)
VALUES
	('Agent\'s Of Shield','ABC','Action / Adventure','2013-09-24'),
	('NCIS','CBS','Action / Adventure','2003-09-23'),
	('Walking Dead','AMC','Horror','2010-10-31'),
	('Good Luck Charlie','Disney','Family','2010-04-04'),
	('Supernatural','CW','Horror','2005-09-13'),
	('Sports Center','ESPN','Sports','1979-09-06'),
	('Anderson Cooper 360','CNN','News','2003-09-08'),
	('The Big Bang Theory','CBS','Comedy','2007-09-24'),
	('Scandal','ABC','Drama','2012-04-05'),
	('Tosh .0','Comedy','Comedy','2009-06-04'),
	('Law & Order: SVU ','NBC','Drama','1999-09-20'),
	('Family Guy','Fox','Comedy','1999-01-31'),
	('Once Upon a Time','ABC','Drama','2011-10-23'),
	('Continuum','SyFy','Sci-fi','2012-05-27');

/*!40000 ALTER TABLE `cableRawData` ENABLE KEYS */;
UNLOCK TABLES;



/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
