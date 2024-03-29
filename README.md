[![Build Status](https://travis-ci.com/Toliak/lab0302.svg?branch=master)](https://travis-ci.com/Toliak/lab0302)
![Code size](https://img.shields.io/github/languages/code-size/Toliak/lab0302.svg)
![GitHub Repo size](https://img.shields.io/github/repo-size/Toliak/lab0302.svg)

# Лабораторная работа №2

> Кэш - промежуточный буфер с быстрым доступом, содержащий информацию, которая может быть запрошена с наибольшей вероятностью. Доступ к данным в кэше осуществляется быстрее, чем выборка исходных данных из более медленной памяти или удаленного источника, однако её объём существенно ограничен по сравнению с хранилищем исходных данных.

### Задача

Для каждого из вариантов проходов (*прямой*, *обратный* и *случайный*) целочисленного массива 
провести исследование зависимости времени от размера.

Каждое исследование включает в себя серию эксперементов c определенными размерами.

Количество экспериментов в серии определяется следующим образом:

```cpp
1/2 * cache_sizes['1'] < 2^x < 2^(x+1) < ... < 2^(x+n) < 3/2 * cache_sizes['max']
```

### Пример

В примере ниже показано, что для процессора с тремя уровнями кэша (`2mb`, `4mb`, `8mb`)
необходимо провести серию из 5 эксперементов.

```cpp
cache_size['1'] = 2 mb;
cache_size['2'] = 4 mb;
cache_size['3'] = 8 mb;

// 1mb < 2mb < 4mb < 8mb < 12mb
```

### Эксперимент

Каждый эксперемент состоит из 3 шагов:

```cpp
1. Создание буфера
2. Прогревание кэша
// <-- время начала эксперемнта
3. Циклический проход (1000 итераций)
// <-- время окончание эксперемента
```

#### Шаг 1

Инициализация буфера может происходит, как с помощью чтения данных из файла в выделенную область памяти,
так и с помощью случайного заполнения с использованием генератора случайных чисел.

#### Шаг 2

Данный шаг необходимо выполнить для получения репрезентативных данных, т.к. кэш-память еще не заполнена.

#### Шаг 3

Для получения времени обхода от размера массива процедуру прохода необходимо многократно повторить (порядка 1000 раз).

### Результаты

Ниже представлен формат и пример отчета:

```yaml
- investigation:                                     |  - investigaion:
  travel_variant: <вариант_прохода>                  |    travel_order: "direction"
  experiments:                                       |    experiments:
  - experiment:                                      |    - experiment:
      number:                                        |        number: 1
      input_data:                                    |        input_data:
        buffer_size: <размер_буфера>                 |          buffer_size: "1mb"
      results:                                       |        results:
        duration: <продолжительность>                |          duration: "1ns"
  - experiment:                                      |    - experiment:
      number: <номер_эксперимента>                   |        number: 2
      input_data:                                    |        input_data:
        buffer_size: <размер_буфера>                 |          buffer_size: "2mb"
      results:                                       |        results:
        duration: <продолжительность>                |          duration: "2ns"
                                                     |
- investigation:                                     |  - investigation:
...                                                  |  ...
```

### Отчет

#### Intel Core i7-9750H

Эксперимент 1:
[Отчет](reports/report_1_i7_9750h.yml), 
[График](https://jsfiddle.net/xfvz3k89/)

Эксперимент 2:
[Отчет](reports/report_1_i7_9750h.yml), 
[График](https://jsfiddle.net/4tf7Lhuj/)

#### Intel Core i7-7700HQ

Эксперимент 1:
[Отчет](reports/report_1_i7_7700hq.yml), 
[График](https://jsfiddle.net/v36n2zja/)

Эксперимент 2:
[Отчет](reports/report_2_i7_7700hq.yml), 
[График](https://jsfiddle.net/qsuLkp4v/)

#### Intel Core i7-7700

Эксперимент 1:
[Отчет](reports/report_1_i7_7700.yml), 
[График](https://jsfiddle.net/10mjn5cp/)
