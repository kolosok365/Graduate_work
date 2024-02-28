import pandas as pd

def filling_set(my_set, path, sheet, col_name):  
       data1 = pd.read_excel(path, sheet)
       try:
           unique_values = data1[col_name].unique()
           for value in unique_values:
               my_set.add(value);
       except KeyError:
           print("Исключение отработано!")

def f(col_name):
       result_unique = set([])

       print("2018")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2018г).xlsx","PC", col_name)
        
       print("2019")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2019г).xlsx","PC", col_name)

       print("2020")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2020г).xlsx","PC", col_name)        
       
       print("LCV")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","LCV", col_name)  
       
       print("HCV")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","HCV", col_name)

       print("BUS")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","BUS", col_name)

       print("MT")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","MT", col_name)
       
       print("TR")
       filling_set(result_unique, "C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","TR", col_name)

       with open("C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\" + col_name + ".txt", "w") as file:                  
           for value in result_unique:
               file.write(str(value) + "\n")

print(0)

f("Модель")
print(1)
f("Тип кузова")
print(2)
f("Тип топлива")
print(3)
f("Тип владельца")
print(4)
f("Место производства")
print(5)
f("Страна производства")
print(6)
f("Происхождение марки")
print(7)
f("Страна происхождения марки")
print(8)
f("Сегмент Автостат")
print(8)
f("Тип расположения руля")
print(9)
f("Округ")
print(10)
f("Район")
print(11)
f("Вид кузова")
print(12)
f("Тип прицепа")
print(13)
f("Тип кузова прицепа")
print(14)


