#include <stdio.h>
#include <time.h>

const char* rebus;

char letters[10];
int  count = 0;
int  digit[26];
int  used[10];
char text[200];
int  found = 0;
long long checked = 0;

int is_letter(char c) { return c >= 'A' && c <= 'Z'; }

int words[8][20];
int word_len[8];
int nwords = 0;

int check(void)
{
    checked++;

    long long left = 0;
    long long right = 0;

    for (int w = 0; w < nwords; w++) {
        if (word_len[w] > 1 && digit[words[w][0]] == 0)
            return 0;

        long long number = 0;
        for (int j = 0; j < word_len[w]; j++)
            number = number * 10 + digit[words[w][j]];

        if (w == nwords - 1)
            right = number;
        else
            left = left + number;
    }

    return left == right;
}

void make_text(void)
{
    int i;
    for (i = 0; rebus[i] != '\0'; i++) {
        if (is_letter(rebus[i]))
            text[i] = '0' + digit[rebus[i] - 'A'];
        else
            text[i] = rebus[i];
    }
    text[i] = '\0';
}

void assign(int i)
{
    if (found) return;

    if (i == count) {
        if (check()) found = 1;
        return;
    }

    for (int d = 0; d <= 9; d++) {
        if (found) return;
        if (used[d]) continue;

        used[d] = 1;
        digit[letters[i] - 'A'] = d;
        assign(i + 1);
        used[d] = 0;
    }
}

void solve(const char* puzzle)
{
    int seen[26] = { 0 };

    rebus = puzzle;
    count = 0;
    found = 0;
    checked = 0;
    for (int d = 0; d <= 9; d++) used[d] = 0;

    for (int i = 0; rebus[i] != '\0'; i++) {
        char c = rebus[i];
        if (is_letter(c) && seen[c - 'A'] == 0) {
            seen[c - 'A'] = 1;
            letters[count] = c;
            count++;
        }
    }

    nwords = 0;
    int len = 0;
    for (int i = 0; ; i++) {
        char c = rebus[i];
        if (is_letter(c)) {
            words[nwords][len] = c - 'A';
            len++;
        }
        else if (len > 0) {
            word_len[nwords] = len;
            nwords++;
            len = 0;
        }
        if (c == '\0') break;
    }

    assign(0);

    if (found) make_text();
}

#define REPEAT 5

int main(void)
{
    const char* tests[] = {
        "BE + BE = MOO",
        "TO + GO = OUT",
        "ODD + ODD = EVEN",
        "TWO + TWO = FOUR",
        "SEND + MORE = MONEY",
        "BIG + CAT = LION",
        "CROSS + ROADS = DANGER",
        "A + B + C + D + E + F + G = HI",
        "ELEVEN + NINE + FIVE + FIVE = THIRTY",
    };
    int n = sizeof(tests) / sizeof(tests[0]);

    printf("%-38s %7s %10s %12s   %s\n", "Rebus", "Letters", "Time, ms", "Checked", "Answer");

    for (int t = 0; t < n; t++) {
        clock_t start = clock();
        for (int r = 0; r < REPEAT; r++)
            solve(tests[t]);
        clock_t end = clock();

        double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC / REPEAT;

        printf("%-38s %7d %10.2f %12lld   %s\n",
            tests[t], count, ms, checked, found ? text : "No solution");
    }

    return 0;
}