USE db_Result

CREATE TABLE Страна
	(ID_страны smallint PRIMARY KEY NOT NULL,
	Название varchar(30))

INSERT INTO db_Result..Страна  SELECT * FROM db_Norm..Страна

CREATE TABLE Место_производства
	(ID_места_производства tinyint PRIMARY KEY NOT NULL,
	Название varchar(20))

INSERT INTO db_Result..Место_производства  SELECT * FROM db_Norm..Место_производства

CREATE TABLE Марка
	(ID_марки smallint PRIMARY KEY NOT NULL,
	Название varchar(40),
	ID_страны_происхождения smallint,
	ID_места_происхождения tinyint,
	FOREIGN KEY (ID_страны_происхождения) REFERENCES Страна(ID_страны),
    FOREIGN KEY (ID_места_происхождения) REFERENCES Место_производства (ID_места_производства))

INSERT INTO db_Result..Марка  SELECT * FROM db_Norm..Марка


CREATE TABLE Модель
	(ID_модели int PRIMARY KEY NOT NULL,
	Название varchar(30))

INSERT INTO db_Result..Модель  SELECT * FROM db_Norm..Модель


CREATE TABLE Тип_кузова
	(ID_типа_кузова tinyint PRIMARY KEY NOT NULL,
	Название varchar(50))

INSERT INTO db_Result..Тип_кузова  SELECT * FROM db_Norm..Тип_кузова


CREATE TABLE Тип_топлива
	(ID_типа_топлива tinyint PRIMARY KEY NOT NULL,
	Название varchar(50))

INSERT INTO db_Result..Тип_топлива  SELECT * FROM db_Norm..Тип_топлива


CREATE TABLE Тип_владельца
	(ID_типа_владельца tinyint PRIMARY KEY NOT NULL,
	Название varchar(30))

INSERT INTO db_Result..Тип_владельца  SELECT * FROM db_Norm..Тип_владельца


CREATE TABLE Сегмент_автостата
	(ID_сегмента_автостата tinyint PRIMARY KEY NOT NULL,
	Название varchar(30))

INSERT INTO db_Result..Сегмент_автостата  SELECT * FROM db_Norm..Сегмент_автостата


CREATE TABLE Расположение_руля
	(ID_расположения_руля tinyint PRIMARY KEY NOT NULL,
	Название varchar(20))

INSERT INTO db_Result..Расположение_руля  SELECT * FROM db_Norm..Расположение_руля


CREATE TABLE Округ
	(ID_округа tinyint PRIMARY KEY NOT NULL,
	Название varchar(50))

INSERT INTO db_Result..Округ  SELECT * FROM db_Norm..Округ


CREATE TABLE Район
	(ID_района smallint PRIMARY KEY NOT NULL,
	Название varchar(50))

INSERT INTO db_Result..Район  SELECT * FROM db_Norm..Район


CREATE TABLE PC
	(ID_PC int PRIMARY KEY NOT NULL,
	Срез_парка varchar(20),
	ID_марки smallint,
	FOREIGN KEY (ID_марки) REFERENCES Марка(ID_марки),
	ID_модели int,
	FOREIGN KEY (ID_модели) REFERENCES Модель(ID_модели),
	Год_выпуска varchar(20),
	ID_типа_кузова tinyint,
	FOREIGN KEY (ID_типа_кузова) REFERENCES Тип_кузова(ID_типа_кузова),
	ID_типа_топлива tinyint,
	FOREIGN KEY (ID_типа_топлива) REFERENCES Тип_топлива(ID_типа_топлива),
	Объём_двигателя varchar(10),
	Мощность varchar(10),
	ID_типа_владельца tinyint,
	FOREIGN KEY (ID_типа_владельца) REFERENCES Тип_владельца(ID_типа_владельца),
	Экологический_класс varchar(20),		
	ID_места_производства tinyint,
	FOREIGN KEY (ID_места_производства) REFERENCES Место_производства(ID_места_производства),
	ID_страны_производства smallint,
	FOREIGN KEY (ID_страны_производства) REFERENCES Страна(ID_страны),
	ID_сегмента_автостата tinyint,
	FOREIGN KEY (ID_сегмента_автостата) REFERENCES Сегмент_автостата(ID_сегмента_автостата),
	ID_расположения_руля tinyint,
	FOREIGN KEY (ID_расположения_руля) REFERENCES Расположение_руля(ID_расположения_руля),
	ID_округа tinyint,
	FOREIGN KEY (ID_округа) REFERENCES Округ(ID_округа),
	ID_района smallint,
	FOREIGN KEY (ID_района) REFERENCES Район(ID_района),
	Количество varchar(10))


