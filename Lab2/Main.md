МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

​

### Отчёт по лабораторной работе № 2<br/> по дисциплине "Программирование"

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


## Цель лабораторной работы:
1. Овладеть практическими навыками разработки и программирования вычислительного процесса циклической структуры;
2. Сформировать навыков программирования алгоритмов разветвляющейся структуры;
3. Изучить операторы ветвления. Особенности использования полной и сокращенной формы оператора if и тернарного оператора.
   
## Задачи для выполнения лабораторной работы:
1. Написание на языке С++ программу которая для функции f(x) на интервале x ∈ [хнач; xкон]:
    - Вывыводит в консоль значения функции f(x) с шагом dx;
    - Определяет максимальное и минимальное значение функции.
2. Протабулировать функцию при **a = 0.7**, **b = 1.2**, **хнач = 0.5**, **xкон = 1.5**, **dx = 0.05**, записать получившиеся реультаты в отчёт в виде таблицы.
3. Определить минимальное и максимальное значение функции на указанном интервале, занести результаты в отчёт.
4. Используя сервис https://www.desmos.com/calculator:
   - Построить график функции
   - Добавить на график точки, полученные процессе табуляции.
   - Добавить на график точки минимума и максимума с метками значений.
   - Вставить скриншот получившегося графика в отчёт.

------

### Написание программы для табуляции кусочно-заданной функции

Настоящая программа разделена на два блока:
- Точка входа, ф-ия main, в которой происходит инициализация параметров и ввод данных с консоли.
- Отдельная функция, реализующая подсчет и вывод результата заданных функций.

Также используется структура `diapasone`, и метод для проверки дробей на равенство с заданной точностью, созданные в классе `"HumanCode"`, находящемся в двух файлах: `"HumanCode.h"` `"HumanCode.cpp"` исходный код которых представлен ниже.

В данном решение были использованы анонимные функции, позволяющие создать расширяемую систему для решения поставленной задачи, а также смежных для нее.

Функция main содержит 5 основных переменных:

|               Переменная              | Исполняемая функция в контексте программы|
|                   :---                |                   :---                                                                                 |
| `HC_::diapasone<ld> xdiapasone`       | Структура `diapasone` содержит две границы(`left_lim`, `right_lim`). В данной задачи реализация для отрезка                                                                                                                                    **X**нач **X**конеч.|
| `HC_::diapasone<ld> extremums`        | Та же структура, используемая для хранения точек экстремумов.                                          |
| `ld arguments[]`                      | Массив, содержащий аргументы передаваемые как в условия, так и в мат. функции(базис для расширяемости).|
| `vector<function<bool(ld[])>> cond`   | Вектор лямбда-функций, содержащих условия выполнения для мат. фукнций.                                 |
| `function<ld(ld[])> funcs[]`          | Массив лямбда-выражний, содержащих мат. функии, выполняющихся при истинности соотвествующего выражения                                                                                                                                  предыдущего пункта.|

#### Главный файл, содержащий всю реализацию программы.

