# qGiro  
#### Author: Oleg  M.Kosorukov  
#### Версия 0.3

#### Программа обработки данных при ориентировании подземной маркшейдерской основы.

Ориентирование стороны подземной полигонометрии выполняется при помощи гиротеодолита с двумя гироблоками и состоит из:
- определения поправки гиротеодолита на стороне с известным дирекционным углом (работа на поверхности);
- определения дирекционного угла ориентируемой стороны подземной полигонометрии (работа в строящемся тоннеле);
- повторного определения поправки гиротеодолита на стороне с известным дирекционным углом (работа на поверхности).
Все измерения выполняются каждым гироблоком.

#### Методика
Определение поправки гиротеодолита выполнялось до и после определения гироскопического азимута стороны подземной полигонометрии, на сторонах наземной полигонометрии.
Сущность определения постоянной поправки (∆) состоит в определении угла между известным дирекционным углом наземной полигонометрии планового опорного обоснования и гироскопическим азимутом, определенным гиротеодолитом.
Определение гироскопического азимута сторон выполнялось в соответствии с п.8.30. ВСН 160-69, глава 6.3.2 СП 120.13330.2012 " Метрополитены. Актуализированная редакция СНиП 32-02-2003". 

Определение поправки гиротеодолита производилось непосредственно на пунктах планового обоснования, а гироскопического азимута стороны подземной полигонометрии выполняется «внецентренно», т.е. гиротеодолит устанавливался не над пунктом подземной полигонометрии, а в произвольной точке в непосредственной близости от него. При определении азимута измеряются направления на пункты полигонометрии и расстояния до них. В азимут Агир(вц) вносится поправка ∆А.

В результате выполненных работ должны быть определены гироскопические азимуты сторон подземной полигонометрии прямого и обратного направления. Расхождения значений вычисленного дирекционного угла подземной линии, определенных из нескольких ориентирований, не должны превышать 20″ (п.8.58. ВСН 160-69, глава 6.3.2 СП 120.13330.2012 " Метрополитены. Актуализированная редакция СНиП 32-02-2003".

#### Вкладка "Оформление":
Содержит информацию о дате измерения, номер гиротеодолита и обоих гироблоков. 
- Тема - не используется
- Письмо - Номер письма от даты и название компании приславшая заявку на выполнение
- Колонтитул - Информация помещаемая в колонтитул
- Введение - Информация помещаемая в начале отчета, где приводится основная информация

#### Вкладка "Наземные измерения":
- Широта - Географическая широта, с точностью до 4 знака
- Пункты наземной полилигонометрии, координаты и измеренный азимут в обоих направлениях

#### Вкладка "Подземные из-я"
- Пункты подземной полигонометрии
- координаты X и Y
- измеренный азимут каждым гироблоком в обоих направлениях
- внецентренный угол, если не нужен можно поставить 0 00 00.0

#### Вкладка "Расчет" 
- Все полученные дирекционные углы из поправок и измеренного гироскопического азимута
- среднее значение и разница с допуском на каждый гироблок
- средний дирекционный угол из всех измерений и разница с дирекционным из полигонометии.
- точность определения

#### Окно "Лог работы программы" 
- все входные данные с проверкой корректности введенных азимутов
- расчет поправок
- данные и формула для расчета каждого дирекционного угла из поправок

#### кнопка "Вычислить"
- выполняет все вычисления 
- сохраняет подробно каждый этап в окно лога

#### кнопка "Отчет"
- читает файл шаблона "templateFull.docx" из корня папки с программой
- заполняет вычисленными значениями
- сохраняет на "C:\WINDOWS\Temp\OtchetFull.docx"
- открывает сохраненный отчет в WORD, если он установлен (проверял в Office 2013) 

#### TO-DO:
1. Перенести шаблон "templateFull.docx" из папки с программой в папку пользователя "Мои документы"
2. Перенести файл "OtchetFull.docx" с результатом в папку пользователя.
3. Добавить в программу и шаблон дополнительные свойства пункта подземной полигонометрии, "Номер кольца" и "Пикет".
