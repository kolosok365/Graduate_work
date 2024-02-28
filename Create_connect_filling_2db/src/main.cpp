
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <OpenXLSX.hpp>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

using namespace OpenXLSX;


#define SQL_RETURN_CODE_LEN 1000
void show_error(unsigned int handletype, const SQLHANDLE& handle) {
    SQLCHAR sqlstate[SQL_RETURN_CODE_LEN];
    SQLCHAR message[SQL_RETURN_CODE_LEN];
    if (SQL_SUCCESS == SQLGetDiagRec(handletype,
        handle,
        1,
        sqlstate,
        NULL,
        message,
        SQL_RETURN_CODE_LEN,
        NULL)) {
        std::cout << "Message: " << message << "\nSQLSTATE: " << sqlstate << std::endl;
    }
}

std::unordered_map<std::string, std::string> d;
std::vector<std::string> db_names;

//void InitMap() {
//    d["Cope"] = "Купе";
//    d["Sedan"] = "Седан";
//    d["Station wagon"] = "Универсал";
//    d["Cabriolet"] = "Кабриолет";
//    d["Liftback"] = "Лифтбек";
//    d["Pickup"] = "Пикап";
//    d["Other"] = "Прочие";
//    d["Hatchback"] = "Хэтчбек";
//    d["Petrol/Methane"] = "Бензин/Метан";
//    d["Methane"] = "Метан";
//    d["Petrol/Electricity"] = "Бензин/Электричество";
//    d["Electricity"] = "Электричество";
//    d["Petrol"] = "Бензин";
//    d["Diesel"] = "Дизель";
//    d["No data"] = "Нет данных";
//    d["Nat. face"] = "Физ. лицо";
//    d["Legal. face"] = "Юр. лицо";
//    d["Imported"] = "Импортное";
//    d["Domestic"] = "Отечественное";
//    d["Australia"] = "Австралия";
//    d["United Kingdom"] = "Великобритания";
//    d["Germany"] = "Германия";
//    d["India"] = "Индия";
//    d["Austria"] = "Австрия";
//    d["Belarus"] = "Беларусь";
//    d["Belgium"] = "Бельгия";
//    d["Brazil"] = "Бразилия";
//    d["Hungary"] = "Венгрия";
//    d["Venezuela"] = "Венесуэла";
//    d["Indonesia"] = "Индонезия";
//    d["Iran"] = "Иран";
//    d["Spain"] = "Испания";
//    d["Italy"] = "Италия";
//    d["Kazakhstan"] = "Казахстан";
//    d["Canada"] = "Канада";
//    d["China"] = "Китай";
//    d["Malaysia"] = "Малайзия";
//    d["Mexico"] = "Мексика";
//    d["Netherlands"] = "Нидерланды";
//    d["Poland"] = "Польша";
//    d["Portugal"] = "Португалия";
//    d["Russia"] = "Россия";
//    d["Romania"] = "Румыния";
//    d["Slovakia"] = "Словакия";
//    d["United States"] = "США";
//    d["Thailand"] = "Таиланд";
//    d["Taiwan"] = "Тайвань";
//    d["Turkey"] = "Турция";
//    d["Uzbekistan"] = "Узбекистан";
//    d["Ukraine"] = "Украина";
//    d["France"] = "Франция";
//    d["Czechia"] = "Чехия";
//    d["Switzerland"] = "Швейцария";
//    d["Sweden"] = "Швеция";
//    d["South Africa"] = "ЮАР";
//    d["South Korea"] = "Южная Корея";
//    d["Japan"] = "Япония";
//    d["Left"] = "Левый";
//    d["Right"] = "Правый";
//    d["Eastern Administrative Okrug"] = "Восточный административный округ";
//    d["Western Administrative Okrug"] = "Западный административный округ";
//    d["Zelenograd Administrative Okrug"] = "Зеленоградский административный округ";
//    d["Novomoskovsk Administrative Okrug"] = "Новомосковский административный округ";
//    d["Northern Administrative Okrug"] = "Северный административный округ";
//    d["North-Western Administrative Okrug"] = "Северо-Западный административный округ";
//    d["North-Eastern Administrative Okrug"] = "Северо-Восточный административный округ";
//    d["Troitsk Administrative Okrug"] = "Троицкий административный округ";
//    d["Central Administrative District"] = "Центральный административный округ";
//    d["South-Eastern Administrative Okrug"] = "Юго-Восточный административный округ";
//    d["South-Western Administrative Okrug"] = "Юго-Западный административный округ";
//    d["Southern Administrative Okrug"] = "Южный административный округ";
//    d["\"Mosrentgen\""] = "\"Мосрентген\"";
//    d["Academic"] = "Академический";
//    d["Alekseevskiy"] = "Алексеевский";
//    d["Altufyevsky"] = "Алтуфьевский";
//    d["Arbat"] = "Арбат";
//    d["Airport"] = "Аэропорт";
//    d["Babushkinskiy"] = "Бабушкинский";
//    d["Basmanny"] = "Басманный";
//    d["Running"] = "Беговой";
//    d["Beskudnikovskiy"] = "Бескудниковский";
//    d["Bibirevo"] = "Бибирево";
//    d["Biryulyovo Vostochnoye"] = "Бирюлево Восточное";
//    d["Biryulyovo Zapadnoye"] = "Бирюлево Западное";
//    d["Bogorodskoye"] = "Богородское";
//    d["Brateevo"] = "Братеево";
//    d["Butyrskiy"] = "Бутырский";
//    d["Veshnyaky"] = "Вешняки";
//    d["Vnukovo"] = "Внуково";
//    d["Vnukovskoye"] = "Внуковское";
//    d["Voykovsky"] = "Войковский";
//    d["Voronovskoye"] = "Вороновское";
//    d["Voskresenskoye"] = "Воскресенское";
//    d["Vostochnoye Degunino"] = "Восточное Дегунино";
//    d["Vostochnoye Izmailovo"] = "Восточное Измайлово";
//    d["Oriental"] = "Восточный";
//    d["Vykhino-Zhulebino"] = "Выхино-Жулебино";
//    d["Gagarinsky"] = "Гагаринский";
//    d["Golovinsky"] = "Головинский";
//    d["Golyanovo"] = "Гольяново";
//    d["Danilovsky"] = "Даниловский";
//    d["Desenovskoye"] = "Десеновское";
//    d["Dmitrovskiy"] = "Дмитровский";
//    d["Donskoy"] = "Донской";
//    d["Dorogomilovo"] = "Дорогомилово";
//    d["Zamoskvorechye"] = "Замоскворечье";
//    d["Zapadnoye Degunino"] = "Западное Дегунино";
//    d["Zyuzino"] = "Зюзино";
//    d["Zyablikovo"] = "Зябликово";
//    d["Ivanovskoye"] = "Ивановское";
//    d["Izmailovo"] = "Измайлово";
//    d["Kapotnya"] = "Капотня";
//    d["Kiev"] = "Киевский";
//    d["Klenovskoye"] = "Кленовское";
//    d["Kokoshkino"] = "Кокошкино";
//    d["Konkovo"] = "Коньково";
//    d["Koptevo"] = "Коптево";
//    d["Kosino-Ukhtomsky"] = "Косино-Ухтомский";
//    d["Kotlovka"] = "Котловка";
//    d["Krasnopahorskoye"] = "Краснопахорское";
//    d["Krasnoselsky"] = "Красносельский";
//    d["Krylatskoye"] = "Крылатское";
//    d["Kryukovo"] = "Крюково";
//    d["Kuzminki"] = "Кузьминки";
//    d["Kuntsevo"] = "Кунцево";
//    d["Kurkino"] = "Куркино";
//    d["Levoberezhny"] = "Левобережный";
//    d["Lefortovo"] = "Лефортово";
//    d["Lianozovo"] = "Лианозово";
//    d["Lomonosovsky"] = "Ломоносовский";
//    d["Losinoostrovsky"] = "Лосиноостровский";
//    d["Lyublino"] = "Люблино";
//    d["Marushkinskoye"] = "Марушкинское";
//    d["Marfino"] = "Марфино";
//    d["Maryina Roshcha"] = "Марьина роща";
//    d["Maryino"] = "Марьино";
//    d["Matushkino"] = "Матушкино";
//    d["Metrogorodok"] = "Метрогородок";
//    d["Narrow-minded"] = "Мещанский";
//    d["Mitino"] = "Митино";
//    d["Mikhailov-Yartsevskoye"] = "Михайлово-Ярцевское";
//    d["Mozhaiskiy"] = "Можайский";
//    d["Molzhaninovskiy"] = "Молжаниновский";
//    d["Moskvorechye-Saburovo"] = "Москворечье-Сабурово";
//    d["Moscow"] = "Московский";
//    d["Nagatino-Sadovniki"] = "Нагатино-Садовники";
//    d["Nagatinsky backwater"] = "Нагатинский затон";
//    d["Upland"] = "Нагорный";
//    d["Nekrasovka"] = "Некрасовка";
//    d["Nizhny Novgorod"] = "Нижегородский";
//    d["Novogireevo"] = "Новогиреево";
//    d["Novokosino"] = "Новокосино";
//    d["Novo-Peredelkino"] = "Ново-Переделкино";
//    d["Novofedorovskoye"] = "Новофедоровское";
//    d["Obruchevskiy"] = "Обручевский";
//    d["Orekhovo-Borisovo Severnoye"] = "Орехово-Борисово Северное";
//    d["Orekhovo-Borisovo Yuzhnoye"] = "Орехово-Борисово Южное";
//    d["Ostankino"] = "Останкинский";
//    d["Welcome"] = "Отрадное";
//    d["Printers"] = "Печатники";
//    d["Perovo"] = "Перово";
//    d["Pokrovskoye-Streshnevo"] = "Покровское-Стрешнево";
//    d["Ochakovo-Matveevskoye"] = "Очаково-Матвеевское";
//    d["Presnensky"] = "Пресненский";
//    d["Pervomayskoye"] = "Первомайское";
//    d["Preobrazhenskoye"] = "Преображенское";
//    d["Prospekt Vernadskogo"] = "Проспект Вернадского";
//    d["Ramenki"] = "Раменки";
//    d["Ryazan"] = "Рязанский";
//    d["Rostokino"] = "Ростокино";
//    d["Savelki"] = "Савелки";
//    d["Ryazanovskoye"] = "Рязановское";
//    d["Severnoe Butovo"] = "Северное Бутово";
//    d["Severnoye Izmailovo"] = "Северное Измайлово";
//    d["Silino"] = "Силино";
//    d["Falcon"] = "Сокол";
//    d["Sviblovo"] = "Свиблово";
//    d["Severnoye Medvedkovo"] = "Северное Медведково";
//    d["Savyolovsky"] = "Савеловский";
//    d["Severnoye Tushino"] = "Северное Тушино";
//    d["Falcon Mountain"] = "Соколиная гора";
//    d["Sokolniki"] = "Сокольники";
//    d["Solntsevo"] = "Солнцево";
//    d["Sosenskoye"] = "Сосенское";
//    d["North"] = "Северный";
//    d["Staroe Kryukovo"] = "Старое Крюково";
//    d["Tagansky"] = "Таганский";
//    d["Strogino"] = "Строгино";
//    d["Teply Stan"] = "Теплый Стан";
//    d["Tverskoy"] = "Тверской";
//    d["Timiryazevsky"] = "Тимирязевский";
//    d["Troparevo-Nikulino"] = "Тропарево-Никулино";
//    d["Filevsky Park"] = "Филевский парк";
//    d["Tekstilshchiki"] = "Текстильщики";
//    d["Fili-Davydkovo"] = "Фили-Давыдково";
//    d["Khoroshevsky"] = "Хорошевский";
//    d["Khamovniki"] = "Хамовники";
//    d["Khovrino"] = "Ховрино";
//    d["Khoroshevo-Mnevniki"] = "Хорошево-Мневники";
//    d["Chertanovo Severnoye"] = "Чертаново Северное";
//    d["Cheryomushki"] = "Черемушки";
//    d["Tsaritsyno"] = "Царицыно";
//    d["Shchapovskoye"] = "Щаповское";
//    d["Chertanovo Yuzhnoye"] = "Чертаново Южное";
//    d["Chertanovo Tsentralnoye"] = "Чертаново Центральное";
//    d["Yuzhnoye Butovo"] = "Южное Бутово";
//    d["Shchukino"] = "Щукино";
//    d["Shcherbinka"] = "Щербинка";
//    d["Yuzhnoye Tushino"] = "Южное Тушино";
//    d["Yuzhnoportovy"] = "Южнопортовый";
//    d["Yuzhnoye Medvedkovo"] = "Южное Медведково";
//    d["Yasenevo"] = "Ясенево";
//    d["Yaroslavl"] = "Ярославский";
//    d["Yakimanka"] = "Якиманка";
//    d["Rogovskoye"] = "Роговское";
//    d["Filimonkovskoye"] = "Филимонковское";
//    d["Minibus"] = "Микроавтобус";
//    d["Flatbed/chassis"] = "Бортовой/шасси";
//    d["On the basis of passenger cars"] = "На базе легковых автомобилей";
//    d["Platform"] = "Платформа";
//    d["Specialized"] = "Специализированные";
//    d["Van"] = "Фургон";
//    d["VAN CMC"] = "ЦМК";//Фургон ЦМК
//    d["Emergency"] = "Аварийный";
//    d["Aerial Platform/Auto Hydraulic Lift"] = "Автовышка/Автогидроподъемник";
//    d["Paddy wagon"] = "Автозак";
//    d["Auto Shop"] = "Автолавка";
//    d["Vehicle MVD"] = "Автомобиль МВД";
//    d["Vehicle MHS"] = "Автомобиль МЧС";
//    d["Tanker"] = "Автоцистерна";
//    d["Tow truck"] = "Автоэвакуатор";
//    d["Onboard with crane manipulator"] = "Бортовой с КМУ";
//    d["Flatbed/Side Tarpaulin"] = "Бортовой/Бортовой тентованный";
//    d["Armored"] = "Бронированный";
//    d["Cargo-passenger"] = "Грузопассажирский";
//    d["Minibus up to 13 linear meters"] = "Микроавтобус до 13 п.м.";
//    d["Minibus from 13 linear meters"] = "Микроавтобус от 13 п.м.";
//    d["Pickup/flatbed"] = "Пикап/бортовой";
//    d["Fireman"] = "Пожарный";
//    d["Ritual"] = "Ритуальный";
//    d["Dump truck"] = "Самосвал";
//    d["Tractor"] = "Седельный тягач";
//    d["Ambulance/Medical Service"] = "Скорая помощь/Медслужба";
//    d["Specialized Other"] = "Специализированные прочие";
//    d["Bread Transportation Van"] = "Фургон для перевозки хлеба";
//    d["Isothermal van"] = "Фургон изотермический";
//    d["Lab Van"] = "Фургон лаборатория";
//    d["Van Workshop"] = "Фургон мастерская";
//    d["Refrigerated van"] = "Фургон рефрижератор";
//    d["All-metal"] = "Цельнометаллический";
//    d["All-Metal Refrigerated Truck"] = "Цельнометаллический рефрижератор";
//    d["Full Metal Van"] = "Цельнометаллический фургон";
//    d["No data"] = "Нет данных";
//    d["Shift bus"] = "Автобус вахтовый";
//    d["Truck cranes, hoists"] = "Автокраны, подъемники";
//    d["Onboard"] = "Бортовой";
//    d["Municipal machinery"] = "Коммунальная техника";
//    d["Special equipment of public services"] = "Спецтехника гос.служб";
//    d["Construction Machinery"] = "Строительная техника";
//    d["Machinery for the extractive industries"] = "Техника для добывающих отраслей";
//    d["Motorhome"] = "Автодом";
//    d["Kung"] = "Кунг";   
//    d["Workshop with CMU"] = "Мастерская с КМУ";
//    d["Emergency Rescue Department of the Ministry of Emergency Situations"] = "Аварийно-спасательная МЧС";
//    d["Concrete Pump Truck"] = "Автобетононасос";
//    d["Concrete Mixer Truck"] = "Автобетоносмеситель";
//    d["Bitumen truck"] = "Автобитумовоз";
//    d["Car transporter"] = "Автовоз";
//    d["Asphalt distributor"] = "Автогудронатор";
//    d["Truck Crane"] = "Автокран";
//    d["Oil Tanker"] = "Автомаслозаправщик";
//    d["Bulk Material Tanker Truck"] = "Автоцистерна для сыпучих материалов";
//    d["Oilfield Tank Truck"] = "Автоцистерна нефтепромысловая";
//    d["Tow truck with crane manipulator"] = "Автоэвакуатор с КМУ";
//    d["Oil for Gas Condensate Gathering Unit"] = "Агрегат для сбора нефти и газового конденсата";
//    d["Nitrogen Compressor Unit"] = "Азотная компрессорная установка";
//    d["Sewage Collector"] = "Ассенизатор";
//    d["Fuel tanker"] = "Бензовоз";
//    d["Manifold Block"] = "Блок манифольда";
//    d["Flatbed/tarpaulin"] = "Бортовой/тентованный";
//    d["Bunker Truck"] = "Бункеровоз";
//    d["Vacuum Excavator"] = "Вакуумный экскаватор";
//    d["Water carrier"] = "Водовоз";
//    d["Hydration Unit"] = "Гидратационная установка";
//    d["For gas transportation"] = "Для перевозки газа";
//    d["For transportation of gas cylinders"] = "Для перевозки газовых балонов";
//    d["Live Fish"] = "Живая рыба";
//    d["Dry Food Loader"] = "Загрузчик сухих кормов";
//    d["Sludge sucker"] = "Илосос";
//    d["Channel Washing Machine"] = "Канало-промывочная машина";
//    d["Combined Road Machine"] = "Комбинированная дорожная машина";
//    d["Container ship"] = "Контейнеровоз";
//    d["Timber carrier"] = "Лесовоз";
//    d["Timber truck with crane manipulator"] = "Лесовоз с КМУ";
//    d["Scrap truck with crane manipulator"] = "Ломовоз с КМУ";
//    d["Medical"] = "Медицинская";
//    d["Milk Truck"] = "Молоковоз";
//    d["Multilift"] = "Мультилифт";
//    d["Garbage truck"] = "Мусоровоз";
//    d["Mobile Pumping Unit"] = "Передвижная насосная установка";
//    d["Sand spreader"] = "Пескоразбрасыватель";
//    d["Platform with crane manipulator"] = "Платформа с КМУ";
//    d["The platform is universal"] = "Платформа универсальная";
//    d["Sweeping and sweeping"] = "Подметально-уборочная";
//    d["Logging lift"] = "Подъемник каротажный";
//    d["Fire"] = "Пожарная";
//    d["Fire Ladder Truck"] = "Пожарная автолестница";
//    d["Fire Tanker Truck"] = "Пожарная автоцистерна";
//    d["Fire Pumping Station"] = "Пожарная насосная станция";
//    d["Fire Hose Truck"] = "Пожарный рукавный автомобиль";
//    d["Polovochnaya"] = "Поливочная";
//    d["Other municipal equipment"] = "Прочая коммунальная техника";
//    d["Other for extractive industries"] = "Прочие для добывающих отраслей";
//    d["Others for special services"] = "Прочие для спец. служб";
//    d["Flatbed dump truck"] = "Самосвал бортовой";
//    d["Dump truck with crane manipulator"] = "Самосвал с КМУ";
//    d["Truck tractor with crane manipulator"] = "Седельный тягач с КМУ";
//    d["Mixing Charging Machine"] = "Смесительно-зарядная машина";
//    d["Snowplow"] = "Снегоуборочная";
//    d["Log truck"] = "Сортиментовоз";
//    d["Log truck with crane manipulator"] = "Сортиментовоз с КМУ";
//    d["Hydraulic Fracturing Monitoring Station"] = "Станция контроля гидроразрыва пласта";
//    d["Other construction machinery"] = "Строительная техника прочие";
//    d["Tankerrrr"] = "Топливозаправщик";
//    d["Pipe weave carrier"] = "Трубоплетевоз";
//    d["Drilling and crane rig"] = "Установка бурильно-крановая";
//    d["Drilling rig"] = "Установка буровая";
//    d["Well Acid Treatment Plant"] = "Установка для кислотной обработки скважин";
//    d["Well Cementing Plant"] = "Установка для цементирования скважин";
//    d["Intermediary Installation"] = "Установка осреднительная";
//    d["Steam Generator Unit"] = "Установка парогенераторная";
//    d["Food Tank"] = "Цистерна для пищевых продуктов";
//    d["Excavator"] = "Экскаватор";
//    d["Georgia"] = "Грузия";
//    d["Finland"] = "Финляндия";
//    d["Argentina"] = "Аргентина";
//    d["Troitskk"] = "Троицк";
//    d["Patching Machine"] = "Машина ямочного ремонта";
//    d["Older than 1987"] = "Старше 1987";
//    d["Urban"] = "Городской";
//    d["Long-distance/tourist"] = "Междугородный/туристический";
//    d["Suburban"] = "Пригородный";
//    d["Latvia"] = "Латвия";
//    d["Tajikistan"] = "Таджикистан";
//    d["Slovenia"] = "Словения";
//    d["Serbia"] = "Сербия";
//    d["Motorbike"] = "Мотоцикл";
//    d["Motorcycle"] = "Мотоциклетный";
//    d["Cargo semi-trailer"] = "Полуприцеп грузовой";
//    d["Cargo trailer"] = "Прицеп грузовой";
//    d["Passenger trailer"] = "Прицеп легковой";
//    d["Bulk Carrier"] = "Балковоз";
//    d["Bitumen carrier"] = "Битумовоз";
//    d["Flatbed/Tarpaulin/Container Ship"] = "Бортовой/Тентованный/Контейнеровоз";
//    d["Flatbed/Tarpaulin/Refrigerated"] = "Бортовой/Тентованный/Рефрижиратор";
//    d["Flatbed/Tarpaulin/Van"] = "Бортовой/Тентованный/Фургон";
//    d["LNG Carrier"] = "Газовоз";
//    d["Gel Tank"] = "Гелевый танк";
//    d["Jumbo"] = "Джамбо";
//    d["For transportation of diesel generators"] = "Для перевозки ДГУ";
//    d["For transporting equipment"] = "Для перевозки оборудования";
//    d["Grain carrier"] = "Зерновоз";
//    d["Isothermal"] = "Изотермический";
//    d["Cable Conveyor"] = "Кабельный транспортер";
//    d["Horse Carrier"] = "Коневозный";
//    d["Boat"] = "Лодочный";
//    d["Scrap Truck"] = "Ломовоз";
//    d["Flour Truck"] = "Муковоз";
//    d["Tanker Oil"] = "Нефтевоз";
//    d["Patching Equipment"] = "Оборудование для ямочного ремонта";
//    d["Panel Carrier"] = "Панелевоз";
//    d["Platform/Flatbed/Tarpaulin"] = "Платформа/Бортовой/Тентованный";
//    d["Platform/Container Ship/Flatbed/Tarpaulin"] = "Платформа/Контейнеровоз/Бортовой/Тентованный";
//    d["Other specialized"] = "Прочие специализированные";
//    d["Bee Truck"] = "Пчеловоз";
//    d["Refrigerator"] = "Рефрижератор";
//    d["Dissolution"] = "Роспуск";
//    d["Dump Truck/Car Transporter"] = "Самосвал/Автовоз";
//    d["Cattle Truck"] = "Скотовоз";
//    d["Commercial"] = "Торговый";
//    d["Trawl"] = "Трал";
//    d["Trawl/Panel Carrier/Platform"] = "Трал/Панелевоз/Платформа";
//    d["Pipe Carrier"] = "Трубовоз";
//    d["Cement Truck"] = "Цементовоз";
//    d["Cistern"] = "Цистерна";
//    d["Tank for Chemical Products"] = "Цистерна для химической продукции";
//    d["Chassis"] = "Шасси";
//    d["Wood chip truck"] = "Щеповоз";
//    d["Park cut"] = "Срез парка";
//    d["Vehicle type"] = "Тип ТС";
//    d["Brand"] = "Марка";
//    d["Model"] = "Модель";
//    d["Year of issue"] = "Год выпуска";
//    d["Body type"] = "Тип кузова";
//    d["Fuel type"] = "Тип топлива";
//    d["Engine volume, cm3"] = "Объем двигателя, см3";
//    d["Engine power, hp"] = "Мощность двигателя, л.с.";
//    d["Owner type"] = "Тип владельца";
//    d["Environmental class"] = "Экологический класс";
//    d["Place of production"] = "Место производства";
//    d["Country of Origin"] = "Страна производства";
//    d["Origin of the brand"] = "Происхождение марки";
//    d["Country of origin of the brand"] = "Страна происхождения марки";
//    d["Autostat segment"] = "Сегмент Автостат";
//    d["Steering wheel type"] = "Тип расположения руля";
//    d["District"] = "Округ";
//    d["Area"] = "Район";
//    d["Quantity"] = "Количество";
//    d["Body kind"] = "Вид кузова";
//    d["Total weight, kg"] = "Полная масса, кг";
//    d["Curb weight, kg"] = "Снаряженная масса, кг";
//    d["Trailer type"] = "Тип прицепа";
//    d["Trailer body type"] = "Тип кузова прицепа";
//    d["Number of trailer axles"] = "Количество осей прицепа";
//    d["Number of trailer tires"] = "Количество шин прицепа";
//    //d[""] = "";
//    //d[""] = "";
//    //d[""] = "";
//    //d[""] = "";
//    //d[""] = "";
//    //d["No data"] = "No data";
//    for (auto& [s1,s2] : d) {
//        std::cout << s2 << "\n";
//    }
//}

