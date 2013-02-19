#ifndef USERENUMS_CPP
#define USERENUMS_CPP

// перечисление, определяющее уровень доступа пользователя
enum UserType
{
    utAdmin      = 1, // Администратор
    utDirector   = 2, // Завуч
    utTeacher    = 3, // Учитель
    utParent     = 4, // Родитель
    utStudent    = 5, // Ученик
    utController = 6  // Проверяющий
};

#endif // USERENUMS_CPP
