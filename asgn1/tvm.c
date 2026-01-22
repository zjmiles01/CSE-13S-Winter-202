#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static double n = 0, i = 0, PV = 0, PMT = 0, FV = 0;

static void die(int line, const char *msg) {
    fprintf(stderr, "line %d: %s\n", line, msg);
    exit(1);
}

static int valid_name(const char *name) {
    return !strcmp(name, "n") || !strcmp(name, "i") || !strcmp(name, "PV") || !strcmp(name, "PMT")
           || !strcmp(name, "FV");
}

static void clear_all(void) {
    n = i = PV = PMT = FV = 0;
}

static void set_var(const char *name, double val, int line) {
    if (!valid_name(name)) die(line, "invalid variable name");

    if (!strcmp(name, "n")) {
        if (!(val > 0 && val == ceil(val))) die(line, "n must be a positive integer");
        n = val;
    } else if (!strcmp(name, "i")) {
        if (!(val > 0)) die(line, "i must be positive");
        i = val;
    } else if (!strcmp(name, "PV")) {
        PV = val;
    } else if (!strcmp(name, "PMT")) {
        PMT = val;
    } else { // FV
        FV = val;
    }
}

/*
We use an equivalent f(x) with the same root as equation (2), but easier to compute:
f(x) = PV*(1+i)^x + PMT*((1+i)^x - 1)/i + FV

This is consistent with the assignment’s Newton-Raphson setup and derivatives.  [oai_citation:1‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
*/

static double f_n(double x) {
    double A = pow(1.0 + i, x);
    return PV * A + PMT * (A - 1.0) / i + FV;
}
static double fp_n(double x) {
    double A = pow(1.0 + i, x);
    return log(1.0 + i) * (PV + PMT / i) * A;
}

static double f_i(double x) {
    double A = pow(1.0 + x, n);
    return PV * A + PMT * (A - 1.0) / x + FV;
}
static double fp_i(double x) {
    double A = pow(1.0 + x, n);
    double A_nm1 = pow(1.0 + x, n - 1.0);
    double term1 = n * (PV + PMT / x) * A_nm1;
    double term2 = PMT * (A - 1.0) / (x * x);
    return term1 - term2;
}

static double solve_n(int line) {
    double x = 360.0;           // initial value  [oai_citation:2‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
    double delta = 0.0;
    for (int k = 0; k <= 10000; k++) {
        delta = -f_n(x) / fp_n(x);
        x += delta;
        if (fabs(delta) < 1e-8) break;  // stop rule  [oai_citation:3‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
    }
    if (fabs(delta) >= 1e-8) die(line, "solver did not converge");
    return ceil(x); //  [oai_citation:4‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
}

static double solve_i(int line) {
    double x = 0.0025;          // initial value  [oai_citation:5‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
    double delta = 0.0;
    for (int k = 0; k <= 10000; k++) {
        delta = -f_i(x) / fp_i(x);
        x += delta;
        if (fabs(delta) < 1e-8) break;  // stop rule  [oai_citation:6‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
    }
    if (fabs(delta) >= 1e-8) die(line, "solver did not converge");
    return x;
}

static void compute(const char *name, int line) {
    if (!valid_name(name)) die(line, "invalid variable name");

    // rule: if computing anything except i, then i must not be zero  [oai_citation:7‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
    if (strcmp(name, "i") != 0 && i == 0.0) die(line, "i must not be zero");

    if (!strcmp(name, "PV")) {
        double t = pow(1.0 + i, -n);
        PV = -PMT * (1.0 - t) / i - FV * t; // eq (3)  [oai_citation:8‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
        printf("PV = %.2f\n", PV);
    } else if (!strcmp(name, "PMT")) {
        double t = pow(1.0 + i, n);
        PMT = i * (PV * t + FV) / (1.0 - t); // eq (4)  [oai_citation:9‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
        printf("pmt = %.2f\n", PMT);
    } else if (!strcmp(name, "FV")) {
        double t = pow(1.0 + i, n);
        FV = -PV * t - PMT * (t - 1.0) / i; // eq (5)  [oai_citation:10‡asgn1.pdf](sediment://file_00000000893071f5bb02998c0bdf9ca4)
        printf("FV = %.2f\n", FV);
    } else if (!strcmp(name, "n")) {
        n = solve_n(line);
        printf("n = %.0f\n", n);
    } else { // i
        i = solve_i(line);
        printf("i = %.6f\n", i);
    }
}

static void rstrip_newline(char *s) {
    size_t L = strlen(s);
    while (L > 0 && (s[L - 1] == '\n' || s[L - 1] == '\r')) {
        s[L - 1] = '\0';
        L--;
    }
}

int main(void) {
    clear_all();

    char line[1024];
    int line_no = 0;

    while (fgets(line, sizeof(line), stdin)) {
        line_no++;
        rstrip_newline(line);

        // blank line
        if (line[0] == '\0') continue;

        char name[32];
        double val;

        if (sscanf(line, "set %31s %lf", name, &val) == 2) {
            set_var(name, val, line_no);
        } else if (sscanf(line, "compute %31s", name) == 1) {
            compute(name, line_no);
        } else if (!strcmp(line, "clear")) {
            clear_all();
        } else {
            die(line_no, "invalid command");
        }
    }

    return 0;
}