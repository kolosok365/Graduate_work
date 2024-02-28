import pandas as pd
import pyodbc
import numpy as np

ID_PC = 0
ID_LCV = 0
ID_HCV = 0
ID_BUS = 0
ID_MT = 0
ID_TR = 0

srez = ""
Tip_TS = ""
key_brand = 0
key_model = 0
year_of_release = 0
body_type = ""
fuel_type = ""
engine_volume = ""
engine_power = ""
owner_type = ""
environmental_class = ""
place_of_production = ""
country_of_production = ""
origin_of_the_brand = ""
country_of_origin_of_the_brand = ""
segment = ""
steering_wheel_type = ""
district = ""
area = ""
count = ""
body_kind = ""
total_weight = ""
curb_weight = ""
trailer_type = ""
trailer_body_type = ""

def make_query(query, Tip_TS, has_gaps):
    if has_gaps == False:
        result_query = "INSERT INTO db_Norm.dbo." + Tip_TS + query
        dbCursor.execute(result_query)
    else:
        result_query = "INSERT INTO db_Gaps.dbo." + Tip_TS + query
        dbCursor.execute(result_query)



def reset():
    global srez
    global Tip_TS
    global key_brand
    global key_model
    global year_of_release
    global body_type
    global fuel_type
    global engine_volume
    global engine_power
    global owner_type
    global environmental_class
    global place_of_production
    global country_of_production
    global origin_of_the_brand
    global country_of_origin_of_the_brand
    global segment
    global steering_wheel_type
    global district
    global area
    global count
    global body_kind
    global total_weight
    global curb_weight
    global trailer_type
    global trailer_body_type
    
    srez = ""
    Tip_TS = ""
    key_brand = ""
    key_model = ""
    year_of_release = ""
    body_type = ""
    fuel_type = ""
    engine_volume = ""
    engine_power = ""
    owner_type = ""
    environmental_class = ""
    place_of_production = ""
    country_of_production = ""
    origin_of_the_brand = ""
    country_of_origin_of_the_brand = ""
    segment = ""
    steering_wheel_type = ""
    district = ""
    area = ""
    count = ""
    number_of_axes = ""
    number_of_tires = ""
     