bool Сonnect_DBMS(SQLHANDLE& envHandle, SQLHANDLE& connHandle, SQLRETURN& ret) {
    
    SQLCHAR* connStr = (SQLCHAR*)"DRIVER={SQL Server};SERVER=LAPTOP-MG3M03B5\\SQLEXPRESS;Trusted_Connection=Yes;";
    ret = SQLDriverConnect(connHandle, NULL, connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);//установление соединения с базой данных

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Подключение к SSMS успешно установлено." << std::endl;
        return true;
    }
    return false;
}

bool Create_DB(SQLHANDLE& stmtHandle, SQLRETURN& ret, std::string Name_db, std::string filename) {
    db_names.push_back(Name_db);
    std::string query = "CREATE DATABASE " + Name_db + "; ";
    SQLRETURN answer = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (!(answer == SQL_SUCCESS || answer == SQL_SUCCESS_WITH_INFO)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }

    std::cout << "База данных " + Name_db + " успешно создана." << std::endl;
    query.clear();

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка при открытии файла" << std::endl;
        return 1;
    }
    std::string line;

    // Читаем содержимое файла построчно и сохраняем в строку query
    while (std::getline(file, line)) {
        query += line;
        query += '\n';
    }

    SQLCHAR* create_tables = (SQLCHAR*)query.c_str();
    if (0 >= SQLExecDirect(stmtHandle, create_tables, SQL_NTS)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    return true;

    ret = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Создание сущностей произошло успешно." << std::endl;
        return true;
    }
    else {
        std::cout << "Не удалось создать сущности!" << std::endl;
        return false;
    }

}


