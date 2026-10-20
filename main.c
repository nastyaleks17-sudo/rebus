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

long long weight[26];
int lead[26];
long long rest[11];

int check(long long sum)
{
    checked++;

    if (sum != 0)
        return 0;

    for (int i = 0; i < count; i++) {
        int L = letters[i] - 'A';
        if (lead[L] == 1 && digit[L] == 0)
            return 0;
    }

    return 1;
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

void assign(int i, long long sum)
{
    if (found) return;

    if (sum > rest[i] || -sum > rest[i])
        return;

    if (i == count) {
        if (check(sum)) found = 1;
        return;
    }

    int L = letters[i] - 'A';

    for (int d = 0; d <= 9; d++) {
        if (found) return;
        if (used[d]) continue;

        used[d] = 1;
        digit[L] = d;
        assign(i + 1, sum + weight[L] * d);
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

    for (int L = 0; L < 26; L++) {
        weight[L] = 0;
        lead[L] = 0;
    }

    for (int w = 0; w < nwords; w++) {
        long long p = 1;
        for (int j = word_len[w] - 1; j >= 0; j--) {
            if (w == nwords - 1)
                weight[words[w][j]] -= p;
            else
                weight[words[w][j]] += p;
            p = p * 10;
        }
        if (word_len[w] > 1)
            lead[words[w][0]] = 1;
    }

    rest[count] = 0;
    for (int i = count - 1; i >= 0; i--) {
        long long w = weight[letters[i] - 'A'];
        if (w < 0) w = -w;
        rest[i] = rest[i + 1] + 9 * w;
    }

    assign(0, 0);

    if (found) make_text();
}

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
        int runs = 0;
        clock_t start = clock();
        clock_t end;
        do {
            solve(tests[t]);
            runs++;
            end = clock();
        } while (end - start < CLOCKS_PER_SEC / 5);

        double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC / runs;

        printf("%-38s %7d %10.4f %12lld   %s\n",
            tests[t], count, ms, checked, found ? text : "No solution");
    }

    return 0;
}