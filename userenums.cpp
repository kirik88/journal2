#ifndef USERENUMS_CPP
#define USERENUMS_CPP

// перечисление, определяющее уровень доступа пользователя
enum UserType
{
    ADMIN              = 1, // Администратор
    DIRECTOR           = 2, // Завуч
    TEACHER            = 3, // Учитель
    PARENT             = 4, // Родитель
    STUDENT            = 5, // Ученик
    CONTROLLER         = 6  // Проверяющий
};

#endif // USERENUMS_CPP