//получение одного столбца в excel файле
//void Get_data_from_one_col(XLWorksheet& wks, std::unordered_map<std::string, int>& m,
//                           int current_col, int rows_count, bool change, bool set_keys)
//{
//    if (!change) {
//        for (int i = 2; i <= /*5000*/rows_count; ++i) {
//            std::string str = wks.cell(i, current_col).value();
//            size_t lastIndex = str.find_last_not_of(' ') + 1;
//            str.erase(lastIndex);
//            m.insert({ str, 0 });
//        }
//    }
//    else {        
//        for (int i = 2; i <= /*5000*/rows_count; ++i) {
//            std::string str = wks.cell(i, current_col).value();
//            size_t lastIndex = str.find_last_not_of(' ') + 1;
//            str.erase(lastIndex);
//            m.insert({ d[str], 0 });
//        }
//    }   
//    if (set_keys) {
//        int k = 0;
//        for (auto& [str, key] : m) {
//            key = ++k;
//        }
//    }
//}

void Make_Query_for_two_cols(SQLHANDLE& stmtHandle, const std::string& query,
                             std::unordered_map<std::string, int>& m) 
{
    for (auto& [data, id] : m) {
        std::string result_query = query + std::to_string(id) + ", \'" + data + "\')";
        SQLCHAR* make_query = (SQLCHAR*)result_query.c_str();
        SQLExecDirect(stmtHandle, make_query, SQL_NTS);
    }
}

