МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

​

### Отчёт по лабораторной работе № 6<br/> по дисциплине "Программирование"

<br/>
​
студента 1 курса группы ИВТ-192(1)  
Москаленко Алексея Константиновича
направления подготовки 09.03.01 "Информатика и Вычислительная Техника"  
<br/>
​
<table>
<tr><td>Научный руководитель<br/> старший преподаватель кафедры<br/> компьютерной инженерии и моделирования</td>
<td>(оценка)</td>
<td>Чабанов В.В.</td>
</tr>
</table>
<br/><br/>

Симферополь, 2019

------

# Создание приложения-сервера "Погодный информер"

## Цель:

- Закрепить навыки разработки многофайловыx приложений;
- Изучить способы работы с API web-сервиса;
- Изучить процесс сериализации/десериализации данных.

------

## Теоретическая часть

Большинство популярных сервисов предоставляют частичный доступ к своему серверу средствами **API**(*Application Programming Interface*).
Зачастую доступ к сервисам осуществляется следующим методом:
1. Регистрация учетной записи
2. Регистрация приложения
3. Получение ключа доступа(token/apikey)
4. Осуществление GET/POST запроса по протоколу HTTP

Далее в программе проходит этап обработки полученных данных и взаимодействие с пользовательским интерфейсом, либо хранение/обработка данных.

-------

Следующие шаги полностью описывают алгоритм работы программы:

1. Создание экземпляров класса **Server** и **Client**

2. Включение в программу конфигурационного файла, содержащего настроки:
    | Параметр   | Описание |
    |    :---:   |   :---:   |
    |apikey      |Ключ доступа к сервисам |
    |port        |Порт на котором открывается сервер|
    |stdcity     |Стандартный город для которого будет выводиться прогноз|
    |templatepath|Путь относительный или абсолютный до шаблона.|

3. Запрос на API для проверки существования стандартного города, указанного в конфиге и валидность ключа доступа.
4. Создание сервера и установка прослушивания на указанном порту.
5. Привязка корневой директории к фунции `MakePage`, генерирующей ответ для каждого запроса.
6. Запуск сервера в отдельном потоке
7. Ожидание ввода комманды завершения программы.

------

#### Прототипы функций и их описание
1.  Функция вывода сообщений в консоль с меткой даты и времени.
    ```cpp
    void Log(string Message, bool sysmsg = true);
    ```
2.  Функция замены строки `sub` на `repment` в строке base 
    ```cpp
    bool StrReplace(string *base, string sub, string repment);
    ```
3.  Функция считывает все содержимое файла расположенного по пути `filename` и записывает результат в `result` 
    ```cpp
    bool GetFContent(string filename, string * result);
    ```
4.  Функция возвращает строку сгенерированную по URL-формату со списком указанных параметров в `parametrs` 
    ```cpp
    string GetUrlWithParams(map<string, string> parametrs);
    ```
5.  Функция обрезает дробную часть числа `number` до указанной длины `prec` 
    ```cpp
    string CutDouble(double number, int prec = 1);
    ```
6.  Функция генерирует ответ клиенту запрашивающему страницу корневого каталога. Алгоритм вначале получает данные с API погодного сервиса по указанному в параметре запроса `city` городу или, в случае отсутсвия данного параметра, стандартному городу, указанному в конфиге. После получения ответа устанавливает тело ответа клиента. В случае допущения ошибки в названии города отвечает нестандартным шаблоном, а сообщением с ошибкой.
    ```cpp
    void MakePage(const Request& req, Response& res);
    ```

------

## Практическая часть

