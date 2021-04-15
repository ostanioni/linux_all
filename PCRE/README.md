
pcre2demo man page

Return to the PCRE2 index page.

Эта страница является частью документации PCRE2 HTML. Он был создан автоматически из исходной страницы руководства. Если в этом есть какая-то ерунда, обратитесь к странице руководства на случай, если преобразование пошло не так.

```c
/*************************************************
        PCRE2 DEMONSTRATION PROGRAM          
*************************************************/
```
Это демонстрационная программа, демонстрирующая простой способ использования библиотеки регулярных выражений `PCRE2` из программы на языке `C`. См. Краткое обсуждение в документации по `pcre2sample` ("man pcre2sample", если у вас установлены справочные страницы `PCRE2`). `PCRE2` - это обновленный API для библиотеки, несовместимый с исходным `API PCRE`.

На самом деле существует три библиотеки, каждая из которых поддерживает разную ширину кодовой единицы. Эта демонстрационная программа использует 8-битную библиотеку. По умолчанию каждая кодовая единица обрабатывается как отдельный символ, но если шаблон начинается с `(*UTF)`, и он, и тема обрабатываются как строки `UTF-8`, где символы могут занимать несколько кодовых единиц.

В Unix-подобных средах, если `PCRE2` установлен в ваших стандартных системных библиотеках, вы сможете скомпилировать эту программу с помощью этой команды:

`cc -Wall pcre2demo.c -lpcre2-8 -o pcre2demo`

Если PCRE2 не установлен в стандартном месте, скорее всего, он будет установлен с поддержкой механизма `pkg-config`. Если у вас есть pkg-config, вы можете скомпилировать эту программу с помощью этой команды:

`cc -Wall pcre2demo.c 'pkg-config --cflags --libs libpcre2-8' -o pcre2demo`

Если у вас нет `pkg-config`, возможно, вам придется использовать что-то вроде этого:

```c
cc -Wall pcre2demo.c -I/usr/local/include -L/usr/local/lib \
  -R/usr/local/lib -lpcre2-8 -o pcre2demo
```

Замените `/usr/local/include` и `/usr/local/lib` на то место, где в вашей системе установлены файлы `include` и библиотеки для `PCRE2`. Только некоторые операционные системы (например, `Solaris`) используют параметр `-R`.

Сборка под Windows:

Если вы хотите статически связать эту программу с файлом `.a`, отличным от `dll`, вы должны определить `PCRE2_STATIC` перед включением `pcre2.h`, поэтому в этой среде раскомментируйте следующую строку. */