void ReadFile(std::unordered_map<std::string,int>& um, const std::string& path, bool set_keys) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Ошибка при открытии файла " + path + "!\n";
        exit(1);
    }
    std::string line;
    while (std::getline(file, line))
        um.insert({ line, 0 });
    
    if (set_keys) {
        int k = 0;
        for (auto& [str, key] : um) {
            key = ++k;
        }
    }
}

void Filling_common_entities(SQLHANDLE& stmtHandle, SQLHANDLE& connHandle) {   
    static std::unordered_map<std::string, int> um_models;
    static std::unordered_map<std::string, int> um_body_type;
    static std::unordered_map<std::string, int> um_fuel_type;
    static std::unordered_map<std::string, int> um_owner_type;
    static std::unordered_map<std::string, int> um_segment;
    static std::unordered_map<std::string, int> um_steering_wheel;
    static std::unordered_map<std::string, int> um_district;
    static std::unordered_map<std::string, int> um_area;
    static std::unordered_map<std::string, int> um_imported_domestic;
    static std::unordered_map<std::string, int> um_countries;
    static std::unordered_map<std::string, int> um_body_kind;
    static std::unordered_map<std::string, int> um_trailer_type;
    static std::unordered_map<std::string, int> um_trailer_body_type;
    
    
    //system("C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Read_common_entities.py");
    
    ReadFile(um_models, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Модель.txt", true);
    ReadFile(um_body_type, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Тип кузова.txt", true);
    ReadFile(um_fuel_type, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Тип топлива.txt", true);
    ReadFile(um_owner_type, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Тип владельца.txt", true);
    ReadFile(um_segment, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Сегмент Автостат.txt", true);
    ReadFile(um_steering_wheel, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Тип расположения руля.txt", true);
    ReadFile(um_district, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Округ.txt", true);
    ReadFile(um_area, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Район.txt", true);
    ReadFile(um_body_kind, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Вид кузова.txt", true);
    ReadFile(um_trailer_type, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Тип прицепа.txt", true);
    ReadFile(um_trailer_body_type, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Тип кузова прицепа.txt", true);

    ReadFile(um_imported_domestic, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Место производства.txt", false);
    ReadFile(um_imported_domestic, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Происхождение марки.txt", true);

    ReadFile(um_countries, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Страна производства.txt", false);
    ReadFile(um_countries, "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Страна происхождения марки.txt", true);

    SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
    
       
    for (const auto& current_name : db_names) {//заполненение db_Gaps и db_Norm
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Модель(ID_модели, Название) VALUES(", um_models);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Тип_кузова(ID_типа_кузова, Название) VALUES(", um_body_type);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Тип_топлива(ID_типа_топлива, Название) VALUES(", um_fuel_type);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Тип_владельца(ID_типа_владельца, Название) VALUES(", um_owner_type);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Сегмент_автостата(ID_сегмента_автостата, Название) VALUES(", um_segment);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Расположение_руля(ID_расположения_руля, Название) VALUES(", um_steering_wheel);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Округ(ID_округа, Название) VALUES(", um_district);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Район(ID_района, Название) VALUES(", um_area);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Страна(ID_страны, Название) VALUES(", um_countries);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Место_производства(ID_места_производства, Название) VALUES(", um_imported_domestic);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Вид_кузова(ID_вида_кузова, Название) VALUES(", um_body_kind);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Тип_прицепа(ID_типа_прицепа, Название) VALUES(", um_trailer_type);
        Make_Query_for_two_cols(stmtHandle, "INSERT INTO " + current_name + ".dbo.Тип_кузова_прицепа(ID_типа_кузова_прицепа, Название) VALUES(", um_trailer_body_type);
    }     
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
}

std::string ltrim(const std::string& str)
{
    size_t i = 0;
    while (i < str.length() && std::isspace(str[i]))
    {
        ++i;
    }
    return str.substr(i);
}

void Filling_brands(SQLHANDLE& stmtHandle, SQLHANDLE& connHandle) {
    //system("C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Read_brands.py");
    static std::unordered_map<std::string, int> um_brands;
    std::string filename = "C:\\Graduate_work\\Create_connect_filling_2db\\Уникальные_значения_столбцов\\Марки.txt";
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка при открытии файла " + filename << std::endl;
        exit(1);
    }
    file.seekg(0);
    
    std::string brand;
    std::string place;
    std::string country;
    int key = 0;
    int ID_brand = 0;
    while (std::getline(file, brand, ':')) {       
        std::getline(file, place, ',');
        std::getline(file, country, '\n');
        place = ltrim(place);
        country = ltrim(country);
    
        // Извлечение результатов
        SQLCHAR result[255];
        SQLLEN cbResult;
        std::string place_key;
        std::string country_key;

        SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
        std::string result_query = "select ID_места_производства from db_Norm.dbo.Место_производства where Название = '" + place + "'";
        SQLCHAR* make_query = (SQLCHAR*)result_query.c_str();
        SQLExecDirect(stmtHandle, make_query, SQL_NTS);
        SQLBindCol(stmtHandle, 1, SQL_C_CHAR, result, 255, &cbResult);
        SQLRETURN ret = SQLFetch(stmtHandle);
        //std::string place_key;
        if (SQL_SUCCEEDED(ret)) {
            SQLCHAR* sqlCharPtr = reinterpret_cast<SQLCHAR*>(result);
            std::string s(reinterpret_cast<char*>(sqlCharPtr));
            place_key = s;
        }
        else {
            std::cout << "Не удалось получить результат" << std::endl;
            exit(1);
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

        SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
        result_query = "select ID_страны from db_Norm.dbo.Страна where Название = '" + country + "'";
        make_query = (SQLCHAR*)result_query.c_str();
        SQLExecDirect(stmtHandle, make_query, SQL_NTS);        
       
        SQLBindCol(stmtHandle, 1, SQL_C_CHAR, result, 255, &cbResult);
        ret = SQLFetch(stmtHandle);
        
        if (SQL_SUCCEEDED(ret)) {
            SQLCHAR* sqlCharPtr = reinterpret_cast<SQLCHAR*>(result);
            std::string s(reinterpret_cast<char*>(sqlCharPtr));
            country_key = s;
        }
        else {
            std::cout << "Не удалось получить результат" << std::endl;
            exit(1);
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

        SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
        ++ID_brand;
        for (const auto& current_name : db_names) {
            result_query = "INSERT INTO " + current_name + 
                ".dbo.Марка(ID_марки, Название, ID_страны_происхождения, ID_места_происхождения) VALUES(" 
                + std::to_string(ID_brand) + ", \'" + brand + "\', " + country_key + ", " + place_key + ")";
            make_query = (SQLCHAR*)result_query.c_str();
            SQLRETURN answer = SQLExecDirect(stmtHandle, make_query, SQL_NTS);
            if (!(answer == SQL_SUCCESS || answer == SQL_SUCCESS_WITH_INFO)) {
                show_error(SQL_HANDLE_STMT, stmtHandle);
                exit(1);
            }
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    // Выполнить инициализацию ODBC
    SQLHANDLE envHandle;//для хранения дескриптора среды ODBC
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);//Идентификатор окружения. Выделяется память и инициализируется дескриптор среды ODBC.
    SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_POINTER);//Устанавливается атрибут среды ODBC

    // Создать и открыть подключение к SSMS
    SQLHANDLE connHandle;//будет использоваться для хранения дескриптора подключения к базе данных.
    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle);//Идентификатор соединения. Выделяется память и инициализируется дескриптор подключения к базе данных 
    SQLRETURN ret;

    if(!Сonnect_DBMS(envHandle, connHandle, ret)){
        std::cout << "Не удалось установить подключение к SSMS." << std::endl;
        return 1;
    }

    SQLHANDLE stmtHandle;
    
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
    
    //Создать базу данных
    if (!Create_DB(stmtHandle, ret, "db_Norm", "C:\\Graduate_work\\Creating_Norm.sql")) {
        return 1;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
    if (!Create_DB(stmtHandle, ret, "db_Gaps", "C:\\Graduate_work\\Creating_Gaps.sql")) {
        return 1;
    }

    // Освободить ресурсы
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle); //освобождение памяти, выделенной для дескриптора оператора stmtHandle.
                                                //Эта функция сообщает драйверу ODBC о необходимости освободить ресурсы, 
                                                // связанные с заданным дескриптором.

    //db_names.push_back("db_Norm");// при создании БД надо ЗАКОМЕНТИРОВАТЬ
    //db_names.push_back("db_Gaps");// при создании БД надо ЗАКОМЕНТИРОВАТЬ
    Filling_common_entities(stmtHandle, connHandle);
    Filling_brands(stmtHandle, connHandle);
    //system("C:\\Graduate_work\\Create_connect_filling_2db\\Split_into_2_databases\\Split_into_2_databases.py");
    
    SQLDisconnect(connHandle);//отключает соединение с базой данных, представленным дескриптором connHandle
    
    SQLFreeHandle(SQL_HANDLE_DBC, connHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);

    return 0;
}


