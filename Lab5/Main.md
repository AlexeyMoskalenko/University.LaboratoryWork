МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

​

### Отчёт по лабораторной работе № 4<br/> по дисциплине "Программирование"

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

# Работа с текстовыми файлами и CSV-таблицами

## Цель:

- Научиться работать с текстовыми файлами;
- Закрепить навыки работы со структурами.

------

## Теоретическая часть

Для выполнения лабораторной работы был создан класс CSVReader и вспомогательный класс StringEditor.
#### CSVReader
В классе CSVReader были созданы два метода:

- Реализующий конвертацию файла в таблицу
```cpp
CSVArray convertFile(std::ifstream& filePointer, const char delimiter = '\r', bool consistHeaderRaw = true);
```

- Реализующий вспомогательную функцию конвертации отдельной строки в вектор строк
```cpp
CSVVector stringToVector(std::string stringToConvert);
```

- Также были созданы алиасы для сокращения написания кода и более наглядного поведения между методами объекта.
```cpp
typedef std::vector<std::string> CSVVector;
typedef std::map<std::string, std::string> CSVRow;
typedef std::vector<CSVRow> CSVTable;
```
#### StringEditor

Данный класс является помощником для обработки строк. <br>
А именно, реализует методы, удаляющие пробелы из начала, конца и с обоих сторон строки.


```cpp 
class StringEditor {
public:
	static inline void ltrim(std::string& s);

	static inline void rtrim(std::string& s);

	static inline void trim(std::string& s);
};
```

-------

## Ссылка на конечный результат работы программы
Файл, содеражщий итоги анализа таблицы - [Output.csv](Resources/output.csv)

-------


## Исходный код программы


### Содержание заголовочного файла "CSVReader.h"

```cpp
#pragma once
#include <vector>
#include <regex>
#include <string>
#include <map>
#include <fstream>
#include <stdexcept>

typedef std::vector<std::string> CSVVector;
typedef std::map<std::string, std::string> CSVRow;
typedef std::vector<CSVRow> CSVTable;

class StringEditor {
public:
	static inline void ltrim(std::string& s);

	static inline void rtrim(std::string& s);

	static inline void trim(std::string& s);
};

class CSVReader {
public:
	CSVTable static convertFile(std::ifstream& filePointer, const char delimiter = '\r', bool consistHeaderRaw = true);

	CSVVector static stringToVector(std::string stringToConvert);

	template<typename T>
	std::string static toCsvStr(T str) {
		std::string ret("\"" + std::to_string(str) + "\"");
		return ret;
	}

	std::string static toCsvStr(const char str[]) {
		std::string ret(str);
		ret = "\"" + ret + "\"";
		return ret;
	}

	std::string static toCsvStr(std::string str) {
		std::string ret = "\"" + str + "\"";
		return ret;
	}

	std::string static implode(std::vector<std::string> implodeVector, std::string glue) {
		std::string returnString;
		for (auto it : implodeVector) {
			returnString += it + glue;
		}
		return returnString.substr(0, returnString.length() - glue.length());
	}

	template<typename T>
	std::string static implode(std::vector<T> implodeVector, std::string glue) {
		std::string returnString;
		for (auto it : implodeVector) {
			returnString += std::to_string(it) + glue;
		}

		return returnString.substr(0, returnString.length() - glue.length());
	}
};
```

### Содержание файла "CSVReader.cpp"

