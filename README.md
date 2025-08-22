**Цифровой БИХ фильтр**

Принцип работы:

Принимает 2 байта из файла и отфильтровывает полученные данные в 3 шага с учётом влияния результата на предыдущем шаге алгоритма.

В листинге 1 представлен алгоритм цифрового БИХ фильтра.

Листинг 1 – Алгоритм цифрового БИХ фильтра

//First addition

add <= (ACoef0 \* DI - BCoef0 \* r\_old\_data);

intermed <= add\[ 38 : 23 \];

//Second addition

add <= (ACoef1 \* DI - BCoef1 \* intermed);

intermed <= add\[ 38 : 23 \];

//Third addition

add <= (ACoef2 \* DI - BCoef2 \* intermed);

r\_DO <= add\[ 38 : 23 \];

<img width="928" height="794" alt="Снимок" src="https://github.com/user-attachments/assets/a6bd1469-baf4-494f-85f3-e28e7b8611ef" />

Полученная RTL схема:
![RTL_page-0001](https://github.com/user-attachments/assets/a9eb87eb-46d3-4b88-94f9-58e5bab5ae2c)
