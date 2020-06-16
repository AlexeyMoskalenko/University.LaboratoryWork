МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

​

### Отчёт по лабораторной работе № 3<br/> по дисциплине "Программирование"

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

# Создание дешифровщика текста из изображения

## Цель:

- Закрепить навыки разработки программ использующих операторы цикла;
- Закрепить навыки разработки программ использующих массивы;
- Освоить методы подключения сторонних библиотек.

## Теоретическая часть

Существует исходный файл-изображение в который зашифровано некоторое сообщение.<br>
В каждом канале, каждого пикселя начиная с 0 до N в младшем бите находится единица или ноль.<br><br>
**Задача:** Расшифровать сообщение, получив информацию с каждого пикселя исходной картинки Рисунок 1 при помощи заданного ключа.

**Исходный ключ:**
`00r 00g 00b 10r 10g 10b 01r 01g`

<image src="MVSProject\Resources\pic2.bmp" height="300px"><br>
*Рисунок 1. Исходное изображение.*

## Практическая часть

Скачиваем и подключаем библиотеку [**libbmp**](https://github.com/marc-q/libbmp)

При помощи ключа создаем алгоритм получения младшего бита с каждого пикселя.
<br><br><br>
Подключаемые библиотеки:
```cpp
#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
```

Дальнейшие алгоритмы находятся в пространстве имен **`ImageDecryptor`**:


- Алгоритмы перемещения "указателя" на пиксель:
```cpp
class MovementBehavior::BehaviorLines{}
```

- Алгоритмы перемещения "указателя" на канал:
```cpp
class MovementBehavior::BehaviorRGB{}
```

- Алгоритмы получения байта указанного канала, указанного пикселя:
```cpp
class ImageWorker::BitReader{}
```

- Вспомогательные элементы для работы классов
```cpp
struct 	Point{}      					// Точка для получения информации из цветовых каналов.

enum 	COLOR{}        					// Перечисление для перебора каналов цвета.

enum 	MovementBehavior::ORIENT{}      // Ориентация при проходе по изображению. Горизонтальная / Вертикальная.
```

<br><br><br>
Полученный текст:

```
John Adams (October 30, 1735[a] – July 4, 1826) was an American statesman, attorney, diplomat, writer, and Founding Father who served as the second president of the United States, from 1797 to 1801. Before his presidency, he was a leader of the American Revolution that achieved independence from Great Britain and served as the first vice president of the United States. Adams was a dedicated diarist and regularly corresponded with many important figures in early American history, including his wife and adviser, Abigail. His letters and other papers serve as an important source of historical information about the era.
A lawyer and political activist prior to the revolution, Adams was devoted to the right to counsel and presumption of innocence. He defied anti-British sentiment and successfully defended British soldiers against murder charges arising from the Boston Massacre. Adams was a Massachusetts delegate to the Continental Congress and became a principal leader of the Revolution. He assisted in drafting the Declaration of Independence in 1776 and was its foremost advocate in Congress. As a diplomat in Europe, he helped negotiate the peace treaty with Great Britain and secured vital governmental loans. Adams was the primary author of the Massachusetts Constitution in 1780, which influenced the United States' own constitution, as did his earlier Thoughts on Government.
Adams was elected to two terms as vice president under President George Washington and was elected as the United States' second president in 1796. He was the first, and only, president elected under the banner of the Federalist Party. During his single term, Adams encountered fierce criticism from the Jeffersonian Republicans and from some in his own Federalist Party, led by his rival Alexander Hamilton. Adams signed the controversial Alien and Sedition Acts and built up the Army and Navy in the undeclared "Quasi-War" with France. The main accomplishment of his presidency was a peaceful resolution of this conflict in the face of public anger and Hamilton's opposition. During his term, he became the first president to reside in the executive mansion now known as the White House.
In his bid for reelection, opposition from Federalists and accusations of despotism from Republicans led to Adams's loss to his former friend Thomas Jefferson, and he retired to Massachusetts. He eventually resumed his friendship with Jefferson by initiating a correspondence that lasted fourteen years. He and his wife generated a family of politicians, diplomats, and historians now referred to as the Adams political family, which includes their son John Quincy Adams, the sixth president of the United States. John Adams died on July 4, 1826 – the fiftieth anniversary of the adoption of the Declaration of Independence – hours after Jefferson's death. Surveys of historians and scholars have favorably ranked his administration.
```

## Исходный код

```cpp
#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
#include <functional>
#include "Libs/libbmp/libbmp.h"

using namespace std;

#ifdef _DEBUG
#define DEVMACRO
#endif
// SET INPUT CHARSET LEN IN BYTE
#define INPUT_CHARSET_LEN_BYTE		1
// SET OUPUT CHAR TYPE OF STRING
#define OUTPUT_CHAR_TYPE			char

#pragma region SystemDefines
#define BYTELEN 8
#define INPUT_CHARSET_LEN_BIT		INPUT_CHARSET_LEN_BYTE * BYTELEN 
#define OUTPUT_CHARSET_LEN_BIT		sizeof(OUTPUT_CHAR_TYPE) * 8
#pragma endregion

typedef bitset<BYTELEN> byte_t;

typedef bitset<OUTPUT_CHARSET_LEN_BIT> char_ex;

namespace ImageDecryptor {

	#define getName(var)  #var 

	struct Point {
		int x;
		int y;
	};

	enum COLOR
	{
		RED = 1,
		GREEN,
		BLUE
	};

	namespace MovementBehavior {

		enum ORIENT {
			HORIZONTAL = 0,
			VERTICAL = 1,
		};

		class BehaviorLines {
		private:
			Point	Coords{ 0,0 };
			int		Max;
			bool	Behavior = 0;
		public:
			BehaviorLines(unsigned long Max, ORIENT Orientation = HORIZONTAL) {
				this->Behavior = Behavior;
				this->Max = Max;
			}

			BehaviorLines() {
				throw std::bad_function_call();
			}

			Point Next() {
				if (this->Behavior == HORIZONTAL) {
					if (Coords.x == this->Max - 1) {
						Coords.x = 0;
						Coords.y++;
					}
					else Coords.x++;
				}
				else {
					if (Coords.y == this->Max - 1) {
						Coords.y = 0;
						Coords.x++;
					}
					else Coords.y++;
				}
				return this->Coords;
			}

			Point Get() {
				return this->Coords;
			}
		};

		class BehaviorRGB {
		private:
			unsigned char StateCounter = BLUE;
		public:
			COLOR Next() {
				if (this->StateCounter == BLUE) this->StateCounter = RED; else this->StateCounter++;
				//cout << StateCounter;
				return (COLOR)this->StateCounter;
			}

			COLOR Get() {
				return (COLOR)this->StateCounter;
			}
		};
	}

	namespace ImageWorker {
		class BitReader {
		private:
			BmpImg Image;
		public:

			BitReader(BmpImg Image) {
				this->Image = Image;
			}

			BitReader() {
				throw new std::bad_function_call;
			}

			unsigned char GetPixel(Point PixelPoint, COLOR ColorChanel){
				switch (ColorChanel)
				{
				case RED:
					return this->Image.red_at(PixelPoint.x, PixelPoint.y);
					break;
				case GREEN:
					return this->Image.green_at(PixelPoint.x, PixelPoint.y);
					break;
				case BLUE:
					return this->Image.blue_at(PixelPoint.x, PixelPoint.y);
					break;
				default:
					throw invalid_argument(getName(ColorChanel));
				}
			}
		};
	}
}

BmpImg Image;

ofstream OutputFile;

int main(int argc, char* argv[])
{
	using namespace ImageDecryptor;

	string FileName = "";
#ifdef DEVMACRO
	if (Image.read(R"(Resources\pic2.bmp)") != BmpError::BMP_OK) return EXIT_FAILURE;
	FileName = R"(Resources\decoded_text.txt)";
#else
	if (argc < 3) return EXIT_FAILURE;
	if (Image.read(argv[1]) != BmpError::BMP_OK) return EXIT_FAILURE;
	FileName = argv[2];
#endif // !DEV MACRO

	OutputFile.open(FileName);
	if (!OutputFile.is_open()) return EXIT_FAILURE;

	MovementBehavior::BehaviorLines		MovemementBehavior(Image.get_width(), MovementBehavior::HORIZONTAL);
	MovementBehavior::BehaviorRGB		RGBBehavior;
	ImageWorker::BitReader				BitReader(Image);

	std::basic_string<OUTPUT_CHAR_TYPE> DecodedText = "";
	vector<bool> RawBitsArray;

	short int NullTerminatedCounter = 0;

	while(NullTerminatedCounter < INPUT_CHARSET_LEN_BIT){

		for (long int it = 0; it < 3; it++){
			byte_t colorbyte = BitReader.GetPixel(MovemementBehavior.Get(), RGBBehavior.Next());

			RawBitsArray.push_back(colorbyte[0]);
			
			if (!colorbyte[0]) NullTerminatedCounter++; else NullTerminatedCounter = 0;
		}

		MovemementBehavior.Next();
	}

	for (size_t i = 0; i < RawBitsArray.size() - OUTPUT_CHARSET_LEN_BIT; i+= OUTPUT_CHARSET_LEN_BIT){
		char_ex newchar;
		for (size_t k = 0; k < OUTPUT_CHARSET_LEN_BIT; k++)
		{
			newchar.set(OUTPUT_CHARSET_LEN_BIT-1-k, RawBitsArray[i + k]);
		}
		DecodedText += (OUTPUT_CHAR_TYPE)newchar.to_ulong();
	}

	OutputFile << DecodedText;

	cout << "Saved as: " << FileName;

	return EXIT_SUCCESS;
}
```

## Вывод

В результате проделанной работы были приобретены навыки работы с изображениями средствами подключаемых библиотек.<br>
Приобретены навыки работы с битовыми типами данных.<br>
Отработаны навыки создания и проектирования Объектно-ориентированной модели программирования. 