Для начала перейдем на сайт [openweathermap.org](http://openweathermap.org) и получим ключ доступа после регистрации, как на Снимке 1.

<image src="Resources/Images/Image1.png" height="80px"><br>
*Снимок 1. Демонстрационный токен сервиса* 

Далее скачаем и подключим следующие библиотеки:
- **cpp-httplib**
- **JSON for Modern C++**
- **HC.cfg** (Собственный класс для конфигов. Приведён в директории **Include\HC.cfg**)

И немного изменим шаблон, добавив в него возможность отображения ошибок, вместо содержимого страницы.

Далее получим IP сервиса при помощи сайта ["2ip.ru"](https://2ip.ru/lookup/ip/).

Полученные данные поместим в конфигурацию config.cfg и протестируем работу приложения.<br><br>
Результат представлен на Снимке 2.
<br><image src="Resources/Images/Image2.png" height="250px"><br>
*Снимок 2. Демонстрация работы сервера.*


------

#### Исходный код программы

```cpp
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <thread>
#include <chrono>
#include <time.h>
#include "Include/HC.cfg/HC.cfg.h"
#include "Include/httplib/httplib.h"
#include "Include/NlohmannJson/json.hpp"

using namespace std;
using namespace httplib;
using nlohmann::json;

//#define DEV

#define NS ""

#define FBSIZE 256
#define HOURSCOUNT 40
#define HOURSPASSC 8


HC_cfg Config;

Server ServerObj;						// Сервер для обращений 
Client ClientObj("82.196.7.246", 80);	// Клиент для получения данных с API погодного сервера
vector<pair<string, string>> RTable;	// Таблица для замены вхождений

map<string, string> UrlPars;

void Log(string Message, bool sysmsg = true){
	if (sysmsg) {
		cout << Message << endl;
		return;
	}
	time_t rawtime;
	struct tm* timeinfo = new tm;
	char timestamp[32];

	time(&rawtime);
	localtime_s(timeinfo, &rawtime);

	strftime(timestamp, 32, "%x|%X", timeinfo);

	cout << "[" << timestamp << "] > " << Message << endl;
}

/*---Ф-ия для замены вхождения---*/
bool StrReplace(string *base, string sub, string repment){
	auto entrypos = base->find(sub);
	if (entrypos == string::npos) return false;
	base->replace(entrypos, sub.length(), repment);
	return true;
}

/*---Ф-ия для получения содержимого файла---*/
bool GetFContent(string filename, string * result){
	ifstream file(filename);
	if (!file.is_open()) return false;

	char readbuffer[FBSIZE];
	do {
		file.getline(readbuffer, FBSIZE - 1);
		*result += readbuffer + string("\n");
	} while (!file.eof());
	file.close();
	return true;
}

/*---Ф-ия для создания строки для GET-запроса---*/
string GetUrlWithParams(map<string, string> parametrs) {
	string url;
	for (auto& element : parametrs) {
		url += element.first + "=" + element.second + "&";
	}
	url.erase(url.length() - 1);
	return url;
}

string CutDouble(double number, int prec = 1) {
	string res, dnum = to_string(number);
	string tdp, tip;
	string delim = "";
	if (dnum.find(",") != dnum.npos) delim = ",";
	if (dnum.find(".") != dnum.npos) delim = ".";
	if (delim != "") {
		tip = dnum.substr(0, dnum.find(delim));
		tdp = dnum.substr(dnum.find(delim), dnum.length());
		res = tip + tdp.substr(0, ++prec);
	}
	return res;
}

void MakePage(const Request& req, Response& res){
	if (req.get_param_value("city") != NS) UrlPars["q"] = req.get_param_value("city");
	else	UrlPars["q"] = Config.cfg["stdcity"];

	/* Совершаем запрос */
	string UrlRequest = R"(/data/2.5/forecast?)" + GetUrlWithParams(UrlPars);
	auto response = ClientObj.Get(UrlRequest.c_str());

	/* Отрабатываем возможные ошибки. */
	if (!response) {
		res.set_content("System error. Please contact with admin.", "text/plain");
		return;
	}
	
	string TemplateContent;
	json JsonFromServer;
	RTable.clear();

	switch (response->status) {
		case 404:
			RTable.push_back(make_pair("{error.message}", "Wrong city name!"));
			RTable.push_back(make_pair("{variable.state}", "true"));
		break;
		case 401:
			RTable.push_back(make_pair("{error.message}", "Wrong API key! Please contact with admin"));
			RTable.push_back(make_pair("{variable.state}", "true"));
		break;
		case 200:
			/*Парсим полученный ответ*/
			JsonFromServer = json::parse(response->body);
			RTable.push_back(make_pair("{city.name}", UrlPars["q"]));
			RTable.push_back(make_pair("{variable.state}", "false"));

			/*  Заполняем таблицу замены вхождений */
			for (size_t i = 0; i < HOURSCOUNT; i += HOURSPASSC)
			{
				RTable.push_back(make_pair("{list.dt}", JsonFromServer["list"][i]["dt_txt"].get<string>().substr(0, 10)));
				RTable.push_back(make_pair("{list.main.temp}", CutDouble(JsonFromServer["list"][i]["main"]["temp"].get<double>())));
				RTable.push_back(make_pair("{list.weather.icon}", JsonFromServer["list"][i]["weather"][0]["icon"].get<string>()));
			}
		break;
		default:
			RTable.push_back(make_pair("{error.message}", "Unknown system error! Please contact with admin."));
			RTable.push_back(make_pair("{variable.state}", "true"));
		break;
	}

	/*  Получаем шаблон из файла и заменяем вхождения */
	GetFContent(Config.cfg["templatepath"], &TemplateContent);

	for (auto replace : RTable) {
		StrReplace(&TemplateContent, replace.first, replace.second);
	}

	res.set_content(TemplateContent, "text/html");
	if (response->status == 200) Log("Запрос для города " + UrlPars["q"] + "!", false);
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
#ifndef DEV 
	if (!(argc > 1)) return EXIT_FAILURE;
#endif

	try{
#ifdef DEV
		Config.process("config.cfg");
#else
		Config.process(argv[1]);
#endif
	}
	catch (exception ex) {
		return EXIT_FAILURE;
	}

	if (Config.cfg.find("apikey") == Config.cfg.end()) return EXIT_FAILURE;
	if (Config.cfg.find("port") == Config.cfg.end()) return EXIT_FAILURE;
	if (Config.cfg.find("stdcity") == Config.cfg.end()) return EXIT_FAILURE;
	if (Config.cfg.find("templatepath") == Config.cfg.end()) return EXIT_FAILURE;
	if (![](string fn) { fstream f(fn); return f.is_open(); }(Config.cfg["templatepath"])) return EXIT_FAILURE;

	UrlPars["q"] = Config.cfg["stdcity"];
	UrlPars["appid"] = Config.cfg["apikey"];
	UrlPars["units"] = "metric";

	string UrlRequest = R"(/data/2.5/forecast?)" + GetUrlWithParams(UrlPars);
	auto response = ClientObj.Get(UrlRequest.c_str());

	if ((!response) or response->status != 200) return EXIT_FAILURE;

	ServerObj.Get("/", MakePage);
	thread ThListen([]() { ServerObj.listen("localhost", atoi(Config.cfg["port"].c_str())); });

	Log("Сервер запущен!");
	Log("Для завершения работы сервера введите \"Exit\"\n");

	string cmd;
	do{
		cin >> cmd;
	}while(cmd != "Exit");

	Log("Завершение работы...");
	ServerObj.stop();
	ThListen.join();
	return EXIT_SUCCESS;
}
```



## Заключение

В ходе проделанной работы:
- Были получены навыки работы с библиотекой httplib.
- Закреплены навыки замены слов, вхождений в тексте.
- Был повторен материал по десериализации JSON-формата.
- Был повторен материал по структуре HTTP запроса.