```cpp
#include "CSVReader.h"

void StringEditor::ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
};

void StringEditor::rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

void StringEditor::trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

CSVTable CSVReader::convertFile(std::ifstream& filePointer, const char delimiter, bool consistHeaderRaw) {
	if (!filePointer.is_open()) throw std::invalid_argument("File is not opened");

	CSVTable CSVTable;

	CSVVector headersVector;
	CSVVector nextVector;
	std::string nextString;

	if (filePointer.eof()) return CSVTable;

	if (consistHeaderRaw) {
		std::getline(filePointer, nextString, delimiter);
		headersVector = CSVReader::stringToVector(nextString);
	}
	else {
		std::getline(filePointer, nextString, delimiter);
		nextVector = CSVReader::stringToVector(nextString);
		for (auto it : nextVector)
			headersVector.push_back(std::to_string(headersVector.size() + 1));
	}

	try {
		while (!filePointer.eof()) {
			std::getline(filePointer, nextString, delimiter);
			nextVector = CSVReader::stringToVector(nextString);
			CSVRow newRow;

			for (int i = 0; i < headersVector.size(); i++)
			{
				auto keyValuePair = std::make_pair(headersVector.at(i), nextVector.at(i));
				newRow.insert(newRow.end(), keyValuePair);
			}

			CSVTable.insert(CSVTable.end(), newRow);
		}

		return CSVTable;
	}
	catch (std::out_of_range& ex) {
		throw new std::invalid_argument("File data struct was corrupted. Headers and data content are differrent.");
	}
};


CSVVector CSVReader::stringToVector(std::string stringToConvert) {
	std::vector<std::string> cellValuesVector;
	std::string CSVRegExp(R"((?:,|\n|^)("(?:(?:"")*[^"]*)*"|[^",\n]*|(?:\n|$)))");
	std::smatch searchResult;
	std::regex RegExp(CSVRegExp, std::regex::ECMAScript);

	stringToConvert.insert(0, ",");

	auto begin = std::sregex_iterator(stringToConvert.begin(), stringToConvert.end(), RegExp);
	auto end = std::sregex_iterator();

	for (std::sregex_iterator it = begin; it != end; it++) {
		std::string nextCellValue = it->str().substr(1, std::string::npos);
		StringEditor::trim(nextCellValue);

		if (nextCellValue.length() > 1)
			if (nextCellValue[0] == '\"' and nextCellValue[nextCellValue.length() - 1] == '\"')
				nextCellValue = nextCellValue.substr(1, nextCellValue.length() - 2);

		cellValuesVector.push_back(nextCellValue);
	}

	return cellValuesVector;
};
```

### Содержание файла точки входа в программу

