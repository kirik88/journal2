#ifndef LOADERENUMS_CPP
#define LOADERENUMS_CPP

// перечисление, определяющее текущую работу загрузчика
enum LoaderOperation
{
    loIddle    = 0, // простаивает, свободен для операций
    loLogin    = 1, // пробует подключиться к системе
    loJournals = 2, // пробует загрузить список журналов
    loJournal  = 3  // пробует загрузить журнал
};

#endif // LOADERENUMS_CPP