CREATE TABLE Вид_кузова
	(ID_вида_кузова tinyint PRIMARY KEY NOT NULL,
	Название varchar(50))

INSERT INTO db_Result..Вид_кузова  SELECT * FROM db_Norm..Вид_кузова


CREATE TABLE LCV
	(ID_LCV int PRIMARY KEY NOT NULL,
	Срез_парка varchar(20),
	ID_марки smallint,
	FOREIGN KEY (ID_марки) REFERENCES Марка(ID_марки),
	ID_модели int,
	FOREIGN KEY (ID_модели) REFERENCES Модель(ID_модели),
	Год_выпуска varchar(20),
	ID_типа_кузова tinyint,
	FOREIGN KEY (ID_типа_кузова) REFERENCES Тип_кузова(ID_типа_кузова),
	ID_вида_кузова tinyint,
	FOREIGN KEY (ID_вида_кузова) REFERENCES Вид_кузова(ID_вида_кузова),
	ID_типа_топлива tinyint,
	FOREIGN KEY (ID_типа_топлива) REFERENCES Тип_топлива(ID_типа_топлива),
	Объём_двигателя varchar(20),
	Мощность varchar(20),
	Полная_масса varchar(20),
	Снаряжённая_масса varchar(20),
	ID_типа_владельца tinyint,
	FOREIGN KEY (ID_типа_владельца) REFERENCES Тип_владельца(ID_типа_владельца),
	Экологический_класс varchar(20),		
	ID_места_производства tinyint,
	FOREIGN KEY (ID_места_производства) REFERENCES Место_производства(ID_места_производства),
	ID_страны_производства smallint,
	FOREIGN KEY (ID_страны_производства) REFERENCES Страна(ID_страны),
	ID_сегмента_автостата tinyint,
	FOREIGN KEY (ID_сегмента_автостата) REFERENCES Сегмент_автостата(ID_сегмента_автостата),
	ID_расположения_руля tinyint,
	FOREIGN KEY (ID_расположения_руля) REFERENCES Расположение_руля(ID_расположения_руля),
	ID_округа tinyint,
	FOREIGN KEY (ID_округа) REFERENCES Округ(ID_округа),
	ID_района smallint,
	FOREIGN KEY (ID_района) REFERENCES Район(ID_района),
	Количество varchar(20))


CREATE TABLE HCV
	(ID_HCV int PRIMARY KEY NOT NULL,
	Срез_парка varchar(20),
	ID_марки smallint,
	FOREIGN KEY (ID_марки) REFERENCES Марка(ID_марки),
	ID_модели int,
	FOREIGN KEY (ID_модели) REFERENCES Модель(ID_модели),
	Год_выпуска varchar(20),
	ID_типа_кузова tinyint,
	FOREIGN KEY (ID_типа_кузова) REFERENCES Тип_кузова(ID_типа_кузова),
	ID_вида_кузова tinyint,
	FOREIGN KEY (ID_вида_кузова) REFERENCES Вид_кузова(ID_вида_кузова),
	ID_типа_топлива tinyint,
	FOREIGN KEY (ID_типа_топлива) REFERENCES Тип_топлива(ID_типа_топлива),
	Объём_двигателя varchar(20),
	Мощность varchar(20),
	Полная_масса varchar(20),
	Снаряжённая_масса varchar(20),
	ID_типа_владельца tinyint,
	FOREIGN KEY (ID_типа_владельца) REFERENCES Тип_владельца(ID_типа_владельца),
	Экологический_класс varchar(20),		
	ID_места_производства tinyint,
	FOREIGN KEY (ID_места_производства) REFERENCES Место_производства(ID_места_производства),
	ID_страны_производства smallint,
	FOREIGN KEY (ID_страны_производства) REFERENCES Страна(ID_страны),
	ID_расположения_руля tinyint,
	FOREIGN KEY (ID_расположения_руля) REFERENCES Расположение_руля(ID_расположения_руля),
	ID_округа tinyint,
	FOREIGN KEY (ID_округа) REFERENCES Округ(ID_округа),
	ID_района smallint,
	FOREIGN KEY (ID_района) REFERENCES Район(ID_района),
	Количество varchar(20))


