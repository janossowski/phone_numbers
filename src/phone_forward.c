#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "phone_forward.h"

#define PHONE_NUMBER_DIGITS 12

/** @brief Struktura przechowująca przekierowania numerów telefonów działająca na zasadzie drzewa trie.
 */
struct PhoneForward {
    //! Wskazuje na tablicę zawierającą PHONE_NUMBER_DIGITS wskaźników reprezentujących kolejną cyfrę prefiksu w drzewie
    //! trie.
    PhoneForward **next;
    //! Wskazuje na numer będący prefiksem na który powinny być przekierowywane numery o prefiksie reprezentowanym przez
    //! aktualną pozycję w drzewie trie. Jeżeli NULL, to prawidłowy prefiks to najbliższe nie-NULLowe redirection na
    //! drodze do korzenia.
    char *redirection;
    //! Ilość inwersji w drzewie.
    size_t inversion_amount;
    //! Aktualna pojemność inwersji w drzewie.
    size_t inversion_capacity;
    //! Przechowuje inwersje wszystkich przekierowań dodanych do tego drzewa.
    Inversion **inversions;
};

/** @brief Struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers {
    //! Liczba numerów zawartych w strukturze.
    size_t number_amount;
    //! Liczba numerów, które może pomieścić struktura.
    size_t number_capacity;
    //! Wskaźnik na numery zawarte w strukturze.
    char **numbers;
};

/** @brief Struktura przechowująca inwersję przekierowania numeru telefonu.
 */
struct Inversion {
    char *forward;
    char *origin;
};

static bool numDigitIsCorrect(char c) {
    if ((c >= '0' && c <= '9') || c == '*' || c == '#') return true;
    return false;
}

static bool numIsPrefix(const char *num1, const char *num2) {
    size_t num1_len = numlen(num1);
    if (num1_len > numlen(num2)) return false;
    for (size_t i = 0; i < num1_len; i++) {
        if (num1[i] != num2[i]) return false;
    }
    return true;
}

static int numDigitToIndex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c == '*') {
        return 10;
    } else if (c == '#') {
        return 11;
    } else {
        return -1; //digit not correct
    }
}

static size_t numlen(const char *num) {
    size_t it = 0;
    while (numDigitIsCorrect(num[it])) {
        it++;
    }
    return it;
}

static char *numcpy(char *num1, const char *num2) {
    size_t len = numlen(num2);
    for (size_t i = 0; i < len; i++) {
        num1[i] = num2[i];
    }

    num1[len] = '\0';

    return num1;
}

static char *numcat(char *num1, const char *num2) {
    size_t len1 = numlen(num1);
    size_t len2 = numlen(num2);

    for (size_t i = 0; i <= len2; i++) {
        num1[i + len1] = num2[i];
    }

    return num1;
}

static int numcmp(const char *num1, const char *num2) {
    size_t len1 = numlen(num1);
    size_t len2 = numlen(num2);
    size_t len = len1;
    if (len2 < len) len = len2;

    for (size_t i = 0; i < len; i++) {
        if (numDigitToIndex(num1[i]) > numDigitToIndex(num2[i])) return 1;
        if (numDigitToIndex(num1[i]) < numDigitToIndex(num2[i])) return -1;
    }

    if (len1 < len2) return -1;
    if (len1 > len2) return 1;
    return 0;
}

static int numcmpwrap(const void *num1, const void *num2) {
    const char *arg1 = *(char **) num1;
    const char *arg2 = *(char **) num2;
    return numcmp(arg1, arg2);
}

static int invrscmporg(const void *inv1, const void *inv2) {
    Inversion *arg1 = *(Inversion **) inv1;
    Inversion *arg2 = *(Inversion **) inv2;

    return numcmp(arg1->origin, arg2->origin);
}

static bool numIsCorrect(const char *num) {
    if (num == NULL) return false;
    if (numlen(num) == 0) return false;
    if (num[numlen(num)] != '\0') return false;
    return true;
}

