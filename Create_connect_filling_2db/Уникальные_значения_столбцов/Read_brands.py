import pandas as pd

def f(brands_dict, path, sheet):
    # Чтение данных из excel файла
    df = pd.read_excel(path, sheet)
    # Итерация по строкам в DataFrame
    for index, row in df.iterrows():
        # Получение значений столбцов
        brand = row['Марка']
        origin = row['Происхождение марки']
        country = row['Страна происхождения марки']
    
        # Запись значений в словарь
        brands_dict[brand] = f'{origin}, {country}'


def f2(brands_dict, path, sheet):
    # Чтение данных из excel файла
    df = pd.read_excel(path, sheet)
    # Итерация по строкам в DataFrame
    for index, row in df.iterrows():
        # Получение значений столбцов
        brand = row['Марка']
    
        # Запись значений в словарь
        brands_dict[brand] = f'{"Нет данных"}, {"Нет данных"}'

        
# Создание пустого словаря
brands_dict = {}
print("2018")
f(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2018г).xlsx","PC")
print("2019")
f(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2019г).xlsx","PC")
print("2020")
f(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(2020г).xlsx","PC")
print("LCV")
f(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","LCV")
print("HCV")
f(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","HCV")
print("BUS")
f(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","BUS")
print("MT")
f2(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","MT")
print("TR")
f2(brands_dict,"C:\\Graduate_work\\Create_connect_filling_2db\\Москва(Грузовые 2018-2020г).xlsx","TR")


# Запись словаря в файл Марки.txt
with open('Марки.txt', 'w') as file:
    for key, value in brands_dict.items():
        file.write(f'{key}: {value}\n')