def fill(path, sheet, connection, cursor):
    global ID_PC
    global ID_LCV
    global ID_HCV
    global ID_BUS
    global ID_MT
    global ID_TR
   
    data = pd.read_excel(path, sheet, na_filter = False)
    print("Открыл!")
    has_gaps = False

    for index, row in data.iterrows():
        reset()
        has_gaps = False
        for column in data.columns:
            if row[column] == '':
                has_gaps = True               
            if column == 'Срез парка':
                if row[column] == '':
                    srez = 'NULL'
                    continue
                srez = row['Срез парка']
            elif column == 'Тип ТС':
                if row[column] == '':
                    Tip_TS = 'NULL'
                    continue
                Tip_TS = row['Тип ТС']
            elif column == 'Марка':
                if row[column] == '':
                    key_brand = 'NULL'
                    continue
                sql_query = f"SELECT ID_марки FROM db_Norm.dbo.Марка WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                key_brand = cursor.fetchone()[0]
            elif column == 'Модель':
                if row[column] == '':
                    key_model = 'NULL'
                    continue
                sql_query = f"SELECT ID_модели FROM db_Norm.dbo.Модель WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                key_model = cursor.fetchone()[0]
            elif column == 'Год выпуска':
                if row[column] == '':
                    year_of_release = 'NULL'
                    continue
                year_of_release = row['Год выпуска']   
            elif column == 'Тип кузова':
                if row[column] == '':
                    body_type = 'NULL'
                    continue
                sql_query = f"SELECT ID_типа_кузова FROM db_Norm.dbo.Тип_кузова WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                body_type = cursor.fetchone()[0]
            elif column == 'Тип топлива':
                if row[column] == '':
                    fuel_type = 'NULL'
                    continue
                sql_query = f"SELECT ID_типа_топлива FROM db_Norm.dbo.Тип_топлива WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                fuel_type = cursor.fetchone()[0]
            elif column == 'Объем двигателя, см3':
                if row[column] == '':
                    engine_volume = 'NULL'
                    continue
                engine_volume = row['Объем двигателя, см3']
            elif column == 'Мощность двигателя, л.с.':
                if row[column] == '':
                    engine_power = 'NULL'
                    continue
                engine_power = row['Мощность двигателя, л.с.']
            elif column == 'Тип владельца':
                if row[column] == '':
                    owner_type = 'NULL'
                    continue
                sql_query = f"SELECT ID_типа_владельца FROM db_Norm.dbo.Тип_владельца WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                owner_type = cursor.fetchone()[0]
            elif column == 'Экологический класс':
                if row[column] == '':
                    environmental_class = 'NULL'
                    continue
                environmental_class = row['Экологический класс']
            elif column == 'Место производства':
                if row[column] == '':
                    place_of_production = 'NULL'
                    continue
                sql_query = f"SELECT ID_места_производства FROM db_Norm.dbo.Место_производства WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                place_of_production = cursor.fetchone()[0]
            elif column == 'Страна производства':
                if row[column] == '':
                    country_of_production = 'NULL'
                    continue
                sql_query = f"SELECT ID_страны FROM db_Norm.dbo.Страна WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                country_of_production = cursor.fetchone()[0]
            elif column == 'Происхождение марки':
                if row[column] == '':
                    origin_of_the_brand = 'NULL'
                    continue
                sql_query = f"SELECT ID_места_производства FROM db_Norm.dbo.Место_производства WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                origin_of_the_brand = cursor.fetchone()[0]
            elif column == 'Страна происхождения марки':
                if row[column] == '':
                    country_of_origin_of_the_brand = 'NULL'
                    continue
                sql_query = f"SELECT ID_страны FROM db_Norm.dbo.Страна WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                country_of_origin_of_the_brand = cursor.fetchone()[0]
            elif column == 'Сегмент Автостат':
                if row[column] == '':
                    segment = 'NULL'
                    continue
                sql_query = f"SELECT ID_сегмента_автостата FROM db_Norm.dbo.Сегмент_автостата WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                segment = cursor.fetchone()[0]
            elif column == 'Тип расположения руля':
                if row[column] == '':
                    steering_wheel_type = 'NULL'
                    continue
                sql_query = f"SELECT ID_расположения_руля FROM db_Norm.dbo.Расположение_руля WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                steering_wheel_type = cursor.fetchone()[0]
            elif column == 'Округ':
                if row[column] == '':
                    district = 'NULL'
                    continue
                sql_query = f"SELECT ID_округа FROM db_Norm.dbo.Округ WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                district = cursor.fetchone()[0]
            elif column == 'Район':
                if row[column] == '':
                    area = 'NULL'
                    continue
                sql_query = f"SELECT ID_района FROM db_Norm.dbo.Район WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                area = cursor.fetchone()[0]
            elif column == 'Количество':
                if row[column] == '':
                    count = 'NULL'
                    continue
                count = row['Количество']
            elif column == 'Вид кузова':
                if row[column] == '':
                    body_kind = 'NULL'
                    continue
                sql_query = f"SELECT ID_вида_кузова FROM db_Norm.dbo.Вид_кузова WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                body_kind = cursor.fetchone()[0]
            elif column == 'Полная масса, кг':
                if row[column] == '':
                    total_weight = 'NULL'
                    continue
                total_weight = row['Полная масса, кг']
            elif column == 'Снаряженная масса, кг':
                if row[column] == '':
                    curb_weight = 'NULL'
                    continue
                curb_weight = row['Снаряженная масса, кг']
            elif column == 'Тип прицепа':
                if row[column] == '':
                    trailer_type = 'NULL'
                    continue
                sql_query = f"SELECT ID_типа_прицепа FROM db_Norm.dbo.Тип_прицепа WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                trailer_type = cursor.fetchone()[0]
            elif column == 'Тип кузова прицепа':
                if row[column] == '':
                    trailer_body_type = 'NULL'
                    continue
                sql_query = f"SELECT ID_типа_кузова_прицепа FROM db_Norm.dbo.Тип_кузова_прицепа WHERE Название = '{row[column]}'"
                cursor.execute(sql_query)
                trailer_body_type = cursor.fetchone()[0]
            elif column == 'Количество осей прицепа':
                if row[column] == '':
                    number_of_axes = 'NULL'
                    continue
                number_of_axes = row['Количество осей прицепа']
            elif column == 'Количество шин прицепа':
                if row[column] == '':
                    number_of_tires = 'NULL'
                    continue
                number_of_tires = row['Количество шин прицепа']
               
                                                                                                                                                                                                                                                                                                                      
        if Tip_TS == 'PC':
            ID_PC = ID_PC + 1
            if ID_PC == 768:
                print("n")
            query = "(ID_" + Tip_TS + ", Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность, ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа, ID_района, Количество) VALUES(" + str(ID_PC) + ", \'" + str(srez) + "\', " + str(key_brand) + ", " + str(key_model) + ", \'" + str(year_of_release) + "\', " + str(body_type) + ", " + str(fuel_type) + ", \'" + str(engine_volume) + "\', \'" + str(engine_power) + "\', " + str(owner_type) + ", \'" + str(environmental_class)  + "\', " + str(place_of_production) + ", " + str(country_of_production) + ", " + str(segment) + ", " + str(steering_wheel_type) + ", " + str(district) + ", " + str(area) + ", \'" + str(count) +"\')" 
            make_query(query, Tip_TS, has_gaps) 
        elif Tip_TS == 'LCV':
            ID_LCV = ID_LCV + 1
            query = "(ID_" + Tip_TS + ", Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_вида_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса, ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа, ID_района, Количество) VALUES(" + str(ID_LCV) + ", \'" + str(srez) + "\', " + str(key_brand) + ", " + str(key_model) + ", \'" + str(year_of_release) + "\', " + str(body_type) + ", " + str(body_kind) + ", " + str(fuel_type) + ", \'" + str(engine_volume) + "\', \'" + str(engine_power) + "\', \'" + str(total_weight) + "\', \'" + str(curb_weight)+ "\', " + str(owner_type) + ", \'" + str(environmental_class)  + "\', " + str(place_of_production) + ", " + str(country_of_production) + ", " + str(segment) + ", " + str(steering_wheel_type) + ", " + str(district) + ", " + str(area) + ", \'" + str(count) +"\')" 
            make_query(query, Tip_TS, has_gaps)
        elif Tip_TS == 'BUS':
            ID_BUS = ID_BUS + 1
            query = "(ID_" + Tip_TS + ", Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса, ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа, ID_района, Количество) VALUES(" + str(ID_BUS) + ", \'" + str(srez) + "\', " + str(key_brand) + ", " + str(key_model) + ", \'" + str(year_of_release) + "\', " + str(body_type) + ", " + str(fuel_type) + ", \'" + str(engine_volume) + "\', \'" + str(engine_power) + "\', \'" + str(total_weight) + "\', \'" + str(curb_weight)+ "\', " + str(owner_type) + ", \'" + str(environmental_class) + "\', " + str(place_of_production) + ", " + str(country_of_production) + ", " + str(segment) + ", " + str(steering_wheel_type) + ", " + str(district) + ", " + str(area) + ", \'" + str(count) +"\')" 
            make_query(query, Tip_TS, has_gaps)
        elif Tip_TS == 'HCV':
            ID_HCV = ID_HCV + 1
            query = "(ID_" + Tip_TS + ", Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_вида_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса, ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_расположения_руля, ID_округа, ID_района, Количество) VALUES(" + str(ID_HCV) + ", \'" + str(srez) + "\', " + str(key_brand) + ", " + str(key_model) + ", \'" + str(year_of_release) + "\', " + str(body_type) + ", " + str(body_kind) + ", " + str(fuel_type) + ", \'" + str(engine_volume) + "\', \'" + str(engine_power) + "\', \'" + str(total_weight) + "\', \'" + str(curb_weight)+ "\', " + str(owner_type) + ", \'" + str(environmental_class)  + "\', " + str(place_of_production) + ", " + str(country_of_production) + ", " + str(steering_wheel_type) + ", " + str(district) + ", " + str(area) + ", \'" + str(count) +"\')"  
            make_query(query, Tip_TS, has_gaps)
        elif Tip_TS == 'MT':
            ID_MT = ID_MT + 1
            query = "(ID_" + Tip_TS + ", Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность, ID_типа_владельца, ID_округа, ID_района, Количество) VALUES("+ str(ID_MT) + ", \'" + str(srez) + "\', " + str(key_brand) + ", " + str(key_model) + ", \'" + str(year_of_release) + "\', " + str(body_type) + ", " + str(fuel_type) + ", \'" + str(engine_volume) + "\', \'" + str(engine_power) + "\', " + str(owner_type) + ", " + str(district) + ", " + str(area) + ", \'" + str(count) +"\')" 
            make_query(query, Tip_TS, has_gaps)
        elif Tip_TS == 'TR':
            ID_TR = ID_TR + 1
            query = "(ID_" + Tip_TS + ", Срез_парка, ID_марки, ID_модели, Год_выпуска, Полная_масса, Снаряжённая_масса, ID_типа_прицепа, ID_типа_кузова_прицепа, Количество_осей_прицепа, Количество_шин_прицепа, ID_типа_владельца, ID_округа, ID_района, Количество) VALUES(" + str(ID_TR) + ", \'" + str(srez) + "\', " + str(key_brand) + ", " + str(key_model) + ", \'" + str(year_of_release) + "\', \'" + str(total_weight) + "\', \'" + str(curb_weight) + "\', " + str(trailer_type) + ", " + str(trailer_body_type) + ", \'" + str(number_of_axes) + "\', \'" + str(number_of_tires) + "\', " + str(owner_type) + ", " + str(district) + ", " + str(area) + ", \'" + str(count) +"\')" 
            make_query(query, Tip_TS, has_gaps)
        

connectionString1 = ("DRIVER={SQL Server};SERVER=LAPTOP-MG3M03B5\\SQLEXPRESS;Trusted_Connection=Yes;")

connection = pyodbc.connect(connectionString1, autocommit=True)

dbCursor = connection.cursor()

# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2018г).xlsx","PC", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2019г).xlsx","PC", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2020г).xlsx","PC", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","LCV", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","HCV", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","BUS", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","MT", connection, dbCursor)
# fill("C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","TR", connection, dbCursor)

connection.commit()