PhoneForward *phfwdNew(void) {
    PhoneForward *newphfwd = malloc(sizeof(PhoneForward));
    if (newphfwd == NULL) return NULL;

    newphfwd->next = malloc(PHONE_NUMBER_DIGITS * sizeof(PhoneForward *));
    if (newphfwd->next == NULL) {
        free(newphfwd);
        return NULL;
    }

    newphfwd->inversion_amount = 0;
    newphfwd->inversion_capacity = 1;
    newphfwd->inversions = malloc(sizeof(Inversion *));
    if (newphfwd->inversions == NULL) {
        free(newphfwd->next);
        free(newphfwd);
        return NULL;
    }

    for (int i = 0; i < PHONE_NUMBER_DIGITS; i++) {
        newphfwd->next[i] = NULL;
    }
    newphfwd->redirection = NULL;
    return newphfwd;
}

Inversion *invrsNew(const char *num_forward, const char *num_origin) {
    if (!numIsCorrect(num_forward) || !numIsCorrect(num_origin)) return NULL;
    Inversion *newinvrs = malloc(sizeof(Inversion));
    if (newinvrs == NULL) return NULL;

    newinvrs->forward = malloc(numlen(num_forward) + 1);
    if (newinvrs->forward == NULL) {
        free(newinvrs);
        return NULL;
    }

    newinvrs->origin = malloc(numlen(num_origin) + 1);
    if (newinvrs->origin == NULL) {
        free(newinvrs->forward);
        free(newinvrs);
        return NULL;
    }

    numcpy(newinvrs->forward, num_forward);
    numcpy(newinvrs->origin, num_origin);
    return newinvrs;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL) return NULL;

    PhoneNumbers *pnum = malloc(sizeof(PhoneNumbers));
    if (pnum == NULL) return NULL;

    pnum->number_amount = 0;
    pnum->number_capacity = 1;
    pnum->numbers = malloc(sizeof(char *));
    if (pnum->numbers == NULL) {
        free(pnum);
        return NULL;
    }

    if (!numIsCorrect(num)) return pnum;

    if (!phnumAdd(pnum, num)) {
        phnumDelete(pnum);
        return NULL;
    }

    char *c = malloc(numlen(num) + 1);
    if (c == NULL) {
        phnumDelete(pnum);
        return NULL;
    }
    for (size_t i = 0; i < pf->inversion_amount; i++) {
        if (numIsPrefix(pf->inversions[i]->forward, num)) {
            c = realloc(c, numlen(pf->inversions[i]->origin) + numlen(num) - numlen(pf->inversions[i]->forward) + 1);
            if (c == NULL) {
                phnumDelete(pnum);
                return NULL;
            }
            numcpy(c, pf->inversions[i]->origin);
            numcat(c, num + numlen(pf->inversions[i]->forward));
            if (!phnumAdd(pnum, c)) {
                phnumDelete(pnum);
                free(c);
                return NULL;
            }
        }
    }
    free(c);

    qsort(pnum->numbers, pnum->number_amount, sizeof(char *), numcmpwrap);

    PhoneNumbers *res = malloc(sizeof(PhoneNumbers));
    if (res == NULL) {
        phnumDelete(pnum);
        return NULL;
    }

    res->number_amount = 0;
    res->number_capacity = 1;
    res->numbers = malloc(sizeof(char *));
    if (res->numbers == NULL) {
        phnumDelete(pnum);
        free(res);
        return NULL;
    }

    if (!phnumAdd(res, pnum->numbers[0])) {
        phnumDelete(pnum);
        phnumDelete(res);
        return NULL;
    }

    for (size_t i = 1; i < pnum->number_amount; i++) {
        if (numcmp(pnum->numbers[i], pnum->numbers[i - 1]) != 0) {
            if (!phnumAdd(res, pnum->numbers[i])) {
                phnumDelete(pnum);
                phnumDelete(res);
                return NULL;
            }
        }
    }

    phnumDelete(pnum);

    return res;
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL) return NULL;
    PhoneNumbers *res = malloc(sizeof(PhoneNumbers));
    if (res == NULL) return NULL;
    res->number_amount = 0;
    res->number_capacity = 1;
    res->numbers = malloc(sizeof(char *));
    if (res->numbers == NULL) {
        free(res);
        return NULL;
    }
    if (!numIsCorrect(num)) return res;

    PhoneNumbers *pnum = phfwdReverse(pf, num);
    if (pnum == NULL) {
        free(res->numbers);
        free(res);
        return NULL;
    }

    PhoneNumbers *get;
    for (size_t i = 0; i < pnum->number_amount; i++) {
        get = phfwdGet(pf, pnum->numbers[i]);
        if (get == NULL) {
            phnumDelete(pnum);
            phnumDelete(res);
            return NULL;
        }
        const char *get_res = phnumGet(get, 0);
        if (get_res == NULL) {
            phnumDelete(pnum);
            phnumDelete(get);
            phnumDelete(res);
            return NULL;
        }
        if (numcmp(phnumGet(get, 0), num) == 0) {
            if (!phnumAdd(res, pnum->numbers[i])) {
                phnumDelete(pnum);
                phnumDelete(get);
                phnumDelete(res);
                return NULL;
            }
        }
        phnumDelete(get);
    }
    phnumDelete(pnum);
    return res;
}