```c
/* #define PCRE2_STATIC */

/* Макрос `PCRE2_CODE_UNIT_WIDTH` должен быть определен до включения `pcre2.h`.
Для программы, которая использует только одну ширину кодовой единицы, установк ее на 8, 16 или 32 позволяет использовать общие имена функций, такие как `pcre2_compile()`. Обратите внимание, что простого изменения 8 на 16 (например) недостаточно для преобразования этой программы для обработки 16-битных символов. Даже в полностью 16-разрядной среде, где функции обработки строк, такие как `strcmp()` и `printf()`, работают с 16-разрядными символами, код для создания таблицы именованных подстрок все равно необходимо будет изменить. */

#define PCRE2_CODE_UNIT_WIDTH 8

#include <stdio.h>
#include <string.h>
#include <pcre2.h>


/********************************************************************************
* Вот  программа. API  включает  понятие  «контекстов»  для установки необычных *
* требований к интерфейсу для компиляции и исправления, таких как настраиваемые *
* менеджеры  памяти  и нестандартные определения новой строки. Эта программа не *
* делает ничего из этого, поэтому она не использует контексты, всегда передавая *
* NULL там, где может быть задан контекст.                                      *
********************************************************************************/

int main(int argc, char **argv)
{
pcre2_code *re;
PCRE2_SPTR pattern;     /* PCRE2_SPTR is a pointer to unsigned code units of */
PCRE2_SPTR subject;     /* the appropriate width (in this case, 8 bits). */
PCRE2_SPTR name_table;

int crlf_is_newline;
int errornumber;
int find_all;
int i;
int rc;
int utf8;

uint32_t option_bits;
uint32_t namecount;
uint32_t name_entry_size;
uint32_t newline;

PCRE2_SIZE erroroffset;
PCRE2_SIZE *ovector;
PCRE2_SIZE subject_length;

pcre2_match_data *match_data;

/****************************************************************************
* Сначала разберитесь в командной строке. На данный момент есть только одна *
* возможная опция, "-g", чтобы запросить повторное сопоставление для поиска *
* всех совпадений, например параметр /g в Perl. Мы устанавливаем переменную *
* find_all в ненулевое значение, если присутствует опция -g.                *
****************************************************************************/

find_all = 0;
for (i = 1; i < argc; i++)
  {
  if (strcmp(argv[i], "-g") == 0) find_all = 1;
  else if (argv[i][0] == '-')
    {
    printf("Unrecognised option %s\n", argv[i]);
    return 1;
    }
  else break;
  }

/* После параметров нам требуется ровно два аргумента: шаблон и строка темы. */

if (argc - i != 2)
  {
  printf("Exactly two arguments required: a regex and a subject string\n");
  return 1;
  }

/* Шаблон и субъект (subject) являются аргументами типа char, поэтому их можно напрямую
преобразовать в PCRE2_SPTR, потому что мы работаем в 8-битных единицах кода.
Длина объекта приводится к PCRE2_SIZE для полноты, хотя PCRE2_SIZE фактически
определено как size_t.*/


pattern = (PCRE2_SPTR)argv[i];
subject = (PCRE2_SPTR)argv[i+1];
subject_length = (PCRE2_SIZE)strlen((char *)subject);


/*************************************************************************
* Now we are going to compile the regular expression pattern, and handle *
* any errors that are detected.                                          *
*************************************************************************/

re = pcre2_compile(
  pattern,               /* the pattern */
  PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
  0,                     /* default options */
  &errornumber,          /* for error number */
  &erroroffset,          /* for error offset */
  NULL);                 /* use default compile context */

/* Compilation failed: print the error message and exit. */

if (re == NULL)
  {
  PCRE2_UCHAR buffer[256];
  pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
  printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,
    buffer);
  return 1;
  }

/*****************************************************************************
* Если компиляция прошла успешно, мы снова вызываем PCRE2, чтобы выполнить   *
* сопоставление с шаблоном строки темы. Это только одно совпадение(match).   *
* Если потребуется дальнейшее сопоставление, это будет сделано ниже.         *
* Перед запуском матча мы должны настроить блок match_data для хранения      *
* результата. Использование pcre2_match_data_create_from_pattern()           *
* гарантирует, что размер блока точно соответствует количеству захватывающих *
* скобок в шаблоне. Если вам нужно знать фактический размер блока match_data *
* в виде количества байтов, вы можете найти его так:                         *
* PCRE2_SIZE match_data_size = pcre2_get_match_data_size(match_data);        *
*****************************************************************************/

match_data = pcre2_match_data_create_from_pattern(re, NULL);

/* Now run the match. */

rc = pcre2_match(
  re,                   /* the compiled pattern */
  subject,              /* the subject string */
  subject_length,       /* the length of the subject */
  0,                    /* start at offset 0 in the subject */
  0,                    /* default options */
  match_data,           /* block for storing the result */
  NULL);                /* use default match context */

/* Matching failed: handle error cases */

if (rc < 0)
  {
  switch(rc)
    {
    case PCRE2_ERROR_NOMATCH: printf("No match\n"); break;
    /*
    Handle other special cases if you like
    */
    default: printf("Matching error %d\n", rc); break;
    }
  pcre2_match_data_free(match_data);   /* Release memory used for the match */
  pcre2_code_free(re);                 /*   data and the compiled pattern. */
  return 1;
  }

/* Match succeded. Get a pointer to the output vector, where string offsets are
stored. */

ovector = pcre2_get_ovector_pointer(match_data);
printf("Match succeeded at offset %d\n", (int)ovector[0]);


/*****************************************************************************
* Мы нашли первое совпадение в строке. Если выходной вектор был недостаточно *
* большим, скажите об этом. Затем выведите все захваченные одстроки.         *
*****************************************************************************/

/* Выходной вектор был недостаточно большим. Этого не должно происходить,
 потому что мы использовали pcre2_match_data_create_from_pattern() выше. */

if (rc == 0)
  printf("ovector was not big enough for all the captured substrings\n");

/* Мы должны защититься от шаблонов, таких как /(?=.\K)/, которые используют \K в утверждении, чтобы установить начало совпадения позже, чем его конец. В этой демонстрационной программе мы просто обнаруживаем этот случай и сдаемся.  */

if (ovector[0] > ovector[1])
  {
  printf("\\K использовался в утверждении, чтобы установить начало совпадения после его окончания.\n"
    "From end to start the match was: %.*s\n", (int)(ovector[0] - ovector[1]),
      (char *)(subject + ovector[1]));
  printf("Run abandoned\n");
  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
  return 1;
  }

/* Показать подстроки, хранящиеся в выходном векторе, по номеру. Очевидно,
что в реальном приложении вы можете захотеть делать что-то, кроме печати. */

for (i = 0; i < rc; i++)
  {
  PCRE2_SPTR substring_start = subject + ovector[2*i];
  PCRE2_SIZE substring_length = ovector[2*i+1] - ovector[2*i];
  printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
  }


/**************************************************************************
* На этом основная часть демонстрационной программы завершена. Мы составили
шаблон и выполнили одно совпадение. В приведенном ниже коде сначала показано,
как получить доступ к именованным подстрокам, а затем как кодировать
повторяющиеся совпадения по той же теме. **************************************************************************/

/* Посмотрите, есть ли какие-либо именованные подстроки, и если да, покажите их по имени. Сначала нам нужно извлечь количество именованных скобок из шаблона. */

(void)pcre2_pattern_info(
  re,                   /* the compiled pattern */
  PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
  &namecount);          /* where to put the answer */

if (namecount == 0) printf("No named substrings\n"); else
  {
  PCRE2_SPTR tabptr;
  printf("Named substrings\n");

  /* Прежде чем мы сможем получить доступ к подстрокам, мы должны извлечь
  таблицу для преобразования имен в числа и размер каждой записи в таблице. */

  (void)pcre2_pattern_info(
    re,                       /* the compiled pattern */
    PCRE2_INFO_NAMETABLE,     /* address of the table */
    &name_table);             /* where to put the answer */

  (void)pcre2_pattern_info(
    re,                       /* the compiled pattern */
    PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
    &name_entry_size);        /* where to put the answer */

  /* Теперь мы можем сканировать таблицу и для каждой записи вывести номер, имя и саму подстроку. В 8-битной библиотеке номер хранится в двух байтах, начиная со старшего. */

  tabptr = name_table;
  for (i = 0; i < namecount; i++)
    {
    int n = (tabptr[0] << 8) | tabptr[1];
    printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
      (int)(ovector[2*n+1] - ovector[2*n]), subject + ovector[2*n]);
    tabptr += name_entry_size;
    }
  }


/*************************************************************************
* If the "-g" option was given on the command line, we want to continue  *
* to search for additional matches in the subject string, in a similar   *
* way to the /g option in Perl. This turns out to be trickier than you   *
* might think because of the possibility of matching an empty string.    *
* What happens is as follows:                                            *
*                                                                        *
* If the previous match was NOT for an empty string, we can just start   *
* the next match at the end of the previous one.                         *
*                                                                        *
* If the previous match WAS for an empty string, we can't do that, as it *
* would lead to an infinite loop. Instead, a call of pcre2_match() is    *
* made with the PCRE2_NOTEMPTY_ATSTART and PCRE2_ANCHORED flags set. The *
* first of these tells PCRE2 that an empty string at the start of the    *
* subject is not a valid match; other possibilities must be tried. The   *
* second flag restricts PCRE2 to one match attempt at the initial string *
* position. If this match succeeds, an alternative to the empty string   *
* match has been found, and we can print it and proceed round the loop,  *
* advancing by the length of whatever was found. If this match does not  *
* succeed, we still stay in the loop, advancing by just one character.   *
* In UTF-8 mode, which can be set by (*UTF) in the pattern, this may be  *
* more than one byte.                                                    *
*                                                                        *
* However, there is a complication concerned with newlines. When the     *
* newline convention is such that CRLF is a valid newline, we must       *
* advance by two characters rather than one. The newline convention can  *
* be set in the regex by (*CR), etc.; if not, we must find the default.  *
*************************************************************************/

if (!find_all)     /* Check for -g */
  {
  pcre2_match_data_free(match_data);  /* Release the memory that was used */
  pcre2_code_free(re);                /* for the match data and the pattern. */
  return 0;                           /* Exit the program. */
  }

/* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
sequence. First, find the options with which the regex was compiled and extract
the UTF state. */

(void)pcre2_pattern_info(re, PCRE2_INFO_ALLOPTIONS, &option_bits);
utf8 = (option_bits & PCRE2_UTF) != 0;

/* Now find the newline convention and see whether CRLF is a valid newline
sequence. */

(void)pcre2_pattern_info(re, PCRE2_INFO_NEWLINE, &newline);
crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
                  newline == PCRE2_NEWLINE_CRLF ||
                  newline == PCRE2_NEWLINE_ANYCRLF;

/* Loop for second and subsequent matches */

for (;;)
  {
  uint32_t options = 0;                   /* Normally no options */
  PCRE2_SIZE start_offset = ovector[1];   /* Start at end of previous match */

  /* If the previous match was for an empty string, we are finished if we are
  at the end of the subject. Otherwise, arrange to run another match at the
  same point to see if a non-empty match can be found. */

  if (ovector[0] == ovector[1])
    {
    if (ovector[0] == subject_length) break;
    options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
    }

  /* If the previous match was not an empty string, there is one tricky case to
  consider. If a pattern contains \K within a lookbehind assertion at the
  start, the end of the matched string can be at the offset where the match
  started. Without special action, this leads to a loop that keeps on matching
  the same substring. We must detect this case and arrange to move the start on
  by one character. The pcre2_get_startchar() function returns the starting
  offset that was passed to pcre2_match(). */

  else
    {
    PCRE2_SIZE startchar = pcre2_get_startchar(match_data);
    if (start_offset <= startchar)
      {
      if (startchar >= subject_length) break;   /* Reached end of subject.   */
      start_offset = startchar + 1;             /* Advance by one character. */
      if (utf8)                                 /* If UTF-8, it may be more  */
        {                                       /*   than one code unit.     */
        for (; start_offset < subject_length; start_offset++)
          if ((subject[start_offset] & 0xc0) != 0x80) break;
        }
      }
    }

  /* Run the next matching operation */

  rc = pcre2_match(
    re,                   /* the compiled pattern */
    subject,              /* the subject string */
    subject_length,       /* the length of the subject */
    start_offset,         /* starting offset in the subject */
    options,              /* options */
    match_data,           /* block for storing the result */
    NULL);                /* use default match context */

  /* This time, a result of NOMATCH isn't an error. If the value in "options"
  is zero, it just means we have found all possible matches, so the loop ends.
  Otherwise, it means we have failed to find a non-empty-string match at a
  point where there was a previous empty-string match. In this case, we do what
  Perl does: advance the matching position by one character, and continue. We
  do this by setting the "end of previous match" offset, because that is picked
  up at the top of the loop as the point at which to start again.

  There are two complications: (a) When CRLF is a valid newline sequence, and
  the current position is just before it, advance by an extra byte. (b)
  Otherwise we must ensure that we skip an entire UTF character if we are in
  UTF mode. */

  if (rc == PCRE2_ERROR_NOMATCH)
    {
    if (options == 0) break;                    /* All matches found */
    ovector[1] = start_offset + 1;              /* Advance one code unit */
    if (crlf_is_newline &&                      /* If CRLF is a newline & */
        start_offset < subject_length - 1 &&    /* we are at CRLF, */
        subject[start_offset] == '\r' &&
        subject[start_offset + 1] == '\n')
      ovector[1] += 1;                          /* Advance by one more. */
    else if (utf8)                              /* Otherwise, ensure we */
      {                                         /* advance a whole UTF-8 */
      while (ovector[1] < subject_length)       /* character. */
        {
        if ((subject[ovector[1]] & 0xc0) != 0x80) break;
        ovector[1] += 1;
        }
      }
    continue;    /* Go round the loop again */
    }

  /* Other matching errors are not recoverable. */

  if (rc < 0)
    {
    printf("Matching error %d\n", rc);
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    return 1;
    }

  /* Match succeded */

  printf("\nMatch succeeded again at offset %d\n", (int)ovector[0]);

  /* The match succeeded, but the output vector wasn't big enough. This
  should not happen. */

  if (rc == 0)
    printf("ovector was not big enough for all the captured substrings\n");

  /* We must guard against patterns such as /(?=.\K)/ that use \K in an
  assertion to set the start of a match later than its end. In this
  demonstration program, we just detect this case and give up. */

  if (ovector[0] > ovector[1])
    {
    printf("\\K was used in an assertion to set the match start after its end.\n"
      "From end to start the match was: %.*s\n", (int)(ovector[0] - ovector[1]),
        (char *)(subject + ovector[1]));
    printf("Run abandoned\n");
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    return 1;
    }

  /* As before, show substrings stored in the output vector by number, and then
  also any named substrings. */

  for (i = 0; i < rc; i++)
    {
    PCRE2_SPTR substring_start = subject + ovector[2*i];
    size_t substring_length = ovector[2*i+1] - ovector[2*i];
    printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
    }

  if (namecount == 0) printf("No named substrings\n"); else
    {
    PCRE2_SPTR tabptr = name_table;
    printf("Named substrings\n");
    for (i = 0; i < namecount; i++)
      {
      int n = (tabptr[0] << 8) | tabptr[1];
      printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
        (int)(ovector[2*n+1] - ovector[2*n]), subject + ovector[2*n]);
      tabptr += name_entry_size;
      }
    }
  }      /* End of loop to find second and subsequent matches */

printf("\n");
pcre2_match_data_free(match_data);
pcre2_code_free(re);
return 0;
}

/* End of pcre2demo.c */


Return to the PCRE2 index page.