```cpp
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <vector>
#include "Libs/HumanCode.h"

using namespace std;
#define HC_ HumanCode
typedef long double ld;
HumanCode HC(true);

template<typename T>
void func_tab(function<T(T[])> funcs[], HC_::diapasone<T> diapasone, T arguments[], vector<function<bool(T[])>> conditions, T step, HC_::diapasone<T>* extremums = nullptr, bool strict = false, int precision = 3){
	size_t state = 0;
	T Fvalue = 0.0;

	T it = diapasone.left_limit;
	if (strict) it += step;

	for (;; it += step){
		if (strict)  if (it >= diapasone.right_limit)	break;
		arguments[0] = it;
		
		if (!conditions[state](arguments)) { cout << "Следующая функция." << endl; state++; };

		try{
			Fvalue = funcs[state](arguments);
		}
		catch (exception ex){
			printf("Произошла ошибка при вычислении[%i] функции: %s", state + 1, ex.what());
			if (extremums != nullptr) {
				extremums->right_limit = 0.0;
				extremums->left_limit = 0.0;
			}
			return;
		}

		printf("X[%.*f]+(%.*f)\tF:%.*f\n", precision, it, precision, step, precision, Fvalue);
		
		if (extremums != nullptr) {
			if (Fvalue > extremums->right_limit) extremums->right_limit = Fvalue;
			if (Fvalue < extremums->left_limit)	extremums->left_limit = Fvalue;
		}
		
		if (!strict) if (it > diapasone.right_limit)	break;
	}
}

int main()
{
								/*Xнач Xконеч*/
	HC_::diapasone<ld> xdiapasone {0.0, 0.0};
								/*MIN MAX*/
	HC_::diapasone<ld> extremums{0.0, 0.0};
					 /* X    A    B */
	ld arguments[] = { 0.0, 0.0, 0.0};

	vector<function<bool(ld[])>> cond = {
		[](ld args[])->bool {
			if (args[0] <= args[1])	return true; else return false;
		},
		[](ld args[])->bool {
			if ((args[1] < args[0]) and (args[0] < args[2])) return true; else return false;
		},
		[](ld args[])->bool {
			if (args[0] >= args[2])	return true; else return false;
		},
	};

	function<ld(ld[])> funcs[] = {
		[](ld args[]) -> ld {
			return (ld)pow(M_E, args[0]);
		},
		[](ld args[]) -> ld {
			return (ld)(pow(M_E, args[0]) + cos(args[0]));
		},
		[](ld args[]) -> ld{
			return (ld)tan(args[0]);
		}
	};

	cout << "Введите X начальный: ";	cin >> xdiapasone.left_limit;
	cout << "Введите X конечный: ";		cin >> xdiapasone.right_limit;
	cout << "Введите A: ";				cin >> arguments[1];
	cout << "Введите B: ";				cin >> arguments[2];
	cout << "---------------------------" << endl;
	func_tab(funcs, xdiapasone,arguments, cond, (long double)0.05);
	
	return EXIT_SUCCESS;
}
```
#### Заголовочный файл `"HumanCode.h"`

```cpp
template<typename T>
struct diapasone {
    T left_limit;
    T right_limit;
};

bool static isequal(double first_var, double second_var, unsigned short int comparison_accuracy);
```

#### Файл `"HumanCode.cpp"`

```cpp
bool HumanCode::isequal(double f_var, double s_var, unsigned short int comp_acc) {
	long double diff;
	long long int comp_var;
	diff = f_var - s_var;
	for (int i = 0; i != comp_acc; i++) {
		diff = diff * 10;
	}
	comp_var = diff;
	if (comp_var == 0)	return true;
	else				return false;
}
```

<br>

-------

### Протабулировать функцию

Для выполнения табуляция используется функция с условиями:
<br><br>
<image src="Resources/Images/Image1.png" height="80px">

*Рисунок 1. Функция с параметрами*

Исходные данные: <br>

**`a = 0.7`**, **`b = 1.2`**<br>
**`хнач = 0.5`**, **`xкон = 1.5`**<br>
**`dx = 0.05`**

|     X     |   F(X)    |
|   :---:   |   :---:   |
|   0,500   |   1,649   |
|   0,550   |   1,733   |
|   0,600   |   1,822   |
|   0,650   |   1,916   |
|   0,700   |   2,014   |
|   0,750   |   2,849   |
|   0,800   |   2,922   |
|   0,850   |   3,000   |
|   0,900   |   3,081   |
|   0,950   |   3,167   |
|   1,000   |   3,259   |
|   1,050   |   3,355   |
|   1,100   |   3,458   |
|   1,150   |   3,567   |
|   1,200   |   2,572   |
|   1,250   |   3,010   |
|   1,300   |   3,602   |
|   1,350   |   4,455   |
|   1,400   |   5,798   |
|   1,450   |   8,238   |
|   1,500   |   14,101  |

**F***min* = 1,649<br>
**F***max* = 14,101<br>
<br>

### Построение графика функций

Создадим график и установим точки на промежуточных значениях, полученных в результате программы, а также установим точки минимума и максимума.

Оранжевым цветом обозначены экстремумы. Сиреневым - промежуточные значений.

<image src="Resources/Images/Image3.png" height="700">

*Рисунок 2. График функций.*


## Вывод

В ходе проделанной работы были изучены основы построения графиков, в особенности отрезочных функцияй средствами сервиса **desmos.com**
Были отработы и закреплены на практике знания о лямбда-функциях и математических вычислениях при помощи языков программировани, в частности **C++**