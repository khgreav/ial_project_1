
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/
    while (L->First !=  NULL) { //loop until list is empty
        tDLElemPtr tmp = L->First->rptr; //store pointer to next
        free(L->First); //free First
        L->First = tmp; //set as new First
    }
    //reinitialize
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr new = malloc(sizeof(struct tDLElem));
    if (new == NULL) { //memory allocation failed, call DLError and end
        DLError();
        return;
    }
    new->data = val;
    new->rptr = L->First;
    new->lptr = NULL;
    if (L->First != NULL) { //list not empty, assign first element as second
        L->First->lptr = new;
    } else { //list is empty
        L->Last = new;
    }
    L->First = new;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr new = malloc(sizeof(struct tDLElem));
    if (new == NULL) { //memory allocation failed, call DLError and end
        DLError();
        return;
    }
    new->data = val;
    new->rptr = NULL;
    if (L->First == NULL) { //list empty, set new element a First
        new->lptr = NULL;
        L->First = new;
    } else { //list not empty, connect Last and new element
        new->lptr = L->Last;
        L->Last->rptr = new;
    }
    L->Last = new;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First != NULL) { //list is not empty, copy first
        *val = L->First->data;
    } else { //list is empty, call DLError
        DLError();
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First != NULL && L->Last != NULL) { //list is not empty, copy last
        *val = L->Last->data;
    } else { //list is empty, call DLError
        DLError();
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L->First != NULL) { //if list is empty, do nothing
        tDLElemPtr tmp = L->First;
        if (L->First == L->Act) { //activity detected, remove activity
            L->Act = NULL;
        }
        if (L->First == L->Last) { //only one element in list
            L->First = NULL;
            L->Last = NULL;
        } else { //assign second element as first
            L->First = L->First->rptr;
            L->First->lptr = NULL;
        }
        free(tmp);
    }
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L->First != NULL && L->Last != NULL) { //if list is empty do nothing
        tDLElemPtr tmp = L->Last;
        if (L->Last == L->Act) { //last is active, remove activity
            L->Act = NULL;
        }
        if (L->Last == L->First) { //only one element in list
            L->First = NULL;
            L->Last = NULL;
        } else { //assign second last element as last
            L->Last = L->Last->lptr;
            L->Last->rptr = NULL;
        }
        free(tmp);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L->Act != NULL && L->First != NULL) { //list is active and not empty
        if (L->Act != L->Last) { //active element is not last
            tDLElemPtr tmp = L->Act->rptr;
            if (tmp->rptr != NULL) { //removed element is not last
                L->Act->rptr = tmp->rptr;
                tmp->rptr->lptr = L->Act;
            } else { //removed element is last, active element becomes last
                L->Act->rptr = NULL;
                L->Last = L->Act;
            }
            free(tmp);
        }
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if (L->Act != NULL && L->First != NULL) { //list is active and not empty
        if(L->Act != L->First) { //active element is not first
            tDLElemPtr tmp = L->Act->lptr;
            if (tmp->lptr != NULL) { //removed element is not first
                L->Act->lptr = tmp->lptr;
                tmp->lptr->rptr = L->Act;
            } else { //removed element is first, active element becomes first
                L->Act->lptr = NULL;
                L->First = L->Act;
            }
            free(tmp);
        }
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL) { //list is active
        tDLElemPtr new = malloc(sizeof(struct tDLElem));
        if (new == NULL) { //memory allocation failed, call DLError and end
            DLError();
            return;
        }
        new->data = val;
        if(L->Act->rptr == NULL) { //active element is last, new element will be the last one
            new->rptr = NULL;
            L->Last = new;
        } else { //active element is not last, join elements accordingly
            tDLElemPtr tmp = L->Act->rptr;
            tmp->lptr = new;
            new->rptr = tmp;
        }
        new->lptr = L->Act;
        L->Act->rptr = new;
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL) { //list is active
        tDLElemPtr new = malloc(sizeof(struct tDLElem));
        if (new == NULL) { //memory allocation failed, call DLError and end
            DLError();
            return;
        }
        new->data = val;
        if (L->Act->lptr == NULL) { //active element is first, new element becomes first
            new->lptr = NULL;
            L->First = new;
        } else { //active element is not first, join elements accordingly
            tDLElemPtr tmp = L->Act->lptr;
            tmp->rptr = new;
            new->lptr = tmp;
        }
        new->rptr = L->Act;
        L->Act->lptr = new;
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if (L->Act != NULL && L->First != NULL) { //list active, copy value
        *val = L->Act->data;
    } else { //list not active or empty, call DLError
        DLError();
    }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if (L->Act != NULL && L->First != NULL) { //if list is inactive or empty, do nothing
        L->Act->data = val;
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    if (L->Act != NULL && L->First != NULL) { //if list is inactive or empty, do nothing
        L->Act = L->Act->rptr;
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    if (L->Act != NULL && L->First != NULL) { //if list is inactive or empty, do nothing
        L->Act = L->Act->lptr;
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return ((L->Act != NULL && L->First != NULL) ? 1 : 0);
}

/* Konec c206.c*/
