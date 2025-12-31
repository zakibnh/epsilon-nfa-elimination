/*
 * Lab: Eliminating ε-Transitions from an NFA
 *
 * Group Members:
 *  - Benhabrou Zakaria
 *  - Habchi Samir
 *  - Boussouf Zineddine
 *
 * Programming Language: C
 */

#include <stdio.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

/* Global variables */
int nStates, nSymbols;
char symbols[MAX_SYMBOLS];
int startState = 0;

int finalStates[MAX_STATES];
int nFinalStates;

/* Transition tables */
int trans[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int epsTrans[MAX_STATES][MAX_STATES];

int newTrans[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int newFinalStates[MAX_STATES];

/* ε-closure */
int epsClosure[MAX_STATES][MAX_STATES];

/* ================= TASK 1 ================= */
void readNFA() {
    int i, j, k;
    int nTransitions;
    int from, to;
    char sym;

    printf("Number of states: ");
    scanf("%d", &nStates);

    printf("Number of symbols: ");
    scanf("%d", &nSymbols);

    printf("Symbols: ");
    for (i = 0; i < nSymbols; i++)
        scanf(" %c", &symbols[i]);

    /* Initialize tables */
    for (i = 0; i < nStates; i++) {
        for (j = 0; j < nSymbols; j++)
            for (k = 0; k < nStates; k++)
                trans[i][j][k] = 0;

        for (j = 0; j < nStates; j++)
            epsTrans[i][j] = 0;
    }

    printf("Number of transitions: ");
    scanf("%d", &nTransitions);

    printf("From  Symbol  To\n");
    for (i = 0; i < nTransitions; i++) {
        scanf("%d %c %d", &from, &sym, &to);

        if (sym == 'e') {
            epsTrans[from][to] = 1;
        } else {
            for (j = 0; j < nSymbols; j++) {
                if (symbols[j] == sym) {
                    trans[from][j][to] = 1;
                    break;
                }
            }
        }
    }

    printf("Number of final states: ");
    scanf("%d", &nFinalStates);

    printf("Final states: ");
    for (i = 0; i < nFinalStates; i++)
        scanf("%d", &finalStates[i]);
}

/* ================= TASK 2 ================= */
void computeEpsClosure() {
    int i, j, k;
    int changed;

    for (i = 0; i < nStates; i++) {
        for (j = 0; j < nStates; j++)
            epsClosure[i][j] = 0;

        epsClosure[i][i] = 1;
    }

    do {
        changed = 0;
        for (i = 0; i < nStates; i++) {
            for (j = 0; j < nStates; j++) {
                if (epsClosure[i][j]) {
                    for (k = 0; k < nStates; k++) {
                        if (epsTrans[j][k] && !epsClosure[i][k]) {
                            epsClosure[i][k] = 1;
                            changed = 1;
                        }
                    }
                }
            }
        }
    } while (changed);
}

/* ================= TASK 3 ================= */
void computeNewTransitions() {
    int i, j, k, p, q;

    for (i = 0; i < nStates; i++)
        for (j = 0; j < nSymbols; j++)
            for (k = 0; k < nStates; k++)
                newTrans[i][j][k] = 0;

    for (i = 0; i < nStates; i++) {
        for (j = 0; j < nSymbols; j++) {
            for (p = 0; p < nStates; p++) {
                if (epsClosure[i][p]) {
                    for (q = 0; q < nStates; q++) {
                        if (trans[p][j][q]) {
                            for (k = 0; k < nStates; k++) {
                                if (epsClosure[q][k])
                                    newTrans[i][j][k] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

/* ================= TASK 4 ================= */
void computeNewFinalStates() {
    int i, j, k;

    for (i = 0; i < nStates; i++)
        newFinalStates[i] = 0;

    for (i = 0; i < nStates; i++) {
        for (j = 0; j < nStates; j++) {
            if (epsClosure[i][j]) {
                for (k = 0; k < nFinalStates; k++) {
                    if (j == finalStates[k]) {
                        newFinalStates[i] = 1;
                    }
                }
            }
        }
    }
}

/* ================= TASK 5 ================= */
void displayNewNFA() {
    int i, j, k;

    printf("\n===== ε-free NFA =====\n");
    printf("Start state: %d\n", startState);

    printf("Final states: ");
    for (i = 0; i < nStates; i++)
        if (newFinalStates[i])
            printf("%d ", i);
    printf("\n\nTransitions:\n");

    for (i = 0; i < nStates; i++) {
        for (j = 0; j < nSymbols; j++) {
            printf("δ(%d, %c) = { ", i, symbols[j]);
            for (k = 0; k < nStates; k++) {
                if (newTrans[i][j][k])
                    printf("%d ", k);
            }
            printf("}\n");
        }
    }
}

/* ================= MAIN ================= */
int main() {
    readNFA();
    computeEpsClosure();
    computeNewTransitions();
    computeNewFinalStates();
    displayNewNFA();
    return 0;
}