CREATE TABLE BUS
	(ID_BUS int PRIMARY KEY NOT NULL,
	Срез_парка varchar(20),
	ID_марки smallint,
	FOREIGN KEY (ID_марки) REFERENCES Марка(ID_марки),
	ID_модели int,
	FOREIGN KEY (ID_модели) REFERENCES Модель(ID_модели),
	Год_выпуска varchar(20),
	ID_типа_кузова tinyint,
	FOREIGN KEY (ID_типа_кузова) REFERENCES Тип_кузова(ID_типа_кузова),
	ID_типа_топлива tinyint,
	FOREIGN KEY (ID_типа_топлива) REFERENCES Тип_топлива(ID_типа_топлива),
	Объём_двигателя varchar(20),
	Мощность varchar(20),
	Полная_масса varchar(20),
	Снаряжённая_масса varchar(20),
	ID_типа_владельца tinyint,
	FOREIGN KEY (ID_типа_владельца) REFERENCES Тип_владельца(ID_типа_владельца),
	Экологический_класс varchar(20),		
	ID_места_производства tinyint,
	FOREIGN KEY (ID_места_производства) REFERENCES Место_производства(ID_места_производства),
	ID_страны_производства smallint,
	FOREIGN KEY (ID_страны_производства) REFERENCES Страна(ID_страны),
	ID_сегмента_автостата tinyint,
	FOREIGN KEY (ID_сегмента_автостата) REFERENCES Сегмент_автостата(ID_сегмента_автостата),
	ID_расположения_руля tinyint,
	FOREIGN KEY (ID_расположения_руля) REFERENCES Расположение_руля(ID_расположения_руля),
	ID_округа tinyint,
	FOREIGN KEY (ID_округа) REFERENCES Округ(ID_округа),
	ID_района smallint,
	FOREIGN KEY (ID_района) REFERENCES Район(ID_района),
	Количество varchar(20))


CREATE TABLE MT
	(ID_MT int PRIMARY KEY NOT NULL,
	Срез_парка varchar(20),
	ID_марки smallint,
	FOREIGN KEY (ID_марки) REFERENCES Марка(ID_марки),
	ID_модели int,
	FOREIGN KEY (ID_модели) REFERENCES Модель(ID_модели),
	Год_выпуска varchar(20),
	ID_типа_кузова tinyint,
	FOREIGN KEY (ID_типа_кузова) REFERENCES Тип_кузова(ID_типа_кузова),
	ID_типа_топлива tinyint,
	FOREIGN KEY (ID_типа_топлива) REFERENCES Тип_топлива(ID_типа_топлива),
	Объём_двигателя varchar(20),
	Мощность varchar(20),
	ID_типа_владельца tinyint,
	FOREIGN KEY (ID_типа_владельца) REFERENCES Тип_владельца(ID_типа_владельца),
	ID_округа tinyint,
	FOREIGN KEY (ID_округа) REFERENCES Округ(ID_округа),
	ID_района smallint,
	FOREIGN KEY (ID_района) REFERENCES Район(ID_района),
	Количество varchar(20))


