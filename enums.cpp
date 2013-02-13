#ifndef ENUMS_CPP
#define ENUMS_CPP

// перечисление, определяющее режимы работы главного окна приложения
enum MainMode
{
    mmLogin,    // страница подключения к системе
    mmJournals, // страница со списком доступных журналов
    mmJournal,  // страница с текущим журналом
    mmTools     // страница с инструментами
};

// перечисление, определяющее инструмент в списке инструментов главного окна приложения
enum Tool
{
    tRefresh, // обновить журнал
    tData,    // редактор данных журнала
    tImport,  // импортировать журнал
    tExport,  // экспортировать журнал
    tDelete,  // удалить журнал
    tArchive  // поместить журнал в архив (извлечь журнал их архива)
};

#endif // ENUMS_CPP