void phfwdDelete(PhoneForward *pf) {
    if (pf == NULL) {
        return;
    }
    for (int i = 0; i < PHONE_NUMBER_DIGITS; i++) {
        phfwdDelete(pf->next[i]);
    }
    free(pf->next);
    free(pf->redirection);

    for (size_t i = 0; i < pf->inversion_amount; i++) {
        invrsDelete(pf->inversions[i]);
    }
    free(pf->inversions);

    free(pf);
}

void invrsDelete(Inversion *inv) {
    if (inv == NULL) {
        return;
    }
    free(inv->origin);
    free(inv->forward);
    free(inv);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (pf == NULL || !numIsCorrect(num1) || !numIsCorrect(num2) || numcmp(num1, num2) == 0) return false;

    PhoneForward *pf_origin = pf;
    size_t num1_len = numlen(num1);
    for (size_t num1_it = 0; num1_it < num1_len; num1_it++) {
        int index = numDigitToIndex(num1[num1_it]);
        if (pf->next[index] == NULL) pf->next[index] = phfwdNew();
        if (pf->next[index] == NULL) return false;
        pf = pf->next[index];
    }
    if (pf->redirection == NULL) {
        pf->redirection = malloc((numlen(num2) + 1));
        if (pf->redirection == NULL) return false;
    } else {
        char *new_redirection = realloc(pf->redirection, numlen(num2) + 1);
        if (new_redirection == NULL) return false;
        pf->redirection = new_redirection;
    }

    numcpy(pf->redirection, num2);

    if (pf_origin->inversion_amount >= pf_origin->inversion_capacity) {
        pf_origin->inversion_capacity *= 2;
        Inversion **new_inversions = realloc(pf_origin->inversions,
                                             pf_origin->inversion_capacity * sizeof(Inversion *));

        if (new_inversions == NULL) {
            pf_origin->inversion_capacity /= 2;
            free(pf->redirection);
            pf->redirection = NULL;
            return false;
        }
        pf_origin->inversions = new_inversions;
    }

    pf_origin->inversions[pf_origin->inversion_amount] = invrsNew(num2, num1);
    if (pf_origin->inversions[pf_origin->inversion_amount] == NULL) {
        free(pf->redirection);
        pf->redirection = NULL;
        return false;
    }
    qsort(pf->inversions, pf->inversion_amount, sizeof(Inversion *), invrscmporg);

    pf_origin->inversion_amount++;
    return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (!numIsCorrect(num) || pf == NULL) return;

    size_t l = 0;
    size_t r = pf->inversion_amount;
    size_t m;

    while (l < r) {
        m = (l + r) / 2;
        if (numcmp(num, pf->inversions[m]->origin) > 0) {
            l = m + 1;
        } else {
            r = m;
        }
    }

    size_t first_index = l;

    while (l < pf->inversion_amount && numIsPrefix(num, pf->inversions[l]->origin)) {
        invrsDelete(pf->inversions[l]);
        l++;
    }

    size_t removed_inversions = l - first_index;

    for (size_t i = 0; l + i < pf->inversion_amount; i++) {
        pf->inversions[first_index + i] = pf->inversions[l + i];
    }

    pf->inversion_amount -= removed_inversions;

    size_t num_len = numlen(num);
    for (size_t num_it = 0; num_it < num_len - 1; num_it++) {
        int index = numDigitToIndex(num[num_it]);
        if (pf->next[index] == NULL) return;
        pf = pf->next[index];
    }
    int index = numDigitToIndex(num[num_len - 1]);
    phfwdDelete(pf->next[index]);
    pf->next[index] = NULL;
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (pf == NULL) return NULL;
    if (!numIsCorrect(num)) {
        PhoneNumbers *res = malloc(sizeof(PhoneNumbers));
        if (res == NULL) return NULL;

        res->number_amount = 0;
        res->number_capacity = 1;
        res->numbers = malloc(sizeof(char *));
        if (res->numbers == NULL) {
            free(res);
            return NULL;
        }
        return res;
    }

    size_t num_len = numlen(num);
    size_t deepest_found = 0;
    PhoneForward pf_it = *pf;

    for (size_t num_it = 0; num_it <= num_len; num_it++) {
        if (pf_it.redirection != NULL) {
            deepest_found = num_it;
        }
        int index = numDigitToIndex(num[num_it]);
        if (num_it == num_len || pf_it.next[index] == NULL) break;
        pf_it = *pf_it.next[index];
    }
    PhoneNumbers *res = malloc(sizeof(PhoneNumbers));
    if (res == NULL) return NULL;

    res->number_amount = 1;
    res->numbers = malloc(sizeof(char *));
    if (res->numbers == NULL) {
        free(res);
        return NULL;
    }

    if (deepest_found == 0) {
        res->numbers[0] = malloc(numlen(num) + 1);
        if (res->numbers[0] == NULL) {
            free(res->numbers);
            free(res);
            return NULL;
        }

        numcpy(res->numbers[0], num);
    } else {
        pf_it = *pf;

        for (size_t num_it = 0; num_it < deepest_found; num_it++) {
            pf_it = *pf_it.next[numDigitToIndex(num[num_it])];
        }
        res->numbers[0] = malloc(numlen(pf_it.redirection) + numlen(num + deepest_found) + 1);
        if (res->numbers[0] == NULL) {
            free(res->numbers);
            free(res);
            return NULL;
        }

        numcpy(res->numbers[0], pf_it.redirection);
        numcat(res->numbers[0], num + deepest_found);
    }
    return res;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum == NULL) return;

    for (size_t i = 0; i < pnum->number_amount; i++) {
        free(pnum->numbers[i]);
    }
    free(pnum->numbers);
    free(pnum);
}

static inline bool phnumAdd(PhoneNumbers *pnum, const char *num) {
    if (pnum == NULL || !numIsCorrect(num)) {
        return false;
    }
    if (pnum->number_amount >= pnum->number_capacity) {
        pnum->number_capacity *= 2;
        char **new_numbers = realloc(pnum->numbers, pnum->number_capacity * sizeof(char *));

        if (new_numbers == NULL) {
            pnum->number_capacity /= 2;
            return false;
        }
        pnum->numbers = new_numbers;
    }
    pnum->numbers[pnum->number_amount] = malloc(numlen(num) + 1);
    if (pnum->numbers[pnum->number_amount] == NULL) {
        return false;
    }
    numcpy(pnum->numbers[pnum->number_amount], num);
    pnum->number_amount++;
    return true;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (pnum == NULL) return NULL;
    char *res = NULL;
    if (idx >= pnum->number_amount) return res;
    res = pnum->numbers[idx];
    return res;
}