```cpp
#include <iostream>

#include "CSVReader.h"

using namespace std;

int main(){

	string inputFileName  = R"(..\Resources\train.csv)";
	string outputFileName = R"(..\Resources\output.csv)";

	ifstream inputFileStream(inputFileName);
	ofstream outputFileStream(outputFileName);

	struct {
		uint32_t survivedCommonC = 0;
		uint32_t survivedFromFirstStClass = 0;
		uint32_t survivedFromSecStClass = 0;
		uint32_t survivedFromThirdStClass = 0;
		uint32_t survivedWomens = 0;
		uint32_t survivedMans = 0;
		struct {
			
			struct {
				uint32_t counter = 0;
				uint64_t totalAge = 0;
			}man;
			
			struct {
				uint32_t counter = 0;
				uint64_t totalAge = 0;
			}woman;
			
			struct {
				uint32_t counter = 0;
				uint64_t totalAge = 0;
			}common;

		} averageAge;
		vector<std::string> underagePIDsList;
	}Statistics;

	if (inputFileStream.is_open() and outputFileStream.is_open()) {
		auto passangersTable = CSVReader::convertFile(inputFileStream);
		for (auto passanger : passangersTable) {
			if (std::stoi(passanger["Survived"])){
				Statistics.survivedCommonC++;

				if (!passanger["Sex"].empty()) {
					if (passanger["Sex"] == "female")
						Statistics.survivedWomens++;
					if (passanger["Sex"] == "male")
						Statistics.survivedMans++;
				}

				
				if (!passanger["Pclass"].empty()){
					if (std::stoi(passanger["Pclass"]) == 1)
						Statistics.survivedFromFirstStClass++;
					if (std::stoi(passanger["Pclass"]) == 2)
						Statistics.survivedFromSecStClass++;
					if (std::stoi(passanger["Pclass"]) == 3)
						Statistics.survivedFromThirdStClass++;
				}
			}
			if (!passanger["Age"].empty()) {
				if (passanger["Sex"] == "female") {
					Statistics.averageAge.woman.counter++;
					Statistics.averageAge.woman.totalAge += std::stoi(passanger["Age"]);
				}
				if (passanger["Sex"] == "male") {
					Statistics.averageAge.man.counter++;
					Statistics.averageAge.man.totalAge += std::stoi(passanger["Age"]);
				}

				Statistics.averageAge.common.totalAge += std::stoi(passanger["Age"]);

				if (std::stoi(passanger["Age"]) < 18)
					Statistics.underagePIDsList.push_back(passanger["PassengerId"]);
			}
		}

		Statistics.averageAge.common.counter = passangersTable.size();


		auto sortPredicate = [](std::string f, std::string s) {
			return std::stoi(f) > std::stoi(s) ? false : true;
		};

		std::sort(Statistics.underagePIDsList.begin(), Statistics.underagePIDsList.end(), sortPredicate);

		std::string fileOutputResult =
			"Характеристика, Результат \n" +
			CSVReader::toCsvStr("Общее число выживших")		+ "," + to_string(Statistics.survivedCommonC) + "\n" +
			CSVReader::toCsvStr("Выжившие первого класса")	+ "," + to_string(Statistics.survivedFromFirstStClass) + "\n" +
			CSVReader::toCsvStr("Выжившие второго класса")	+ "," + to_string(Statistics.survivedFromSecStClass) + "\n" +
			CSVReader::toCsvStr("Выжившие третьего класса")	+ "," + to_string(Statistics.survivedFromThirdStClass) + "\n" +
			CSVReader::toCsvStr("Выжившие женщины")			+ "," + to_string(Statistics.survivedWomens) + "\n" +
			CSVReader::toCsvStr("Выжившие мужчины")			+ "," + to_string(Statistics.survivedMans) + "\n" +
			CSVReader::toCsvStr("Средний возраст")			+ "," + to_string(Statistics.averageAge.common.totalAge / Statistics.averageAge.common.counter) + "\n" +
			CSVReader::toCsvStr("Средний возраст жен.")		+ "," + to_string(Statistics.averageAge.woman.totalAge / Statistics.averageAge.woman.counter) + "\n" +
			CSVReader::toCsvStr("Средний возраст муж.")		+ "," + to_string(Statistics.averageAge.man.totalAge / Statistics.averageAge.man.counter) + "\n" + 
			CSVReader::toCsvStr("Список пассажиров")		+ "," + CSVReader::toCsvStr(CSVReader::implode(Statistics.underagePIDsList, ",")) + "\n"
		;
	

		outputFileStream << fileOutputResult;

	}
	else {
		cout << "Input or output file was not oppened";
	}
	
	return EXIT_SUCCESS;
}
```


## Выходная таблица данных

| Характеристика			| Результат			|
|:---------------:			| :---------------:	|
|Общее число выживших		|	342				|
|Выжившие первого класса	|	136				|
|Выжившие второго класса	|	87				|
|Выжившие третьего класса	|	119				|
|Выжившие женщины			|	233				|
|Выжившие мужчины			|	109				|
|Средний возраст			|	23				|
|Средний возраст жен		|	27				|
|Средний возраст муж		|	30				|

Список пассажиров младше 18 лет:<br>
8,10,11,15,17,23,25,40,44,51,59,60,64,69,
72,79,85,87,112,115,120,126,139,148,157,164,
165,166,172,173,183,184,185,194,206,209,221,
234,238,262,267,279,283,298,306,308,330,334,
341,349,353,375,382,387,390,408,420,434,436,
446,447,449,470,480,481,490,501,505,531,533,
536,542,543,550,551,575,619,635,643,645,684,
687,690,692,721,722,732,747,751,752,756,765,
778,781,782,788,789,792,803,804,814,820,825,
828,831,832,842,845,851,853,854,870,876|

------

## Заключение

В ходе проделанной лабораторной работы были получены навыки работы с STL-регулярными выражениями.
На практике были отработаны навыки применения векторов, карт и иных контейнером библиотеки стандартных шаблонов.
Были отработаны навыки создания лямбда-выражений. 