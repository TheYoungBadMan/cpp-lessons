# Пространства имен

В языке C++ появилась возможность группировать переменные, функции и классы в *пространства имен* (`namespace`). Пространство имен — это область, в которой имена объектов должны быть уникальными и не конфликтовать с другими аналогичными именами в программе или сторонних библиотеках.

Основная цель пространств имен:
1. **Избегать конфликтов имен** при использовании нескольких библиотек или модулей.
2. **Логически** группировать связанные между собой сущности (классы, функции, переменные).

Важно понимать, что пространства имен — это средство **компиляции и организации кода**; они **не влияют** на двоичный код напрямую, но упрощают разбор и разрешение имен на этапе компиляции.


## Объявление пространства имен

Синтаксис объявления пространства имен следующий:

```cpp
namespace имя_пространства {
	// объявления
}
```

Пример:

```cpp
namespace my_namespace {
	int a = 5;
	void foo() {
		std::cout << "Hello, world!" << std::endl;
	}
}
```

### Вложенные пространства имен

Пространства имен могут быть вложены друг в друга, создавая иерархию:

```cpp

namespace outer {
	namespace inner {
		int a = 5;
	}
}
```

### Пространства имен внутри классов/структур

Хотя язык позволяет помещать namespace внутрь классов или структур, на практике это встречается крайне редко и обычно не рекомендуется:

```cpp
class MyClass {
public:
	namespace inner {
		int a = 5;
	}
};
```

*Примечание*: сами классы и структуры представляют собой пространства имен, хотя, обычно это не подразумевается явно.

## Оператор разрешения области видимости `::`

Для доступа к объектам внутри пространства имен используется оператор разрешения области видимости `::`. Пример:

```cpp
namespace my_namespace {
	int a = 5;
	void foo() {
		std::cout << "Hello, world!" << std::endl;
	}
}

int main() {
	my_namespace::foo();
	std::cout << my_namespace::a << std::endl;
}
```

Если пространство имен вложено в другое, доступ осуществляется последовательным применением оператора `::`:

```cpp
namespace outer {
	namespace inner {
		int a = 5;
	}
}

int main() {
	std::cout << outer::inner::a << std::endl;
}
```

Если внутри пространства имен нужно использовать объекты из глобальной области видимости, можно применить оператор `::` без имени:

```cpp
int a = 5;

namespace my_namespace {
	int a = 10;
	void foo() {
		std::cout << a << std::endl; // а из текущей пространства
		std::cout << ::a << std::endl; // а из глобального пространства
	}
}
```

## Анонимные пространства имен

***Анонимные пространства*** имен не имеют имени и объявляются следующим образом:

```cpp

namespace {
	int a = 5;
}

int main() {
	std::cout << a << std::endl;
}
```

Смысл анонимного пространства в том, что все его объекты имеют внутреннее связывание (internal linkage), то есть видны только в пределах данного файла (translation unit). Ранее в C для похожей цели использовали static на глобальном уровне, но в C++ предпочитают анонимные пространства имен.

```cpp
// example.h
namespace {
	int a = 5;
}

// example.cpp
#include "example.h"

int main() {
	std::cout << a << std::endl; // Ошибка компиляции: a не видна вне файла example.cpp
}
```

## Директива `using`

Для упрощения доступа к объектам внутри пространства имен можно использовать ключевое слово `using`. Оно позволяет использовать объекты без указания имени пространства имен. 

По своей сути, использование `using namespace` равносильно извлечению всех объектов из пространства имен и вставке их в текущую область видимости.

Пример использования:

```cpp
namespace my_namespace {
	int a = 5;
	void foo() {
		std::cout << "Hello, world!" << std::endl;
	}
}

int main() {
	using namespace my_namespace;
	foo();
	std::cout << a << std::endl;
}
```

Также можно использовать ключевое слово `using` для доступа к конкретному объекту:

```cpp
namespace my_namespace {
	int a = 5;
	void foo() {
		std::cout << "Hello, world!" << std::endl;
	}
}

int main() {
	using my_namespace::foo;
	foo();
}
```

Использование `using namespace` бывает полезным, если часто приходится обращаться к объектам из одного пространства имен или если имена объектов в пространстве имен слишком длинные из-за вложенности.

Следует быть осторожным с использованием ключевого слова `using`, так как это может привести к конфликтам, если в текущей области видимости уже есть объекты с такими же именами. Пример конфликта:

```cpp
namespace my_namespace {
	int a = 5;
}

int a = 10;

int main() {
	using namespace my_namespace;
	std::cout << a << std::endl; // Конфликт имен: a из my_namespace или a из глобальной области видимости?
}
```

## Псевдонимы пространств имен

Если пространство имен имеет слишком длинное имя или содержит глубокую вложенность, его можно сократить, создав псевдоним следующим образом:

```cpp
namespace outer {
	namespace inner {
		int a = 5;
	}
}

namespace oi = outer::inner;

int main() {
	std::cout << oi::a << std::endl;
}
```

## Inline-пространства имен (`C++11`)

*Inline-пространства имен* позволяют версионировать код, при этом объекты "встроенного" 
пространства имен видны так, как будто они объявлены во внешнем пространстве имен:

```cpp
namespace my_namespace {
	inline namespace v1 {
		void foo() {}
	}
	namespace v2 {
		void foo() {}
	}
}

int main() {
	my_namespace::foo(); // вызовется foo из v1
	my_namespace::v2::foo(); // вызовется foo из v2
}
```

Это позволяет обеспечить обратную совместимость кода при добавлении новых функций или изменении старых.

## Пространства имен `std`

Все объекты стандартной библиотеки C++ (например, `std::cout`, `std::string`) находятся в пространстве имен `std`. Встречается директива using namespace std;, которая позволяет писать cout вместо `std::cout`, но в больших проектах такой подход не рекомендуется из-за возможных конфликтов имен.

```cpp
#include <iostream>

int main() {
	std::cout << "Hello, world!" << std::endl;
}
```

## Итоги и рекомендации

1. Используйте пространства имен, чтобы избежать конфликтов имен и структурировать код.
2. В анонимном пространстве имен размещайте объекты, которые должны быть видны только в рамках одного .cpp-файла.
3. Помните о конфликте имен при использовании `using namespace`.
4. Не используйте using namespace в заголовках — это может ломать чужой код.
5. Псевдонимы (`alias`) упрощают работу с вложенными пространствами имен.
6. Inline namespace полезен для версионирования кода.