#ifndef LOADERENUMS_CPP
#define LOADERENUMS_CPP

// перечисление, определяющее текущую работу загрузчика
enum LoaderOperation
{
    loIddle,         // простаивает, свободен для операций
    loLogin,         // пробует подключиться к системе
    loJournals,      // пробует загрузить список журналов
    loJournal,       // пробует загрузить журнал
    loSaveJournal,   // пробует сохранить журнал
    loDeleteJournal, // пробует удалить журнал
    loEraseJournal,  // пробует стереть журнал
    loData           // пробует загрузкить данные классов, предметов и учителей
};

#endif // LOADERENUMS_CPP

