# user_segmentation_service

### Описание сервиса для управления сегментами пользователей

#### Цель проекта:
Разработка сервиса для управления сегментами пользователей с возможностью динамического распределения пользователей в сегменты. Сервис должен позволять создавать, удалять и редактировать сегменты, а также добавлять и удалять пользователей из сегментов. Также требуется реализовать функционал случайного распределения пользователей по сегментам на основе указанных процентных долей.

#### Задачи:
1. Разработка API для создания, удаления и редактирования сегментов.
2. Разработка механизма случайного распределения пользователей по сегментам с заданным процентом.
3. Реализация возможности добавления/удаления сегментов для конкретного пользователя.
4. Разработка API для получения списка сегментов, к которым относится конкретный пользователь.

#### Основные функции сервиса:
- Создание сегментов: Создание новых пользовательских сегментов, которые могут использоваться для различных целей экспериментов или акций.
- Удаление сегментов: Возможность удалять сегменты, которые больше не актуальны.
- Редактирование сегментов: Обновление сегментов с изменением их параметров или меток.
- Добавление/удаление пользователей в сегменты: Возможность включения пользователей в сегменты и их исключения из сегментов.
- Случайное распределение сегментов: Функция, позволяющая распределять сегменты среди пользователей на основе заданных процентных долей, например, распределение сегмента [MAIL_GPT] среди 30% пользователей.
- Получение списка сегментов пользователя: Эндпоинт для получения списка сегментов, в которых состоит пользователь, по его user_id.

#### Технологии:
- Язык программирования: C++
- Фреймворк: userver для работы с веб-сервисом
- База данных: PostgreSQL для хранения информации о пользователях и сегментах

#### Описание базы данных:
- Таблица `users`: хранит информацию о пользователях (user_id, данные пользователя).
- Таблица `segments`: содержит данные о сегментах (segment_id, имя сегмента, описание).
- Таблица `user_segments`: связывает пользователей с сегментами (user_id, segment_id).

#### Пример использования:
1. Создание сегмента MAIL_GPT.
2. Добавление 30% пользователей в сегмент MAIL_GPT.
3. Получение списка сегментов пользователя по user_id.

#### Результат:
Сервис предоставляет аналитикам и другим заинтересованным пользователям возможность легко управлять сегментами, добавлять и удалять пользователей, а также проводить эксперименты с различными функциями на основе сегментированной пользовательской базы.

## Makefile

Makefile contains typicaly useful targets for development:

* `make build-debug` - debug build of the service with all the assertions and sanitizers enabled
* `make build-release` - release build of the service with LTO
* `make test-debug` - does a `make build-debug` and runs all the tests on the result
* `make test-release` - does a `make build-release` and runs all the tests on the result
* `make service-start-debug` - builds the service in debug mode and starts it
* `make service-start-release` - builds the service in release mode and starts it
* `make` or `make all` - builds and runs all the tests in release and debug modes
* `make format` - autoformat all the C++ and Python sources
* `make clean-` - cleans the object files
* `make dist-clean` - clean all, including the CMake cached configurations
* `make install` - does a `make build-release` and runs install in directory set in environment `PREFIX`
* `make install-debug` - does a `make build-debug` and runs install in directory set in environment `PREFIX`
* `make docker-COMMAND` - run `make COMMAND` in docker environment
* `make docker-build-debug` - debug build of the service with all the assertions and sanitizers enabled in docker environment
* `make docker-test-debug` - does a `make build-debug` and runs all the tests on the result in docker environment
* `make docker-start-service-release` - does a `make install-release` and runs service in docker environment
* `make docker-start-service-debug` - does a `make install-debug` and runs service in docker environment
* `make docker-clean-data` - stop docker containers and clean database data

Edit `Makefile.local` to change the default configuration and build options.