/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Jan Ossowski <marpe@mimuw.edu.pl>
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>

/** @brief To jest struktura przechowująca przekierowania numerów telefonów.
 *
 */
struct PhoneForward;
typedef struct PhoneForward PhoneForward;

/** @brief To jest struktura przechowująca ciąg numerów telefonów.
 *
 */
struct PhoneNumbers;
typedef struct PhoneNumbers PhoneNumbers;

/** @brief To jest struktura przechowująca inwersje przekierowań numerów telefonów w korzeniu struktury PhoneForward.
 *
 */
struct PhoneBackward;
typedef struct PhoneBackward PhoneBackward;

struct Inversion;
typedef struct Inversion Inversion;

/** @brief Sprawdza, czy znak jest prawidłową cyfrą numeru.
 * @param c - sprawdzany znak.
 * @return Wartość @p true jeżeli c jest prawidłową cyfrą numeru lub
 * @p false, jeżeli nie jest prawidłową cyfrą numeru.
 */
static bool numDigitIsCorrect(char c);

/** @brief Sprawdza, czy @p num1 jest prefiksem numeru @p num2.
 * @param num1 sprawdzany numer.
 * @param num2 numer, którego prefiks jest sprawdzany.
 * @return Wartość @p true jeżeli @p num1 jest prefiksem @p num2 lub
 * @p false w przeciwnym wypadku.
 */
static bool numIsPrefix(const char *num1, const char *num2);

/** @brief Zwraca indeks ze struktury @p PhoneForward odpowiadający danej cyfrze.
 * @param c - cyfra.
 * @return Indeks odpowiadający cyfrze @p c (0-9 dla znaków '0'-'9', 10 dla '*', 11 dla '#').
 */
static int numDigitToIndex(char c);

/** @brief Odpowiednik funkcji strlen dla numerów telefonów.
 * Zwraca długość numeru telefonu pod adresem @p num. Numery mogą być kończone dowolnym znakiem niebędącym cyfrą.
 * Zachowanie niezdefiniowane dla numerów, które nie są kończone znakiem niebędącym cyfrą.
 * @param num - wskaźnik na numer.
 * @return Długość numeru.
 */
static size_t numlen(const char *num);

/** @brief Odpowiednik funkcji strcpy dla numerów telefonów.
 * Zwraca wskaźnik podany w argumencie @p num1, którego wartość jest skopiowanym prawidłowym numerem telefonu będącym
 * wartością podanego argumentu @p num2. @p num1 musi mieć zaalokowaną wystarczającą ilość pamięci, by pomieścić kopię.
 * Zachowanie niezdefiniowane dla numerów, które nie są kończone znakiem niebędącym cyfrą.
 * @param num1 - wskaźnik na cel.
 * @param num2 - wskaźnik na źródło.
 * @return Wskaźnik na numer telefonu skopiowanego z @p num2.
 */
static char *numcpy(char *num1, const char *num2);

/** @brief Odpowiednik funkcji strcat dla numerów telefonów.
 * Zwraca wskaźnik podany w argumencie @p num1, którego wartość jest prawidłowym numerem telefonu będącym konkatenacją
 * numerów w @p num1 oraz @p num2. @p num1 musi mieć zaalokowaną wystarczającą ilość pamięci, by pomieścić konkatenację.
 * Zachowanie niezdefiniowane dla numerów, które nie są kończone znakiem niebędącym cyfrą.
 * @param num1 - wskaźnik na pierwszy numer i cel dla konkatenacji.
 * @param num2 - wskaźnik na drugi numer konkatencaji.
 * @return Wskaźnik na konkatenację numerów @p num1 i @p num2.
 */
static char *numcat(char *num1, const char *num2);

/** @brief Odpowiednik funkcji strcmp dla numerów telefonów.
 * Porównuje leksykograficznie numery telefonów wskazywane przez @p num1 i @p num2.
 * Zachowanie niezdefiniowane dla numerów, które nie są kończone znakiem niebędącym cyfrą.
 * @param num1 - wskaźnik na pierwszy numer telefonu.
 * @param num2 - wskaźnik na drugi numer telefonu.
 * @return Zmienna typu int o wartości zgodnej z działaniem komparatorów, odpowiednio ujemnej, dodatniej lub zerowej w
 * zależności od uporządkowania numerów wskazywanych przez @p num1 i @p num2.
 */
