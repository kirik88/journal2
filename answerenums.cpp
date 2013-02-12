#ifndef ANSWERENUMS_CPP
#define ANSWERENUMS_CPP

// перечисление, определяющее код ответа сервера
enum AnswerCode
{
    OK                 =  0, // все в порядке
    ACCESS_FAIL        =  1, // нет прав доступа к журналу
    READ_ONLY          =  2, // нет прав на редактирование
    SUBMIT_FAIL        =  3, // неверно сформирован запрос
    DB_OFFLINE         =  9, // нет доступа к базе данных
    LOGIN_FAIL         = 41, // неверные данные при авторизации
    NOT_AUTORIZED      = 42, // пользователь не авторизован
    HOST_NOT_FOUND1     = 96, // сайт недоступен
    CONNECTION_REFUSED = 97, // нет подключения к интернету
    ANSWER_ERROR       = 98, // ошибка при парсинге ответа
    OTHER_ERROR        = 99  // другая ошибка
};

#endif // ANSWERENUMS_CPP
