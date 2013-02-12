#ifndef LOADERENUMS_CPP
#define LOADERENUMS_CPP

// перечисление, определяющее текущую работу загрузчика
enum LoaderOperation
{
    IDDLE    = 0, // простаивает, свободен для операций
    LOGINING = 1  // пробует залогиниться к сайту
};

#endif // LOADERENUMS_CPP

