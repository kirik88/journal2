#ifndef LOADERENUMS_CPP
#define LOADERENUMS_CPP

// перечисление, определяющее текущую работу загрузчика
enum LoaderOperation
{
    loIDDLE    = 0, // простаивает, свободен для операций
    loLOGIN    = 1, // пробует подключиться к системе
    loJOURNALS = 2, // пробует загрузить список журналов
    loJOURNAL  = 3  // пробует загрузить журнал
};

#endif // LOADERENUMS_CPP