static int numcmp(const char *num1, const char *num2);

/** @brief Wrapper funkcji nucmp.
 * Umożliwia wykorzystanie numcp jako komparatora w funkcji qsort.
 * @param num1 - wskaźnik na pierwszy numer telefonu.
 * @param num2 - wskaźnik na drugi numer telefonu.
 * @return Zmienna typu int o wartości zgodnej z działaniem komparatorów, odpowiednio ujemnej, dodatniej lub zerowej w
 * zależności od uporządkowania numerów wskazywanych przez @p num1 i @p num2.
 */
static int numcmpwrap(const void *num1, const void *num2);

/** @brief Komparator inwersji przekierowań numerów telefonów, porządkując po źródłach.
 * Porównuje leksykograficznie źródła inwersji przekierowań numerów telefonów wskazywanych przez @p inv1 i @p inv2.
 * Zachowanie niezdefiniowane dla numerów, które nie są kończone znakiem niebędącym cyfrą.
 * @param inv1 - wskaźnik na pierwszą inwersje przekierowania numeru telefonu.
 * @param inv2 - wskaźnik na drugą inwersję przekierowania numeru telefonu.
 * @return Zmienna typu int o wartości zgodnej z działaniem komparatorów, odpowiednio ujemnej, dodatniej lub zerowej w
 * zależności od uporządkowania inwersji numerów wskazywanych przez @p inv1 i @p inv2.
 */
static int invrscmporg(const void *inv1, const void *inv2);

/** @brief Sprawdza prawidłowość numeru.
 * Zachowanie niezdefiniowane dla numerów, które nie są kończone znakiem niebędącym cyfrą.
 * @param num - wskaźnik na sprawdzany numer.
 * @return Wartość @p true jeżeli num wskazuje na prawidłowy numer lub
 * @p false, jeżeli nie wskazuje na prawidłowy numer.
 */
static bool numIsCorrect(const char *num);

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward *phfwdNew(void);

/** @brief Tworzy nową strukturę inwersji.
 * Tworzy nową strukturę niezawierającą żadnych inwersji.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
Inversion *invrsNew(const char *num_forward, const char *num_origin);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p inv. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] inv – wskaźnik na usuwaną strukturę.
 */
void invrsDelete(Inversion *inv);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num1   – wskaźnik na napis reprezentujący prefiks numerów
 *                     przekierowywanych;
 * @param[in] num2   – wskaźnik na napis reprezentujący prefiks numerów,
 *                     na które jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2);

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
 * lub napis nie reprezentuje numeru, nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest ciąg zawierający co najwyżej jeden numer. Jeśli dany
 * numer nie został przekierowany, to wynikiem jest ciąg zawierający ten numer.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg.
 * Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza następujący ciąg numerów: jeśli istnieje numer @p x, taki że wynik
 * wywołania @p phfwdGet z numerem @p x zawiera numer @p num, to numer @p x
 * należy do wyniku wywołania @ref phfwdReverse z numerem @p num. Dodatkowo ciąg
 * wynikowy zawsze zawiera też numer @p num. Wynikowe numery są posortowane
 * leksykograficznie i nie mogą się powtarzać. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Wyznacza przeciwobraz funkcji @p phfwdGet dla danego numeru.
 * Wyznacza posortowaną leksykograficznie listę wszystkich takich numerów telefonów i tylko
 * takich numerów telefonów @p x, że @p phfwdGet(x) = num.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers *pnum);

/** @brief Dodaje numer telefonu.
 * Dodaje numer telefonu wskazywany przez @p num do struktury numerów telefonów wskazywanej przez @p pnum.
 * @param[in] pnum – wskaźnik na strukturę.
 * @param[in] num – wskaźnik na dodawany numer.
 */
static bool phnumAdd(PhoneNumbers *pnum, const char *num);

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  – indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const *phnumGet(PhoneNumbers const *pnum, size_t idx);

#endif /* __PHONE_FORWARD_H__ */
